;═════════════════════════════════════════════════════════════════════════════
; project : Explosion Antivirus
; home    : http://explosion.neeful-things.ru
;
; Distributed under GNU General Public License
;
; Compilation string: "fasm.exe xpl.asm > error"
;
; This software is developed on "flat assembler  version 1.67.29"
;

format PE console 4.0
entry start

include 'FASM_INC\win32a.inc'

;──────────────────────────────────────────────────────────────────────────────
 section '.data' data readable writeable executable

;DEBUG                equ 0 
;
 if defined DEBUG
  test_parameters       db 'XPL.EXE /t- c:\av\1\',0
 end if

 msg_memory_error      db 'HeapCreate failed, need more memory!',13,10,0
 msg_windmill          db '/',8,'-',8,'\',8,'|',8
 msg_first             db ' - ',0
 msg_next              db ', ',0
 msg_dots              db ' ... ',0
 msg_large             db 'too big',13,10,0
 msg_io_error          db 'input/output error!',0
 msg_packed            db 'packed with ',0
 msg_pe_error          db 'error in PE-file!',0
 msg_inf               db 'infected with ',0
 msg_healthy           db 'healthy',0
 msg_cured             db 'cured',0
 msg_incurable         db 'incurable (was deleted)!',0
 msg_disinf_er         db 'not cured!',0
 msg_info              db 'Explosion Antivirus 012X (c) Most Needful Things',13,10
                       db 'next versions at - http://explosion.needful-things.ru',13,10
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
 include               "det\virus_base.inc"      ; База вирусов
 include               "det\unpack_base.inc"     ; База упаковщиков
;
 include               "data\av.inc"
 include               "file\pe\struc.inc"       ; структуры PE
 include               "data\files.inc"
 include               "emul\apis\emul_api_db.inc"
 include               "data\console.inc"
 include               "data\ca_emul.inc"

;──────────────────────────────────────────────────────────────────────────────
 section '.text' code data readable writeable executable

;───────────────────────
; процедуры анализатора кода (code analyzer / ca)
;
 include                "det\detect.inc"       ; проверка на совпадение
 include                "det\ca_pack.inc"      ; поиск сигнатур упаковщиков
 include                "det\ca_loop.inc"
; include                "det\ca_ip.inc"        ; поиск процедур подсчета delta value
; include                "det\ca_init.inc"      ; инициализация "ca"
;          
;───────────────────────
; лечение файлов, инфицированных известными вирусами
;
 include                "det\cure\main.inc"       ; главный модуль лечения
; include                det\cure\parite.inc     ; * cure: w32/parite
; include                det\cure\krized.inc     ;         w32/krized
; include                det\cure\thorin.inc     ;         w32/thorin
; include                det\cure\funlove.inc    ;         w32/funlove
; include                det\cure\marburg.inc    ;         w95/marburg
;
;───────────────────────
; процедуры для работы с файлами
;          
 include                "file\debug.inc"          ; создание дампов
 include                "file\main.inc"           ; проверка файла
 include                "file\report.inc"         ; работа с файлом отчета
 include                "file\pe\get_pe_apis.inc" ; Get PE's imported apis
 include                "file\pe\load_pe.inc"     ; Загрузка PE в антивирусный буффер
 include                "file\pe\get_ep.inc"      ; определение точки входа
 include                "file\pe\get_type.inc"    ; определение типа файла
;───────────────────────
; вспомогательные функции
;
 include                'include\console.inc'     ; функции для работы с консолью
 include                'include\library.inc'
 include                'include\cmd.inc'         ; command line
;
 start:         pushfd                           ; Сохраним первоначально
                pop     dword [flags_basic]      ; состояние флагов программы
                                                 ; (понадобится эмулятору)
;
                call    init_console             ; инициализируем консоль
;
                mov     dword [file_map_address],0
;
                push    0
                push    max_program_size+(virtual_stack_size*2)
                push    0
                invoke  HeapCreate               ; Резервируем блок памяти
;
                or      eax,eax
                jnz     process_command_line
;
                push    msg_memory_error         ; Недостаточно памяти
                call    write_string
                jmp     end_of_xpl
;───────────────
 process_command_line:
                mov     dword [heap_handle],eax

                mov     byte [fixed_drives],0
                call    get_command              ; разберем параметры командной строки
;
                or      eax,eax                  ; если они были указанны
                jnz     test_path                ; правильно, продолжим
;
                push    msg_info                 
                call    write_string             
                push    msg_params               ; параметры были указаны
                call    write_string             ; неправильно
                jmp     exit
;═══════════════
 test_path:     mov     dword [start_directory_ptr],eax
;                      
                push    file_path
                push    260                      ; сохраним текущий
                invoke  GetCurrentDirectoryA     ; каталог в "file_path"
;             
                push    dword [start_directory_ptr]
                invoke  SetCurrentDirectoryA     ; меняем текущий каталог
                                                 ; на указанный в командной строке
;
                or      eax,eax                  ; если ошибки не произошло
                jnz     prepare_path             ; продолжим работу
;───────────────
                push    dword [start_directory_ptr]
                call    write_string
                push    msg_first
                call    write_string             ; выведем на экран информацию
                push    msg_error_path           ; о том, что указанного
                call    write_string             ; каталога несуществует
                jmp     exit
;═══════════════
 prepare_path:  
                push    file_path
                invoke  SetCurrentDirectoryA     ; установим "первоначальный"
                                                 ; каталог
;
 show_av_nfo:   push    report_fname             ; * создаем файл отчета если
                call    init_rep                 ; * указан параметр "/r(a/c)"
                push    msg_info
                call    write_string             ; выведем информацию о
;                                                ; антивирусе
                push    find_buf+1
                invoke  lstrlenA
;
                mov     dword [temp+004],eax
                cmp     byte [find_buf+1+eax-1],'\'
                jnz     prepare_search
;
                cmp     byte [find_buf+1+eax-1],0
                dec     dword [temp+004]
;───────────────
 prepare_search:
                pushad
                lea     eax,[thread_windmill] 
		push	0 0 0
		push 	eax
		push 	0 0
		invoke 	CreateThread
		mov     dword [thread_handle],eax
                popad
;
                sub     ecx,ecx
                mov     cl,byte [fixed_drives]
                sub     edx,edx
                mov     dl,1
 search_files:  dec     ecx
                inc     edx
;
                pushad
                push    find_buf
                call    find                     ; рекурсивный обход
                call    report_msg               ; (ищем файлы)
                popad
;
                or      ecx,ecx
                jz      exit
                js      exit
                mov     al,byte [fixed_drives+edx]
                add     al,'a'
                mov     byte [drive_path],al
                mov     eax,dword [drive_path]
                mov     dword [find_buf+1],eax
                jmp     search_files
;
 exit:          push    rep_handle
                invoke  CloseHandle
;
; Освободим память
;
                push    [_argv]
                push    0
                push    [_hheap]
                invoke  HeapFree
;
                push    [_strbuf]
                push    0
                push    [_hheap]
                invoke  HeapFree
;                
                push    [heap_handle]
                invoke  HeapDestroy
;
                push    0
 		push    dword [thread_handle]
                invoke  TerminateThread
;
 end_of_xpl:    push    0
                invoke  ExitProcess
;──────────────────────────────────────────────────────────────────────────────
; find - поиск файлов с использованием памяти переменной "soff"
;
 proc find      soff
;──────────────────────────────────────────────────────────────────────────────
                mov     ebx,[soff]               ; ebx - область данных
                mov     byte [ebx],0             ; сначала ищем все файлы
                mov     edi,ebx                  ; edi - текущий каталог - 1
                inc     edi
;
                push    edi
 		invoke  lstrlenA                 ; eax - длина пути
;
                add     edi,eax
                cmp     byte [edi-1],'*'         ; если маска содержится в
                jz      first_find               ; пути, все в порядке
 check_slash:   lea     esi,[mask]               ; esi - маска
                cmp     byte [edi-1],'\'
                jnz     nneed_slash              ; '\' уже есть в конце пути
                inc     esi
 nneed_slash:   
;
                push    esi
 		invoke  lstrlenA                 ; eax - длина маски
;
                inc     eax                      ; прибавим null
                xchg    eax,ecx
                rep     movsb                    ; дополним путь маской
;────────────────
 first_find:    mov     edx,ebx                  ; edx - буффер
                add     edx,1+260+4              ; edx = edx + 1 + 260 + 4
                push    edx                      ; получим смещение для
                mov     edx,ebx                  ; структуры поиска
                inc     edx
;
                push    edx
 		invoke  FindFirstFileA
;
                cmp     eax,-1
                jnz     save_fnd_hndl
 test_fod:      cmp     byte [ebx],1             ; если каталоги уже искали
                jz      end_find                 ; просто выйдем
 set_filesfind: inc     byte [ebx]               ; иначе ищем каталоги
                jmp     first_find
 end_find:      
 		push    dword [ebx+1+260]        ; прекратим поиск
		invoke  FindClose
                ret
;───────────────
 save_fnd_hndl: mov     dword [ebx+1+260],eax
 lf:            cmp     byte [ebx+1+260+4+44],'.'
                je      find_next                ; пропускаем ".." и "."
 test_if_dir:   test    byte [ebx+1+260+4],10h
                jz      found_file               ; нашли просто файл
 proc_dir:      cmp     byte [ebx],1             ; если ищем файлы, то каталоги
                jnz     find_next                ; не трогаем
;───────────────
 prepare_rec:   mov     edi,ebx
                add     edi,(ENTRY_SIZE)+1       ; не забудем про флаг который 
                mov     esi,ebx                  ; идет перед "маской" поиска
                inc     esi
;
		push    esi
 		invoke  lstrlenA
;
                xchg    eax,ecx
                sub     ecx,4
                cmp     byte [esi+ecx],'\'
                jnz     not_root
                inc     ecx
 not_root:      rep     movsb
;─ ─ ─ ─ ─ ─ ─ ─
                mov     esi,ebx
                add     esi,1+260+4+44
;
                push    esi
		invoke  lstrlenA 
;
                inc     eax
                xchg    eax,ecx
                rep     movsb
                add     ebx,(ENTRY_SIZE)
                push    ebx
                call    find
                sub     ebx,(ENTRY_SIZE)
                jmp     find_next
;
 found_file:    cmp     byte [ebx],1             ; если ищем файлы, то каталоги
                jz      find_next                ; не трогаем
;───────────────
                push    edi esi ebx ebx
                inc     ebx
                xchg    esi,ebx
;
                push    esi
 		invoke  lstrlenA
;
                xchg    eax,ecx
                sub     cl,3
                lea     edi,[file_path]
                rep     movsb
                pop     esi
                add     esi,1+260+4+44
;
		push    esi
 		invoke  lstrlenA
;
                xchg    eax,ecx
                rep     movsb
                sub     eax,eax
                stosb
                pop     ebx esi edi
 _process_file: push    file_path                ; - полный путь к файлу
                call    process_file             ; * проверка файла
                cmp     byte [cure_result],2     ; * если файл был вылечен,
                jz      _process_file            ;   проверим еще раз
;
 find_next:     mov     edx,ebx
                add     edx,1+260+4
                push    edx
;
                push    dword [ebx+1+260]
 		invoke  FindNextFileA 
;
                cmp     eax,0
                jnz     lf
                jmp     test_fod
;
 endp
;──────────────────────────────────────────────────────────────────────────────
 include                "emul\emulator.inc"      ; Эмулятор (основной модуль)
 include                "disasm\disasm.inc"      ; Дизассемблер инструкций (основной модуль)
;
 include                "sections\idata.inc"     ; import data
 include                "sections\rsrc.inc"      ; resource data
;──────────────────────────────────────────────────────────────────────────────