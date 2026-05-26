;──────────────────────────────────────────────────────────────────────────────
; emulate - эмуляция кода (файлов формата Portable Executable)
;
; on start :       ip - оригинальное расположение кода
;             codeloc - смещение на код для эмуляции
;                mode - режим : 0=полный, 1-без анализа вирусных сигнатур
;           file_type - тип файла : 0-COM, 1-EXE, 2-PE
;                       на данный момент поддерживается только тип 2
; on exit  :  - - - - -
;
 proc emulate           file_type, mode, codeloc, ip
;──────────────────────────────────────────────────────────────────────────────
 local .codeloc:DWORD, .ip:DWORD                  ; temporary data

 emul_init:     pushad
                jmp     set_timer
;
 emulator_nfo           db 'EMUL%015'             ; версия эмулятора
;───────────────
;  Определим значение таймера (с момента запуска Windows (в миллисекундах)), на
; момент запуска эмулятора. Допустимое время эмуляции исполнения программы
; составляет 3000 миллисекунд. В "temp+028" сохраним конечное время эмуляции
; программы и после имитации выполенения каждой инструкции будем проверять,
; было ли оно привышено ...
;
; ps. если только таймер не отключен параметром "/t-"
;
 set_timer:     cmp     byte [emul_timer],0       ; если таймер отключен, 
                jz      ca_initialization         ; не инициализируем
                                    
; Инициализируем таймер эмулятора                
                invoke  GetTickCount
                add     eax,3100
                mov     dword [temp+028],eax
;══════════════════════════════════════════════════════════════════════════════
; процесс инициализации эмулятора:
;   1) сохранение значения регистров (см. выше)
;   2) инициализация таймера (см. выше)
;   3) инициализация анализатора кода ("ca_init")
;   4) инициализация флага "зараженности" ("ill_mark")
;   5) установка значения "виртуального стека", для эмуляции
;      обращения инструкций к стэку
;   6) установка параметров переданных в процедуру: ip (6a) / codeloc (6b)
;   7) инициализация дополнительного виртуального стека, для хранения
;      ip и codeloc для корректной эмуляции возврата (ret) из call
;
 ca_initialization:
                call    ca_init                  ; * 3
                mov     dword [ill_mark],0       ; * 4
;
                mov     esi,dword [virtual_stack_pointer]
                add     esi,virtual_stack_size
                mov     dword [regs+016],esi     ; * 5
;
                mov     eax,[ip]                 ; * 6a
                mov     dword [.ip],eax
                mov     dword [regs+0*4],eax     ; - начальное значение вирт.
                                                 ;   регистра = точке входа
                mov     esi,[codeloc]            ; * 6b
                mov     dword [.codeloc],esi
;────────────────────────────────────────────────
; Перед эмуляцией инструкции:
;  1) Проверим расположена ли текущая инструкция в области памяти
;     выделенной под эмулируемую программу
;
;  2) Запомним значения всех виртуальных регистров, содержащих
;     подсчитанные значения delta_value
;
 emulate_loop:  mov     ebx,esi
                call    test_off
                jnc     emulate_ca_fill_buf
                mov     eax,-2
                jmp     emulate_ret

 emulate_ca_fill_buf:  
                call    ca_fill_buf              ; Запомним значение delta value
;══════════════════════════════════════════════════════════════════════════════
; проверка режима работы эмулятора (0 - полный, 1 - без проверки вирусных
; сигнатур)
;
                mov     eax,[mode]
                or      eax,eax
                jnz     emulate_dasm             ; * режим 1
;
; запуск части анализатора кода, отвечающей за поиск вирусных сигнатур в
; текущей позиции.
;                                                  * режим 2
                push    [file_type]              ; - тип файла
                push    vir_no                   ; - смещение базы данных
                push    esi                      ; - местоположение для проверки
                call    detect                   ; * поиск сигнатур
                or      eax,eax                  ; eax - номер обнаруженного
                                                 ; вируса в базе (иначе = 0)
                jz      emulate_dasm             ; вирусов не обнаружено,
                                                 ; продолжим эмуляцию
;
                mov     dword [ill_mark],eax     ; если вирус обнаружен,
                                                 ; установим флаг и прекратим
                mov     eax,dword [regs+000]
                jmp     emulate_ret              ; процесс эмуляции
;══════════════════════════════════════════════════════════════════════════════
; разбор инструкции с помощью дизассемблера
;
 emulate_dasm:  push    esi                      ; - смещение инструкции
                mov     dword [emul_stop+4],esi
;
                call    disasm          ; * запуск дизассемблера
                cmp     eax,-1
                jz      emulate_ret     ; * инструкция неизвестна дизассемблеру
;
; edx - тип алгоритма, необходимого для эмуляции инструкции
;       0 - достаточно запуска в "карантине"
;       1 - требует полной имитации исполнения
;
                or      edx,edx
                jnz     full_inst_emul           ; полная имитация исполнения

                call    run_instr
                jmp     emulate_check
;══════════════════════════════════════════════════════════════════════════════
; * * * Разбор инструкций, которые требуют полной имитации исполнения
;
 full_inst_emul:
                push    esi
                pop     edi                   ; edi - смещение инструкции
                add     esi,ecx
;───────────────────────
; * * * Имитация исполнения инструкций, обращающихся к смещениям
;
;  Процесс "условно" разделен на три части, каждую я попытаюсь описать,
; как можно подробнее.
;
; Первая часть процесса:
;
;  Инструкции, которые обращаются к смещениям, требуют полной имитации
; исполнения, так как исследуемый код был перенесен в антивирусный буфер и
; расположение всех данных в памяти изменилось (относительно тех по которым
; все еще будет обращаться программа (вирус)).
; Следовательно, имитируя исполнение инструкций такого рода, нужно изменять
; смещение, по которому программа обращается из буфера, на новое. Для этого
; дизассемблер дополнительно разбирает инструкции и заполняет буфер "dregs"
; специальными параметрами инструкции, которые необходимы, для ее правильной
; эмуляции.
;
; сейчас буфер "dregs" содержит примерно следующие данные:
;  dregs + 000 : количество регистров использующихся в инструкции
;  dregs + 004 : номер регистра 1
;  dregs + 008 : номер регистра 2
;  dregs + n*4 : номер регистра n
;
;  Нам необходимо определить сумму значений i регистров использующихся в
; инструкции для указания смещения, по которому обращается инструкция и
; передать ее во вторую часть процесса. Процесс будет выглядеть примерно
; так:
;
; for (i=1,x=0; i<=кол-ва регистров; i++)
; {
;  a = dregs[i*4] * 4;
;  x+= [regs+a];
; }
;
; где:    a - временная переменная
;         i - используется как счетчик
;         x - является той самой суммой, которую необходимо узнать
;      regs - буфер в котором хранятся значение регистров эмулируемой программы
;     dregs - буфер описанный выше
;
 @found_spec:   push    ebx esi ecx
;
                mov     byte [offset_nfo],0
                sub     ebx,ebx
;
                mov     esi,dregs
                lodsd
                xchg    eax,ecx                ; ECX = количество регистров
;
                or      ecx,ecx                
                jz      spec_emul_ip           ; Нет регистров
;
 count_sum_lp:  lodsd
                cmp     byte [eax+_regs],0
                jz      count_sum              ; вирт. рег. содержит "обычные" данные
;
                cmp     byte [eax+_regs],1
                jnz     count_sum_vstack       
                mov     byte [offset_nfo],1    ; вирт. рег. содержит delta value
                jmp     count_sum
;
 count_sum_vstack:                             ; Может быть указывает на
                cmp     byte [eax+_regs],2     ; виртуальный стэк ?
                jnz     count_sum              ; НЕТ ->
;
; Проверим смещение из виртуального регистра
; 
 check_vreg_offet:
                call    mul_4
                add     ebx,dword [regs+eax]

                push    dword [regs+eax]       ; par1: OFFSET
                call    test_in_vstack         ; проверим смещение
                jc      count_sum_loop   
                mov     byte [offset_nfo],2    ; смещение указывает на вирт. стэк
                jmp     count_sum_loop
;
; Прибавим данные регистра (не delta value или virtual stack)
;
 count_sum:     call    mul_4
                add     ebx,dword [regs+eax]
;
 count_sum_loop:
                loop    count_sum_lp
;
 spec_emul_ip:  sub     eax,eax
                pop     ecx
;
                push    edi
                pop     esi
;
; Вторая часть процесса:
;
;  Каждая инструкция, требует определенных действий для имитации ее выполнения,
; некоторые действия производятся в процессе имитации всех инструкций, а
; некоторые индивидуальны. Далее идет разбор типа инструкции и процесс ее
; эмуляции.
;
 choose_emul_proc:
                mov     al,byte [edi]
;───────────────
; PUSH, POP EREG:
;
                cmp     al,050h
                jb      find_emul_proc
                cmp     al,05fh
                jbe     emul_push_pop                  
;───────────────
; NEAR JUMPS:
; jo, jno, jb, jnb, je, jne, jbe, ja, js, jns, jp, jnp, jl, jnl, jle, jg
;
 emul_check_jump_near:
                cmp     al,070h
                jb      find_emul_proc
                cmp     al,07fh
                jna     emul_jmp_near
;
 find_emul_proc:
                call    get_emul_proc
                or      eax,eax
                jnz     run_emul_proc
;
                mov     eax,-4                 ; Инструкция неизвестна эмулятору
                jmp     semul_err_ret
;
 run_emul_proc: push    eax
                sub     eax,eax
                        db 0c3h ; ( ret ) -> RUN PROC
;───────────────────────
;
;                     * * * ПРОЦЕДУРЫ ЭМУЛЯЦИИ ИНСТРУКЦИЙ * * *
;

 emul_cmps_bwd:     
                call    emul_cmps
		jmp     spec_emul_ret
;
 emul_lods_bwd:     
                call    emul_lods
		jmp     spec_emul_ret
;
 emul_stos_bwd:
                call    emul_stos
		jmp     spec_emul_ret
;
 include                "emul\emul\_instr_table.inc"
 include                "emul\emul\ret.inc"
 include                "emul\emul\cmps.inc"
 include                "emul\emul\lods.inc"
 include                "emul\emul\stos.inc"
 include                "emul\emul\loop_byte.inc"
 include                "emul\emul\jump_near.inc"
 include                "emul\emul\call_jump_far.inc"
 include                "emul\emul\push_pop.inc"
;
 include                "emul\emul\emul_000.inc"
 include                "emul\emul\emul_001.inc"
 include                "emul\emul\emul_003.inc"
 include                "emul\emul\emul_008.inc"
 include                "emul\emul\emul_009.inc"
 include                "emul\emul\emul_00a.inc"
 include                "emul\emul\emul_00f.inc"
 include                "emul\emul\emul_010.inc"
 include                "emul\emul\emul_019.inc"
 include                "emul\emul\emul_01b.inc"
 include                "emul\emul\emul_020.inc"
 include                "emul\emul\emul_021.inc"
 include                "emul\emul\emul_029.inc"
 include                "emul\emul\emul_02b.inc"
 include                "emul\emul\emul_030.inc"
 include                "emul\emul\emul_031.inc"
 include                "emul\emul\emul_032.inc"
 include                "emul\emul\emul_033.inc"
 include                "emul\emul\emul_066.inc"
 include                "emul\emul\emul_080.inc"
 include                "emul\emul\emul_081.inc"
 include                "emul\emul\emul_083.inc"
 include                "emul\emul\emul_088.inc"
 include                "emul\emul\emul_089.inc"
 include                "emul\emul\emul_08a.inc"
 include                "emul\emul\emul_08b.inc"
 include                "emul\emul\emul_08d.inc"
 include                "emul\emul\emul_08f.inc"
 include                "emul\emul\emul_0c7.inc"
 include                "emul\emul\emul_0f6.inc"
 include                "emul\emul\emul_0fe.inc"
 include                "emul\emul\emul_0ff.inc"
;
;═══════════════════════
 spec_emul_ret: pop     esi ebx                   ; Emulation completed
                jmp     emulate_check
;
 semul_err_ret: pop     esi ebx                   ; Emulation ERROR
                jmp     emulate_ret
;══════════════════════════════════════════════════════════════════════════════

;═══════════════════════
 emulate_check: 
;
;  Запустим кодо-анализатор для проверки виртуальных регистров на
; наличие в них: delta value
;
                call    ca_check_buf              
;
;  Проверим состояние таймера
;
 		cmp     byte [emul_timer],0       ; если таймер отключен, 
                jz      emulate_loop              ; не проверяем его состояние
;           
                invoke  GetTickCount              ; если было привышено макс.
                cmp     eax,dword [temp+028]      ; закончилось время, отведенное на
                jc      emulate_loop              ; эмуляцию программы? "сворачиваемся"
                sub     eax,eax
;
;               * * * Завершение работы эмулятора * * *
;
;  Если эмулятор завершил свою работу по ошибке,
; EAX <> 0 и содержит код ошибки:
;  
;  1) "-1" - Инструкция неизвестна дизассемблеру
;  2) "-2" - Инструкция обращается к смещению вне кода программы
;  3) "-3" - Указатель стека находится вне кода программы и виртуального стека 
;  4) "-4" - Инструкция неизвестна эмулятору
;  5) "-5" - Вызов API не обрабатывается эмулятором
;
 emulate_ret:   mov     dword [emul_stop],esi     ; запомним смещение, в котором
                                                  ; остановился эмулятор
 if defined DEBUG
;                int 3
 end if
                popad
                ret
;
 endp
;
;══════════════════════════════════════════════════════════════════════════════
; * * * Запуск инструкции в специальных условиях * * *
;
; on start : esi - инструкция
;            ecx - размер инструкции
;
; on exit  : - - -
;
; Описание алгоритма работы процедуры:
;
; 1) перенесем в буфер "instr_buf" инструкции для эмуляции стека и
;    разобранную дизассемблером инструкцию размера ecx.
;
;    в результате получим в "instr_buf" примерно:
;
;               mov     dword ptr [res_stack+1],esp
;               mov     esp,dword ptr [regs+016]
;           ... разобранная инструкция ...
;               mov     dword ptr [regs+016],esp
; res_stack:    mov     esp, ?
;               ret
;
 proc run_instr         
;
                mov     edi,instr_buf
                mov     ax,2589h
                stosw                           ; перенесем "mov 4 ptr [?],esp"
;───────────────
                mov     eax,instr_buf           ; расчитаем смещение
                add     eax,ecx                 ; "res_stack+1" (см. выше) и
                add     eax,6+6+6+1
                stosd                           ; перенесем "остаток" инструкции
;───────────────
                mov     ax,258Bh
                stosw
                mov     eax,regs+016             ; перенесем инструкцию
                stosd                            ; "mov esp,4 ptr [regs+16]"
;───────────────
                rep     movsb                    ; перенесем разобранную
                                                 ; дизассемблером инструкцию
;───────────────
                mov     ax,2589h
                stosw
                mov     eax,regs+016             ; перенесем инструкцию
                stosd                            ; "mov 4 ptr [regs+16],esp"
;───────────────
                mov     al,0bch
                stosb                            ; перенесем инструкцию
                stosd                            ; "mov esp,?"
                mov     al,0c3h
                stosb                            ; перенесем "ret"
;═══════════════════════
; 2) запуск разобранной инструкции в специальных условиях (карантине)
;
; запомним значение регистров программы-эмулятора в стеке
;
                pushad
;
; запомним значение флагов программы-эмулятора в стеке, затем перенесем
; в регистр eax, а из него в переменную "temp"
;
 save_orig_efl: pushfd
                mov     eax,dword [esp]
                mov     dword [temp],eax
                popfd
;
; значение флагов эмулируемого кода перенесем из переменной "flags" в регистр
; eax, а из него в стек использующийся программой-эмулятором
;
 set_emul_efl:  pushfd
                mov     eax,dword [flags]
                mov     dword [esp],eax
;
; установим значения регистров эмулируемого кода из переменной "regs"
; значение esp не устанавливается, иначе мы рискуем испортить значение флагов
; эмулируемого кода, стек будет перенаправлен в подпрограмме "instr_buf"
;
                mov     eax,dword [regs+000]
                mov     ecx,dword [regs+004]
                mov     edx,dword [regs+008]
                mov     ebx,dword [regs+012]
                mov     ebp,dword [regs+020]
                mov     esi,dword [regs+024]
                mov     edi,dword [regs+028]
;
; инструкция "popfd" установит значение флагов эмулируемого кода,
; которое мы сохраняли выше
;
                popfd
                call    instr_buf                ; запустим на исполнение
;
; сохраним новые значения регистров эмулируемого кода в переменной "regs"
;
                mov     dword [regs+000],eax
                mov     dword [regs+004],ecx
                mov     dword [regs+008],edx
                mov     dword [regs+012],ebx
                mov     dword [regs+020],ebp
                mov     dword [regs+024],esi
                mov     dword [regs+028],edi
;
; новое значение флагов эмулируемого кода запомним в стеке, затем перенесем
; в регистр eax и сохраним в переменной "flags"
;
 save_emul_efl: pushfd
                mov     eax,dword [esp]
                mov     dword [flags],eax
                popfd
;
; установим значение флагов программы-эмулятора из переменной "temp"
;
 set_orig_efl:  pushfd
                mov     eax,dword [temp]
                mov     dword [esp],eax
                popfd
;
; восстановим значение регистров программы-эмулятора
;
                popad
                ret
;
 endp
;
 include               "emul\emul\emul_lib.inc"
 include               "emul\emulator_lib.inc"     ; Some useful subroutines
 include               "emul\apis\emul_api.inc"    ; API calls emulator          