;═════════════════════════════════════════════════════════════════════════════
; project : Explosion Antivirus version 010
; author  : beauty on the fire [Most Needful Things]
; home    : http://explosion.needful-things.ru
;
;  Последующие версии программы на http://explosion.needful-things.ru
;
; компилировать: tasm32 -ml -m5 xpl.asm
;                tlink32 -Tpe -c -x xpl.obj ,,, import32 ,, icon.res
;
.386P
.model flat, stdcall
 jumps
;
 include                data\extrn.inc          
;───────────────────────
; процедуры анализатора кода (code analyzer / ca)
;
 include                det\detect.inc       ; проверка на совпадение
 include                det\ca_init.inc      ; инициализация "ca"
 include                det\ca_ip.inc        ; поиск процедур подсчета delta value
 include                det\ca_pack.inc      ; поиск сигнатур упаковщиков
;───────────────────────
; лечение файлов, инфицированных известными вирусами
;
 include                det\cure\main.inc    ; главный модуль лечения
 include                det\cure\parite.inc  ; * cure: w32/parite
 include                det\cure\krized.inc  ;         w32/krized
 include                det\cure\funlove.inc ;         w32/funlove
 include                det\cure\marburg.inc ;         w95/marburg
;───────────────────────
; процедуры для работы с файлами
;
 include                file\debug.inc          ; отладочные процедуры
 include                file\main.inc           ; проверка файла
 include                file\report.inc         ; работа с файлом отчета
 include                file\pe\get_ep.inc      ; определение точки входа
 include                file\pe\get_type.inc    ; определение типа файла
 include                file\pe\struc.inc       ; структуры файлов
 include                file\pe\real_off.inc    ; перевод смещений (в PE-файлах)
;───────────────────────
; вспомогательные функции
;
 include                include\console.inc     ; функции для работы с консолью
 include                include\library.inc
;──────────────────────
.data
 msg_first             db ' - ',0
 msg_next              db ', ',0
 msg_dots              db ' ... ',0
 msg_large             db 'too big',0
 msg_io_error          db 'input/output error!',0
 msg_packed            db 'packed with ',0
 msg_pe_error          db 'error in PE-file!',0
 msg_inf               db 'infected with ',0
 msg_healthy           db 'healthy',0
 msg_cured             db 'cured',0
 msg_incurable         db 'incurable (was deleted)!',0
 msg_disinf_er         db 'not cured!',0
 msg_info              db 'Explosion Antivirus 010 (c) 2004 Most Needful Things',13,10
                       db '                        (c) 2004 beauty on the fire',13,10
                       db 'next versions at - http://needful-things.ru',13,10
 msg_eos               db 13,10,0
 msg_params            db ' use: explosion.exe [switch] [path]',13,10
                       db ' switch: /c  - cure infected files',13,10
                       db '         /ra - append to exist report',13,10
                       db '         /rc - create new report',13,10
                       db ' path  : /*  - scan all fixed drives',13,10,0
 msg_error_path        db 'does not exist!',0
 msg_scanned           db 13,10
                       db ' Scanned  : ',0
 msg_infected          db ' Infected : ',0
 msg_cleaned           db ' Cleaned  : ',0
 msg_deleted           db ' Deleted  : ',0
 msg_modification      db ' Possibly : ',0
;
 report_fname          db 'xplosion.rep',0
;──────────────────────
 include               data\av.inc
 include               data\files.inc
 include               data\console.inc
 include               data\ca_emul.inc
;──────────────────────────────────────────────────────────────────────────────
.code
 start:         call    init_console             ; инициализируем консоль
;───────────────
                mov     byte ptr [fixed_drives],0
                call    get_command              ; разберем параметры командной строки
                or      eax,eax                  ; если они были указанны
                jnz     test_path                ; правильно, продолжим
;
                push    offset msg_params        ; параметры были указаны
                call    write_string             ; неправильно
                jmp     exit
;═══════════════
 test_path:     push    eax                      ;-!
                push    offset file_path
                push    260                      ; сохраним текущий каталог
                call    GetCurrentDirectoryA     ; в "file_path"
;
                push    offset find_buf+1        ; меняем текущий каталог на
                call    SetCurrentDirectoryA     ; указанный в командной строке
                or      eax,eax                  ; если ошибки не произошло
                jnz     prepare_path             ; продолжим работу
;───────────────
                pop     eax                      ;-!
                push    offset find_buf+1
                call    write_string
                push    offset msg_first
                call    write_string             ; выведем на экран информацию
                push    offset msg_error_path    ; о том, что указанного
                call    write_string             ; каталога несуществует
                jmp     exit
;═══════════════
 prepare_path:  push    offset file_path         ; * установим "первоначальный"
                call    SetCurrentDirectoryA     ; * каталог
;
 show_av_nfo:   push    offset report_fname      ; * создаем файл отчета если
                call    init_rep                 ; * указан параметр "/r(a/c)"
                push    offset msg_info
                call    write_string             ; выведем информацию о антивирусе
;
                pop     eax                      ; длина каталога для проверки
                mov     dword ptr [temp+004],eax
                cmp     byte ptr [find_buf+1+eax-1],'\'
                jnz     prepare_search
                dec     dword ptr [temp+004]
;───────────────
 prepare_search:sub     ecx,ecx
                mov     cl,byte ptr [fixed_drives]
                sub     edx,edx
                mov     dl,1
 search_files:  dec     ecx
                inc     edx
                pushad
                push    offset find_buf
                call    find                     ; рекурсивный обходищем файлы
                call    report_msg
                popad
                or      ecx,ecx
                jz      exit
                js      exit
                mov     al,byte ptr [fixed_drives+edx]
                add     al,'a'
                mov     byte ptr [drive_path],al
                mov     eax,dword ptr [drive_path]
                mov     dword ptr [find_buf+1],eax
                jmp     search_files
;
 exit:          push    rep_handle
                call    CloseHandle
                push    0
                call    ExitProcess
;──────────────────────────────────────────────────────────────────────────────
; find - поиск файлов с использованием памяти переменной "soff"
;
 find           proc    soff: dword
;──────────────────────────────────────────────────────────────────────────────
                mov     ebx,soff                 ; ebx - область данных
                mov     byte ptr [ebx],0         ; сначала ищем все файлы
                mov     edi,ebx                  ; edi - текущий каталог - 1
                inc     edi
                push    edi
                call    lstrlenA                 ; eax - длина пути
                add     edi,eax
                cmp     byte ptr [edi-1],'*'     ; если маска содержится в
                jz      first_find               ; пути, все в порядке
 check_slash:   mov     esi,offset mask          ; esi - маска
                cmp     byte ptr [edi-1],'\'
                jnz     nneed_slash              ; '\' уже есть в конце пути
                inc     esi
 nneed_slash:   push    esi
                call    lstrlenA                 ; eax - длина маски
                inc     eax                      ; прибавим null
                xchg    eax,ecx
                rep     movsb                    ; дополним путь маской
;────────────────
 first_find:    mov     edx,ebx                  ; edx - буффер
                add     edx,1+260+4              ; edx = edx + 1 + 260 + 4
                push    edx               ; получим смещение для структуры поиска
                mov     edx,ebx
                inc     edx
                push    edx
                call    FindFirstFileA
                cmp     eax,-1
                jnz     save_fnd_hndl
 test_fod:      cmp     byte ptr [ebx],1         ; если каталоги уже искали
                jz      end_find                 ; просто выйдем
 set_filesfind: inc     byte ptr [ebx]           ; иначе ищем каталоги
                jmp     first_find
 end_find:      push    dword ptr [ebx+1+260]
                call    FindClose                ; прекратим поиск
                ret
;───────────────
 save_fnd_hndl: mov     dword ptr [ebx+1+260],eax
 lf:            cmp     byte ptr [ebx+1+260+4+44],'.'
                je      find_next                ; пропускаем ".." и "."
 test_if_dir:   test    byte ptr [ebx+1+260+4],10h
                jz      found_file               ; нашли просто файл
 proc_dir:      cmp     byte ptr [ebx],1         ; если ищем файлы, то каталоги
                jnz     find_next                ; не трогаем
;───────────────
 prepare_rec:   mov     edi,ebx
                add     edi,(ENTRY_SIZE)+1 ; не забудем про флаг который идет перед "маской" поиска
                mov     esi,ebx
                inc     esi
                push    esi
                call    lstrlenA
                xchg    eax,ecx
                sub     ecx,4
                cmp     byte ptr [esi+ecx],'\'
                jnz     not_root
                inc     ecx
 not_root:      rep     movsb
;─ ─ ─ ─ ─ ─ ─ ─
                mov     esi,ebx
                add     esi,1+260+4+44
                push    esi
                call    lstrlenA
                inc     eax
                xchg    eax,ecx
                rep     movsb
                add     ebx,(ENTRY_SIZE)
                push    ebx
                call    find
                sub     ebx,(ENTRY_SIZE)
                jmp     find_next
;
 found_file:    cmp     byte ptr [ebx],1         ; если ищем файлы, то каталоги
                jz      find_next                ; не трогаем
;───────────────
                push    edi esi ebx ebx
                inc     ebx
                xchg    esi,ebx
                push    esi
                call    lstrlenA
                xchg    eax,ecx
                sub     cl,3
                mov     edi,offset file_path
                rep     movsb
                pop     esi
                add     esi,1+260+4+44
                push    esi
                call    lstrlenA
                xchg    eax,ecx
                rep     movsb
                sub     eax,eax
                stosb
                pop     ebx esi edi
_process_file:  push    offset file_path         ; - полный путь к файлу
                call    process_file             ; * проверка файла
                cmp     byte ptr [cure_result],2 ; * если файл был вылечен,
                jz     _process_file             ;   проверим еще раз
;
 find_next:     mov     edx,ebx
                add     edx,1+260+4
                push    edx
                push    dword ptr [ebx+1+260]
                call    FindNextFileA
                cmp     eax,0
                jnz     lf
                jmp     test_fod
 endp
;──────────────────────────────────────────────────────────────────────────────
 include                emul\emulator.inc     ; эмуляция программного кода
 include                emul\disasm.inc       ; дизассемблер инструкций
;──────────────────────────────────────────────────────────────────────────────
                end    start