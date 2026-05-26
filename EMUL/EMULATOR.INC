.code
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
 emulate        proc   file_type:dword, mode:dword, codeloc:dword, ip:dword
;──────────────────────────────────────────────────────────────────────────────
 emul_init:     pushad
                jmp     set_timer
;
 emulator_nfo           db 'EMULx007'            ; версия эмулятора
;───────────────
;  Определим значение таймера (с момента запуска Windows (в миллисекундах)), на
; момент запуска эмулятора. Допустимое время эмуляции исполнения программы
; составляет 3000 миллисекунд. В "temp+028" сохраним конечное время эмуляции
; программы и после имитации выполенения каждой инструкции будем проверять,
; было ли оно привышено ...
;
 set_timer:     call    GetTickCount
                add     eax,3000
                mov     dword ptr [temp+028],eax
;══════════════════════════════════════════════════════════════════════════════
; процесс инициализации эмулятора:
;   1) сохранение значения регистров (см. выше)
;   2) инициализация таймера (см. выше)
;   3) инициализация анализатора кода ("ca_init")
;   4) инициализация флага "зараженности" ("ill_mark")
;   5) установка значения "виртуального стека", для эмуляции
;      обращения инструкций к стэку
;   6) установка параметров переданных в процедуру: ip (6a) /codeloc (6b)
;
                call    ca_init                  ; * 3
                mov     dword ptr [ill_mark],0   ; * 4
;
                mov     esi,offset _stack+100000
                mov     dword ptr [regs+016],esi ; * 5
;
                mov     eax,ip                   ; * 6a
                mov     dword ptr [regs+0*4],eax ; - начальное значение вирт.
                                                 ;   регистра = точке входа
                mov     esi,codeloc              ; * 6b
;────────────────────────────────────────────────
;  перед эмуляцией инструкции, запомним значения всех виртуальных регистров,
; содержащих подсчитанные значения delta_value
;
 emulate_loop:  call    ca_fill_buf                
;══════════════════════════════════════════════════════════════════════════════
; "хитрости" эмуляции файлов упакованных UPX
;
                cmp     byte ptr [temp+020],1    ; Файл упакован утилитой UPX ? ;)
                jnz     not_repescasb            ; нет
;
; В завершаюшем этапе распаковки исполняемых файлов (упакованных утилитой UPX),
; после инструкции "repe scasb" начинаются инструкции, эмуляция которых не
; обязательна.
; Пропустим их эмуляцию и перейдем к проверке распакованной программы.
;
                cmp     word ptr [esi],0aef2h    ; дошли до инструкции
                                                 ;               "repe scasb" ?
                jnz     not_repescasb            ; еще нет
;
                inc     esi
                inc     esi                      ; пропустим "repe scasb"
                mov     ecx,30
 upx_srch_jmp:  cmp     word ptr [esi-1],0e961h  ; ищем "jmp" на расшифрованный код
                jz      not_repescasb            ; перейдем к эмуляции перехода
                inc     esi
                loop    upx_srch_jmp
;══════════════════════════════════════════════════════════════════════════════
; проверка режима работы эмулятора (0 - полный, 1 - без проверки вирусных
; сигнатур)
;
 not_repescasb: mov     eax,mode
                or      eax,eax
                jnz     emulate_dasm             ; * режим 1
;
; запуск части анализатора кода, отвечающей за поиск вирусных сигнатур в
; текущей позиции.
;                                                  * режим 2
                push    file_type                ; - тип файла
                push    offset vir_no            ; - смещение базы данных
                push    esi                      ; - местоположение для проверки
                call    detect                   ; * поиск сигнатур
                or      eax,eax                  ; eax - номер обнаруженного
                                                 ; вируса в базе (иначе = 0)
                jz      emulate_dasm             ; вирусов не обнаружено,
                                                 ; продолжим эмуляцию
;
                mov     dword ptr [ill_mark],eax ; если вирус обнаружен,
                                                 ; установим флаг и прекратим
                jmp     emulate_ret              ; процесс эмуляции
;══════════════════════════════════════════════════════════════════════════════
; разбор инструкции с помощью дизассемблера
;
 emulate_dasm:  push    esi                      ; - смещение инструкции
                call    disasm                   ; * запуск дизассемблера
                cmp     eax,-1
                jz      emulate_ret     ; * инструкция неизвестна дизассемблеру
;
; edx - тип алгоритма, необходимого для эмуляции инструкции
;       0 - достаточно запуска в "карантине"
;       1 - требует полной имитации исполнения
;
                or      edx,edx
                jnz     fullinst_emul            ; полная имитация исполнения
;══════════════════════════════════════════════════════════════════════════════
; * * * Запуск инструкции в специальных условиях
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
 prep_to_run:   mov     edi,offset instr_buf
                mov     ax,2589h
                stosw                           ; перенесем "mov 4 ptr [?],esp"
;───────────────
                mov     eax,offset instr_buf    ; рассчитаем смещение
                add     eax,ecx                 ; "res_stack+1" (см. выше) и
                add     eax,6+6+6+1
                stosd                           ; перенесем "остаток" инструкции
;───────────────
                mov     ax,258Bh
                stosw
                mov     eax,offset regs+016      ; перенесем инструкцию
                stosd                            ; "mov esp,4 ptr [regs+16]"
;───────────────
                rep     movsb                    ; перенесем разобранную
                                                 ; дизассемблером инструкцию
;───────────────
                mov     ax,2589h
                stosw
                mov     eax,offset regs+016      ; перенесем инструкцию
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
 run_instr:     pushad
;
; запомним значение флагов программы-эмулятора в стеке, затем перенесем
; в регистр eax, а из него в переменную "temp"
;
 save_orig_efl: pushfd
                mov     eax,dword ptr [esp]
                mov     dword ptr [temp],eax
                popfd
;
; значение флагов эмулируемого кода перенесем из переменной "flags" в регистр
; eax, а из него в стек использующийся программой-эмулятором
;
 set_emul_efl:  pushfd
                mov     eax,dword ptr [flags]
                mov     dword ptr [esp],eax
;
; установим значения регистров эмулируемого кода из переменной "regs"
; значение esp не устанавливается, иначе мы рискуем испортить значение флагов
; эмулируемого кода, стек будет перенаправлен в подпрограмме "instr_buf"
;
                mov     eax,dword ptr [regs+000]
                mov     ecx,dword ptr [regs+004]
                mov     edx,dword ptr [regs+008]
                mov     ebx,dword ptr [regs+012]
                mov     ebp,dword ptr [regs+020]
                mov     esi,dword ptr [regs+024]
                mov     edi,dword ptr [regs+028]
;
; инструкция "popfd" установит значение флагов эмулируемого кода,
; которое мы сохраняли выше
;
                popfd
                call    instr_buf                ; запустим на исполнение
;
; сохраним новые значения регистров эмулируемого кода в переменной "regs"
;
                mov     dword ptr [regs+000],eax
                mov     dword ptr [regs+004],ecx
                mov     dword ptr [regs+008],edx
                mov     dword ptr [regs+012],ebx
                mov     dword ptr [regs+020],ebp
                mov     dword ptr [regs+024],esi
                mov     dword ptr [regs+028],edi
;
; новое значение флагов эмулируемого кода запомним в стеке, затем перенесем
; в регистр eax и сохраним в переменной "flags"
;
 save_emul_efl: pushfd
                mov     eax,dword ptr [esp]
                mov     dword ptr [flags],eax
                popfd
;
; установим значение флагов программы-эмулятора из переменной "temp"
;
 set_orig_efl:  pushfd
                mov     eax,dword ptr [temp]
                mov     dword ptr [esp],eax
                popfd
;
; восстановим значение регистров программы-эмулятора
;
                popad
                jmp     emulate_check
;══════════════════════════════════════════════════════════════════════════════
; * * * Разбор инструкций, которые требуют полной имитации исполнения
;
 fullinst_emul: push    esi
                pop     edi                   ; edi - смещение инструкции
                add     esi,ecx               ;       которую разбирали
;─────────────────────── инструкции размером 1 байт
 @one_byte:     cmp     cl,1
                jnz    @two_bytes
                cmp     byte ptr [edi],0AAh
                jz     @stos_bwd              ; " stosb "
                cmp     byte ptr [edi],0ACh
                jz     @lods_bwd              ; " lodsb "
                cmp     byte ptr [edi],0C3h
                jz     @found_ret             ; " ret "
                jmp     emulate_ret
;─────────────────────── инструкции размером 2 байта
 @two_bytes:    cmp     cl,2
                jnz    @three_bytes
                cmp     byte ptr [edi],031h   ;
                jz     @found_spec            ; " xor dword ptr [ereg],ereg "
;───────────────
 @check_jmpb:   cmp     byte ptr [edi],070h   ;─ инструкции:
                jc      emulate_ret           ;─ jo, jno, jb, jnb, je, jne,
                cmp     byte ptr [edi],07fh   ;─ jbe, ja, js, jns, jp, jnp,
                jna    @found_jmpb            ;─ jl, jnl, jle, jg
;───────────────
                cmp     byte ptr [edi],088h
                jc      emulate_ret
                cmp     byte ptr [edi],08Bh
                jna    @found_spec
;───────────────
 @check_loop:   cmp     byte ptr [edi],0E2h
                jz     @found_loop            ; " loop byte "
                cmp     byte ptr [edi],0EBh
                jz     @found_jmpb            ; " jmp byte "
                jmp     emulate_ret
;─────────────────────── инструкции размером 3 байта
 @three_bytes:  cmp     cl,3
                jnz    @four_bytes
                cmp     byte ptr [edi],030h
                jz     @found_spec            ; " xor byte ptr [eregA+eregB],reg "
                cmp     byte ptr [edi],031h
                jz     @found_spec            ; " xor dword ptr [eregA+eregB],ereg "
                cmp     byte ptr [edi],080h
                jz     @found_spec            ; " cmp byte ptr [ereg],byte "
                cmp     byte ptr [edi],08Ah
                jz     @found_spec            ; " mov [reg],byte ptr [ereg+byte] "
                cmp     byte ptr [edi],08Bh
                jz     @found_spec            ; " mov [ereg],dword ptr [ereg+byte] "
                cmp     byte ptr [edi],08Fh
                jz     @found_spec            ; " pop dword ptr [eregA+eregB] "
                cmp     byte ptr [edi],0FFh   ;
                jz     @found_spec            ; " push dword ptr [eregA+eregB] "
                jmp     emulate_ret
;─────────────────────── инструкции размером 4 байта
 @four_bytes:   cmp     cl,4
                jnz    @five_bytes
                cmp     byte ptr [edi],083h
                jz     @found_spec            ; " add byte ptr [esp+byte],byte "
                                              ; " xor byte ptr [esp+byte],byte "
                jmp     emulate_ret
;─────────────────────── инструкции размером 5 байт
 @five_bytes:   cmp     cl,5
                jnz    @six_bytes
                cmp     byte ptr [edi],066h
                jz     @found_spec            ; " xor word ptr [ereg],word "
                cmp     byte ptr [edi],0E8h
                jz     @found_call            ; " call dword "
                cmp     byte ptr [edi],0E9h
                jz     @found_jmp             ; " jmp dword "
                jmp     emulate_ret
;─────────────────────── инструкции размером 6 байт
 @six_bytes:    cmp     cl,6
                jnz    @seven_bytes
                cmp     byte ptr [edi],00Fh
                jnz    @six_bytes_2
;─
 @check_jmpd:   cmp     byte ptr [edi+1],080h ;─ инструкции:
                jc      emulate_ret           ;─ jo, jno, jb, jnb, je, jne,
                cmp     byte ptr [edi+1],08fh ;─ jbe, ja, js, jns, jp, jnp,
                jna    @found_jmpd            ;─ jl, jnl, jle, jg
;─
 @six_bytes_2:  cmp     byte ptr [edi],066h
                jz     @found_spec            ; " xor  word ptr [ebp],word "
                cmp     byte ptr [edi],081h
                jz     @found_spec            ; " xor  dword ptr [ereg], dword "
                cmp     byte ptr [edi],0FEh
                jz     @found_spec            ; " dec  byte ptr [ereg+dword] "
                cmp     byte ptr [edi],0FFh
                jz     @found_spec            ; " call [ereg+dword] "
                                              ; " dec  dword ptr [ereg+dword] "
                jmp     emulate_ret
;─────────────────────── инструкции размером 7 байт
 @seven_bytes:  cmp     cl,7
                jnz    @eight_bytes
                cmp     byte ptr [edi],080h
                jz     @found_spec            ; " xor byte ptr [ereg+dword],byte "
                cmp     byte ptr [edi],081h
                jz     @found_spec            ; " xor dword ptr [esp],dword "
                cmp     byte ptr [edi],0FFh
                jz     @found_spec            ; " dec  dword ptr [esp+dword] "
                                              ; " dec  dword ptr [erega+eregb+dword] "
                jmp     emulate_ret
;─────────────────────── инструкции размером 8 байт
 @eight_bytes:  cmp     cl,8
                jnz    @ten_bytes
                cmp     byte ptr [edi],080h
                jz     @found_spec            ; " xor byte ptr [esp+dword],byte "
                jmp     emulate_ret
;─────────────────────── инструкции размером 10 байт
 @ten_bytes:    cmp     cl,10
                jnz     emulate_ret
                cmp     byte ptr [edi],081h
                jz     @found_spec            ; " add dword ptr [esp+dword],dword "
                                              ; " xor dword ptr [esp+dword],dword "
                jmp     emulate_ret
;═══════════════════════════════════════════════ инструкции размером 1 байт
;────────────────────────
; * * * Имитация исполнения инструкции "stosb/w/d"
;
 @stos_bwd:     pushad
                mov     esi,dword ptr [regs+7*4]
                cmp     byte ptr [007+_regs],0
                jnz     @stos_bwd_002
                sub     esi,ip
                add     esi,codeloc
;
 @stos_bwd_001: mov     ebx,esi
                call    test_off
                jnc     @stos_bwd_002
                popad
                jmp     emulate_ret
;
 @stos_bwd_002: mov     ah,0c3h
                mov     al,byte ptr [edi]
                mov     word ptr [instr_buf],ax
;
                mov     eax,dword ptr [regs+0*4]
                mov     edi,dword ptr [regs+7*4]
                call    instr_buf
                popad
;
                sub     eax,eax
                mov     al,byte ptr [edi]
                sub     al,0A9h
                add     dword ptr [regs+7*4],eax
                jmp     emulate_check
;────────────────────────
; * * * Имитация исполнения инструкции "lodsb/w/d"
;
 @lods_bwd:     pushad
                mov     esi,dword ptr [regs+6*4]
                cmp     byte ptr [006+_regs],0
                jnz     @lods_bwd_002
                sub     esi,ip
                add     esi,codeloc
;
 @lods_bwd_001: mov     ebx,esi
                call    test_off
                jnc     @lods_bwd_002
                popad
                jmp     emulate_ret
;
 @lods_bwd_002: mov     ah,0c3h
                mov     al,byte ptr [edi]
                mov     word ptr [instr_buf],ax
;
                mov     eax,dword ptr [regs+0*4]
                call    instr_buf
                mov     dword ptr [regs+0*4],eax
                popad
;
                sub     eax,eax
                mov     al,byte ptr [edi]
                sub     al,0ABh
                add     dword ptr [regs+6*4],eax
                jmp     emulate_check
;────────────────────────
; * * * Имитация исполнения инструкции "ret"
;
; имитировать исполнение инструкции очень просто:
;
; 1) необходимо взять двойное слово из стека эмулируемого кода
;    ( смещение на это слово содержится в "regs+16" )
; 2) это слово будет новым указателем на смещение разбираемой эмулятором
;    инструкции (значение регистра esi)
; 3) убрать из стека эмулируемого кода, взятое значение
;    add dword ptr [regs+16],4
;
 @found_ret:    mov     esi,dword ptr [regs+016]
                lodsd
                add     dword ptr [regs+016],4
                xchg    esi,eax
                jmp     emulate_check
;═══════════════════════════════════════════════ инструкции размером 2 байта
; * * * Имитация исполнения "условных" и "безусловных" переходов:
;
;       jo, jno, jb, jnb, je, jne, jbe, ja, js,
;       jns, jp, jnp, jl, jnl, jle, jg & jmp byte
;
; 1) Перенесем в буфер "instr_buf" инструкции для имитации исполнения перехода.
;    В результате получим в "instr_buf" :
;
;               инструкция для (без)условного перехода к метке _label
;               inc     eax
; _label:       ret
;
; 2) установим значение флагов эмулируемого кода (из переменной "flags")
;    обнулим регистр eax
; 3) запустим на исполнение содержимое "instr_buf"
;    в том случае, если переход выполнится, значение eax будет равно нулю
;
;  Если переход выполнился, соотвественно необходимо изменить смещение,
; исследуемого кода (значение регистра esi). Для этого рассмотрим второй байт
; инструкции, который содержит данные для вычисления нового смещения.
;
 @found_jmpb:   mov     al,byte ptr [edi]
                call    test_jmpb
                or      eax,eax
                jnz     emulate_check
;───────────────
 jmpb_goto:     cmp     byte ptr [edi+1],07fh
                jna     jmpb_down
 jmpb_up:       sub     al,byte ptr [edi+1]
                sub     esi,eax
                jmp     emulate_check
 jmpb_down:     mov     al,byte ptr [edi+1]
                add     esi,eax
                jmp     emulate_check                 
;═══════════════
 test_jmpb:     mov     ah,1
                push    edi
                mov     edi,offset instr_buf
                stosw
                mov     ax,0c340h
                stosw
                pop     edi
;───────────────
 jmp_ser_eflag: pushfd
                pushfd
                mov     eax,dword ptr [flags]
                mov     dword ptr [esp],eax
                sub     eax,eax
                popfd
                call    instr_buf
                popfd
 _ret                   db 0c3h                        
;══════════════════════════════════════════════════════════════════════════════
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
 @found_spec:   push    ebx esi ecx ebp
                sub     ebp,ebp
                sub     ebx,ebx
                mov     esi,offset dregs
                lodsd
                xchg    eax,ecx
 count_sum_lp:  lodsd
                cmp     byte ptr [eax+_regs],0 ; регистр содержит вычисленное delta value?
                jz      count_sum
                inc     ebp
 count_sum:     call    mul_4
                add     ebx,dword ptr [regs+eax]
                loop    count_sum_lp
;
; Вторая часть процесса:
;
;  Расчет разницы между старым местоположением в памяти и текущим. Полученное
; значение добавить к ebx.
; В том случае, если один из регистров, использующихся в разбираемой инструкции
; будет содержать подсчитанное delta value (см. "\det\ca_ip.inc"),
; необходимость во второй части процесса отпадает.
;
;  Кодо-анализатор антивируса, "умеет" определять выполнялась ли процедура
; подсчета, в исследуемом коде и какой из регистров содержит delta value.
; Если в ходе подсчета смещения использовался регистр, который содержал delta
; value, то значение регистра ebp > 0, иначе оно равно нулю.
;
                or      ebp,ebp
                pop     ebp
                jnz     spec_emul_ip                
                sub     ebx,ip
                add     ebx,codeloc
;
 spec_emul_ip:  sub     eax,eax
                pop     ecx
                push    edi
                pop     esi
;
; Третья часть процесса:
;
;  Каждая инструкция, требует определенных действий для имитации ее выполнения,
; некоторые действия производятся в процессе имитации всех инструкций, а
; некоторые индивидуальны. Далее идет разбор типа инструкции и процесс ее
; эмуляции.
;
 spec_emul_lp:  mov     al,byte ptr [edi]
                cmp     al,030h
                jz      spec_emul_001
                cmp     al,031h
                jz      spec_emul_001x
                cmp     al,066h
                jz      spec_emul_002
                cmp     al,080h
                jz      spec_emul_003
                cmp     al,081h
                jz      spec_emul_004
                cmp     al,088h
                jz      spec_emul_005
                cmp     al,089h
                jz      spec_emul_006
                cmp     al,08Ah
                jz      spec_emul_007
                cmp     al,08Bh
                jz      spec_emul_008
                cmp     al,08Fh
                jz      spec_emul_009
                cmp     al,0FEh
                jz      spec_emul_010
                cmp     al,0FFh
                jz      spec_emul_011
                jmp     spec_emul_ret
;───────────────────────
;
; xor byte ptr [eregA],[reg]               │ 030HEX, 00regrgA
; xor byte ptr [esp],[reg]                 │ 030HEX, 00REGesp, 00espESP
; xor byte ptr [eregA+eregB],reg           │ 030HEX, 00reg100, 00rgArgB
;
 spec_emul_001: mov     al,byte ptr [edi+1]
                cmp     cl,2
                jnz     spec_emul_001a
                call    two_regs
                jmp     spec_emul_001c
 spec_emul_001a:shr     al,3
 spec_emul_001c:call    mul_4
                mov     edx,dword ptr [regs+eax]
                call    test_off
                jc      semul_err_ret
;
                xor     byte ptr [ebx],dl
                jmp     spec_emul_ret
;───────────────────────
;
; xor dword ptr [eregA],[eregB]            │ 031HEX, 00rgBrgA
; xor dword ptr [esp],[ereg]               │ 031HEX, 00REGesp, 00espESP
; xor dword ptr [eregA+eregB],ereg         │ 031HEX, 00reg100, 00rgArgB
;
 spec_emul_001x:mov     al,byte ptr [edi+1]
                cmp     cl,2
                jnz     spec_emul_001b
                call    two_regs
                jmp     spec_emul_001d
 spec_emul_001b:shr     al,3
 spec_emul_001d:call    mul_4
                mov     edx,dword ptr [regs+eax]
                call    test_off
                jc      semul_err_ret
;
                xor     dword ptr [ebx],edx
                jmp     spec_emul_ret
;───────────────────────
;
; xor word ptr [ereg],word │ 066HEX, 081HEX,         , word         = 5 bytes
; xor word ptr  [ebp],word │ 066HEX, 081HEX,   075HEX, 000HEX, WORD = 6 bytes
;
 spec_emul_002: add     esi,ecx
                dec     esi
                dec     esi
                lodsw
                call    test_off
                jc      semul_err_ret
;
                xor     word ptr [ebx],ax
                jmp     spec_emul_ret
;───────────────────────
;
; cmp byte ptr [ereg],byte                 │ 080HEX, 111reg, byte
; xor byte ptr [ereg+dword],byte           │ 080HEX, 10110reg,  dword, byte
; xor byte ptr [esp+dword],byte            │ 080HEX, 10110esp, 024HEX, dword, byte
;
 spec_emul_003: cmp     cl,3
                jz      spec_emul_003b 
                sub     cl,5
                add     ebx,dword ptr [esi+2]
                mov     al,byte ptr [esi+ecx+4]
                xor     byte ptr [ebx],al
                jmp     spec_emul_ret
 spec_emul_003b:push    ecx esi edi
                xchg    esi,edi
                mov     edi,offset instr_buf
                cld
                rep     movsb
                mov     al,0c3h
                stosb
                mov     byte ptr [instr_buf+1],03bh
                pop     edi esi ecx
;───────────────
 cmp_set_eflag: pushfd
                pushfd
                mov     eax,dword ptr [flags]
                mov     dword ptr [esp],eax
                popfd
                call    instr_buf
                pushfd
                mov     eax,dword ptr [esp]
                mov     dword ptr [flags],eax
                popfd
                popfd
                jmp     spec_emul_ret
;───────────────────────
;
; xor dword ptr [ereg],dword               │ 081HEX, 00110reg, dword = 6 bytes
; xor dword ptr [esp],dword                │ 081HEX, 00110100, 024HEX, dword = 7 bytes
; add dword ptr [ereg+dword],dword         │ 081HEX, 10000reg, dword, dword = 10 bytes
; add dword ptr [esp+dword],dword          │ 081HEX, 10000100, 024HEX, dword, dword = 11 bytes
; xor dword ptr [ereg+dword],dword         │ 081HEX, 10110reg, dword, dword = 10 bytes
; xor dword ptr [esp+dword],dword          │ 081HEX, 10110100, 024HEX, dword = 11 bytes
;
 spec_emul_004: mov     eax,dword ptr [esi+ecx-4]
                cmp     byte ptr [esi],10000000b
                jae     spec_emul_004b
;
                call    test_off
                jc      semul_err_ret
                xor     dword ptr [ebx],eax
                jmp     spec_emul_ret
;═══════════════
 spec_emul_004b:add     ebx,dword ptr [esi+ecx-8]
                call    test_off
                jc      semul_err_ret
;
                push    eax edi
                mov     al,byte ptr [edi+1]
                sub     al,80h
                xor     al,byte ptr [dregs+004]
                inc     al
                mov     ah,03h
                mov     edi,offset instr_buf
                stosw
                mov     al,0c3h
                stosb
                pop     edi eax
                call    instr_buf
                jmp     spec_emul_ret
;───────────────────────
;
; mov byte ptr [ereg],[reg]                │ 088HEX,00regErg
;
 spec_emul_005: mov     al,byte ptr [edi+1]
                call    reg8_pos
                mov     dl,byte ptr [regs+eax]
                call    test_off
                jc      semul_err_ret
;
                mov     byte ptr [ebx],dl
                jmp     spec_emul_ret
;───────────────────────
;
; mov dword ptr [ergA],[ergB]              │ 089HEX,00rgBrgA
;
 spec_emul_006: mov     al,byte ptr [edi+1]
                call    two_regs
                call    mul_4
                mov     edx,dword ptr [regs+eax]
                call    test_off
                jc      semul_err_ret
;
                mov     dword ptr [ebx],edx
                jmp     spec_emul_ret
;───────────────────────
;
; mov [reg],byte ptr [ereg]                │ 08AHEX, 00regErg
; mov [reg],byte ptr [esp]                 │ 08AHEX, 00regESP, 00espESP
; mov [reg],byte ptr [ebp]                 │ 08AHEX, 01regEBP, 00000000
; mov [reg],byte ptr [ereg+byte]           │ 08AHEX, 01regErg, byte
; mov [reg],byte ptr [esp+byte]            │ 08AHEX, 01regESP, 00espESP, byte
;
 spec_emul_007: mov     al,byte ptr [esi+1]
                cmp     al,01000000b
                jc      spec_emul_007b
                xor     al,01000000b
                sub     edx,edx
                mov     dl,byte ptr [esi+ecx-1]
                add     ebx,edx
;
 spec_emul_007b:call    reg8_pos
                call    test_off
                jc      semul_err_ret
                mov     dl,byte ptr [ebx]
                mov     byte ptr [regs+eax],dl
                jmp     spec_emul_ret
;───────────────────────
;
; mov [eregA],dword ptr [eregB]            │ 08BHEX, 00rgArgB
; mov [ereg],dword ptr [esp]               │ 08BHEX, 00regESP, 00espESP
; mov [ereg],dword ptr [ebp]               │ 08BHEX, 01regEBP, 00000000
; mov [eregA],dword ptr [eregB+byte]       │ 08BHEX, 01rgArgB, byte
; mov [ereg],dword ptr [esp+byte]          │ 08BHEX, 01regESP, 00espESP, byte
;
;  Имитация исполнения выше перечисленных инструкций почти полностью совпадает
; с, теми, которые разбираются в "spec_emul_007".
;
 spec_emul_008: mov     al,byte ptr [esi+1]
                cmp     al,01000000b
                jc      spec_emul_008b
                xor     al,01000000b
                sub     edx,edx
                mov     dl,byte ptr [esi+ecx-1]
                add     ebx,edx
;
 spec_emul_008b:call    two_regs
                call    mul_4
                call    test_off
                jc      semul_err_ret
;
                mov     edx,dword ptr [ebx]
                mov     dword ptr [regs+eax],edx
                jmp     spec_emul_ret
;───────────────────────
;
; pop dword ptr [eregA+eregB]              │ 08FHEX, 00000100, 00rgBrgA
;
 spec_emul_009:
                call    test_off
                jc      semul_err_ret
;
                call    get_from_vstack
                mov     dword ptr [ebx],eax
                jmp     spec_emul_ret
;───────────────────────
;
; dec  dword ptr [ereg+dword]              │ 0FEHEX, 10001reg,   dword
; dec  dword ptr [esp+dword]               │ 0FEHEX, 10001100,  024HEX, dword
;
 spec_emul_010: lodsw
                cmp     ah,10001000b
                jc      spec_emul_ret
                cmp     ah,10001111b
                ja      spec_emul_ret
;
                add     ebx,dword ptr [esi-2+ecx-4]
                call    test_off
                jc      semul_err_ret
                dec     byte ptr [ebx]
                jmp     spec_emul_ret
;───────────────────────
;
; push dword ptr [eregA+eregB]             │ 0FFHEX, 00110100, 00rgBrgA
; dec  dword ptr [ereg+dword]              │ 0FFHEX, 10001reg, dword
; dec  dword ptr [esp+dword]               │ 0FFHEX, 10001100,   024HEX, dword
; dec  dword ptr [eregA+eregB+dword]       │ 0FFHEX, 10001100, 00rgArgB, dword
; call [ereg+dword]                        │ 0FFHEX, 10010reg, dword
;
 spec_emul_011: lodsb
                lodsb
                cmp     al,00110100b
                jz      spec_emul_011a
;
                add     ebx,dword ptr [esi-2+ecx-4]
;
                cmp     al,10001000b
                jc      spec_emul_ret
                cmp     al,10001111b
                jna     spec_emul_011b
;
                cmp     al,10010000b
                jc      spec_emul_ret
                cmp     al,10010111b
                jna     spec_emul_011c
                jmp     spec_emul_ret
;═══════════════
 spec_emul_011a:call    test_off
                jc      semul_err_ret
;
                mov     eax,dword ptr [ebx]
                call    put_to_vstack
                jmp     spec_emul_ret
;═══════════════
 spec_emul_011b:
                call    test_off
                jc      semul_err_ret
                dec     dword ptr [ebx]
                jmp     spec_emul_ret
;═══════════════
 spec_emul_011c:push    eax edi
                mov     eax,esi
                sub     dword ptr [regs+016],4
                mov     edi,dword ptr [regs+016]
                stosd
                pop     edi eax
                call    test_off
                jc      semul_err_ret
                mov     esi,dword ptr [ebx]
;═══════════════════════
 spec_emul_ret: pop     esi ebx
                jmp     emulate_check
 semul_err_ret: pop     esi ebx
                jmp     emulate_ret
;══════════════════════════════════════════════════════════════════════════════
;───────────────
; * * * Имитация исполнения инструкции "loop byte"
;
; 1) Необходимо уменьшить значение виртуельного регистра "ecx" (regs+004) на 1
; 2) Если полученное значение регистра будет равно 0, переходим к смещению
;    данные о котором содержатся в инструкции
;
 @found_loop:   dec     dword ptr [regs+004]   ; уменьшим значение регистра
                cmp     dword ptr [regs+004],0 ; если "ecx" эмулируемого кода
                jz      emulate_check          ; равно 0, выйдем из цикла
                sub     esi,ecx                ; иначе рассчитаем смещение
                sub     esi,eax                ; начала цикла
                jmp     emulate_check          ; перейдем к следующей инструкии
;═══════════════════════════════════════════════ инструкции размером 5 байт
;───────────────
; * * * Имитация исполнения инструкций: call dword
;                                       jmp  dword
;
;  Необходимо изменить смещение (значение esi) инструкции, которая будет
; следующей исполняться эмулятором.
;  В случае если мы разбираем "call", необходимо сохранить в виртуальном стеке
; смещение инструкции следующей прямо за "call"'ом.
;
 @found_call:   push    eax edi
                mov     eax,esi
                sub     dword ptr [regs+016],4
                mov     edi,dword ptr [regs+016]
                stosd
                pop     edi eax
;
 @found_jmp:    sub     eax,eax
                cmp     dword ptr [edi+ecx-4],7fffffffh
                ja      jmp_up
 jmp_down:      add     esi,dword ptr [edi+ecx-4] ; изменим esi
                jmp     jmp_chk_type
 jmp_up:        sub     eax,dword ptr [edi+ecx-4]
                sub     esi,eax                   ; изменим esi
;
;  Если эмулируем файл формата PE и он не упакован UPX, то необходимо
; откорректировать смещение, на случай если был совершен переход из одной
; секции в другую.
;
 jmp_chk_type:  mov     eax,file_type
                cmp     al,2
                jnz     emulate_check
;
                cmp     byte ptr [temp+020],0
                jnz     emulate_check            ; файл упакован UPX ?
;───────────────
                sub     esi,codeloc
                add     esi,ip
                push    esi
                call    real_off    ; * корректируем смещение
                mov     ip,esi      ; * изменим оригинальное положение в памяти
                mov     codeloc,eax ; *         положение относительно антивирусного буфера
                xchg    esi,eax
                jmp     emulate_check
;═══════════════════════════════════════════════ инструкции размером 6 байт
;───────────────────────
; * * * Имитация исполнения "условных" и "безусловных" переходов:
;
; jo, jno, jb, jnb, je, jne, jbe, ja, js, jns, jp, jnp, jl, jnl, jle, jg
;
 @found_jmpd:   mov     al,byte ptr [edi+1]
                sub     al,10h
                call    test_jmpb
                or      eax,eax
                jz      @found_jmp
;═══════════════════════
 emulate_check: call    ca_check_buf
                call    GetTickCount             ; если было привышено макс.
                cmp     eax,dword ptr [temp+028] ; время, отведенное на
                jc      emulate_loop             ; эмуляцию программы, выйдем

 emulate_ret:   mov     dword ptr [emul_stop],esi ; запомним смещение, в котором
                                                  ; остановился эмулятор
                popad
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; Прочитаем 32-х битное число из "виртуального" стека в регистр eax
;
 get_from_vstack        proc
;──────────────────────────────────────────────────────────────────────────────
                push    edi
                mov     esi,dword ptr [regs+016]
                lodsd
                add     dword ptr [regs+016],4
                pop     esi
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; Положим 32-х битное число из регистра eax в "виртуальный" стек
;
 put_to_vstack  proc
;──────────────────────────────────────────────────────────────────────────────
                push    edi
                sub     dword ptr [regs+016],4
                mov     edi,dword ptr [regs+016]
                stosd
                pop     esi
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; reg8_pos - определяет смещение 8-ми битного регистра (номер которого указан
;            в регистре al) относительно таблицы "regs"
;
; on start :  al - номер 8-ми битного регистра
; on exit  : eax - смещение, на значение о регистре относительно таблицы "regs"
;
 reg8_pos       proc
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx
                sub     ecx,ecx
                call    two_regs
                cmp     al,4
                jc      reg8_low
                sub     al,4
                inc     ecx
 reg8_low:      call    mul_4
                add     eax,ecx
                pop     edx ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; test_off - проверка "допустимости" смещения указанного в регистре ebx
;
;  Имитируя исполнение инструкций, которые обращаются по смещениям, есть очень
; большой риск ошибки в расчете "нового" смещения.
; Это может быть следствием:
;
; - ошибки или недоработки анализатора кода, который пропустил процедуру
;   подсчета delta value или любую другую важную особенность кода.
; - неправильной эмуляции участка кода или инструкции.
; - в результате непредвиденной (разработчиками) ошибки в программе.
;
;  В случае если одно или несколько из этих утверждений верно, будет изменен
; код одной из частей антивируса или важные данные, которые используются
; программой для различных целей и "работать" с ними может только антивирус, а
; не никак не исследуемая программа. Результатом может стать зависание
; антивируса.
;
;  Исследуемая программа может пользоваться данными только из "антивирусного
; буфера", который состоит из двух "переменных": "junk" и "buffer". Так же
; может использоваться "виртуальный стек" (переменная "_stack").
;
; on start : ebx    - смещение
; on exit  : cf = 0 - нормальное смещение
;            cf = 1 - недопустимое смещение
;
;
 test_off       proc
;──────────────────────────────────────────────────────────────────────────────
                cmp     ebx,offset junk
                jc      test_off_ret
                cmp     ebx,offset ((_stack + (100000 * 2) )-4)
                ja      test_off_ret
                clc
                ret
 test_off_ret:  stc
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; eax = eax * 4
; первоначальное значение регистров ecx, edx сохраняется
;
 mul_4          proc
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx
                sub     edx,edx
                mov     dl,4
                mul     edx
                pop     edx ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
