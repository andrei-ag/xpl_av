;──────────────────────────────────────────────────────────────────────────────
; проверка файла "fname"
;
 proc process_file      fname
 local .thread_handle:DWORD
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     byte [cure_result],0
 save_attribs:  push    [fname]
                invoke  GetFileAttributesA
                mov     dword [temp+008],eax     ; сохраним аттрибуты файла
;
 cleare_attr:   sub     eax,eax
                push    eax
                push    [fname]
                invoke  SetFileAttributesA       ; уберем все аттрибуты с файла
;
 open_file:     push    0
                push    0
                push    OPEN_EXISTING
                push    0
                push    0
                push    GENERIC_RNW
                push    [fname]
                invoke  CreateFileA              ; откроем файл для чтения
                cmp     eax,-1                   ; выйдем в случае ошибки
                jnz     read_fname
;───────────────
 error_io:      push    [fname]
                call    show_fname
;
                push    msg_io_error             ; ошибка открытия файла
 error_io_2:    call    write_string
                push    msg_eos
                call    write_string
                jmp     process_ret
;───────────────
 read_fname:    mov     dword [handle],eax       ; сохраним файловый номер
;
 get_file_size: push    0
                push    [handle]
                invoke  GetFileSize              ; EAX - Физический размер файла
;
                mov     dword [temp+024],eax
;
                push    header_temp_buffer       ; - для заголовков (EXE, PE)
                push    [handle]                 ; - файловый номер
                push    [fname]                  ; - имя файла
                call    get_type                 ; * определим тип файла
;
                cmp     al,2
                jnz     close_file               ; проверяем только PE-файлы
;───────────────
                push    [fname]
                call    show_fname

                inc     dword [scanned]          ; увеличим счетчик провереных файлов
;
                mov     eax,dword [temp+024]     ; физический размер файла
                cmp     eax,max_program_size     
                ja      very_big_pe              ; файл слишком велик
;───────────────
                mov     eax,dword [header_temp_buffer+050h] 
                                                 ; размер файла в памяти
                cmp     eax,max_program_size     
                jbe     rd_whole_file            ; файл слишком велик
;
 very_big_pe:   push    msg_large        
                call    write_string
                jmp     process_ret
;
 rd_whole_file: cmp     eax,dword [temp+024]
                jbe     load_pe_in_memory
                mov     dword [temp+024],eax
;
 load_pe_in_memory:
                call    load_pe                  ; Загрузим PE в память
                jc      close_file_handle
;───────────────
                call    get_pe_ep                ; * определим точку входа в PE
                or      eax,eax
                jnz     try_emulate
;───────────────
; точка входа не найдена или расположена в 0 секции (между заголовком
;                                                    PE и первой секцией)
;
                push    msg_pe_error
                jmp     error_io_2
;───────────────
 try_emulate:
;               pushad
;               lea     eax,[thread_windmill] 
;		push	0 0 0
;		push 	eax
;		push 	0 0
;		invoke 	CreateThread
;		mov     dword [.thread_handle],eax
;               popad

;
; Get list of APIs imported by current PE
;
                mov     ebx,dword [file_map_address]
                add     ebx,dword [ph_loc]
                push    ebx                   ; Pointer to PE header
                call    get_imported_apis
;
                mov     byte [temp+020],0        ; * проинициализируем "temp+20"
;
; проверим файл на наличие в точке входа сигнатур запаковщиков
;
                push    2                        ; - тип файла (PE)
                call    det_pack                 ; * проверим
;─
; эмулируем файл
;─
 try_emul:
                push    eax                      ; - точка входа в буфере
                push    0                        ; - режим эмуляции (полный)
                push    2                        ; - тип файла (PE)
                call    emulate                  ; -> * эмулируем файл
;
;               push    0
;		push    dword [.thread_handle]
;               invoke  TerminateThread
;
                cmp     byte [debug_mode],1      ; включен режим создания дампов?
                jnz     ill_or_healthy           ; -> нет
;
                push    [fname]                  ; - имя файла
                call    debug_dump               ; * создаем дамп
;
 ill_or_healthy:
                mov     eax,dword [ill_mark]     ; eax - номер обнаруженного вируса
                or      eax,eax                  ; если eax == 0, значит
                jz      file_healthy             ; файл не заражен вирусом
;═══════════════════════
; в файле был обнаружен вирус (eax - номер найденной вирусной записи)
;
 file_ill:      inc     dword [infected]    ; * увеличим счетчик больных файлов
                dec     eax                 ; - вирусные записи начинаются с нуля
                push    eax
;
                push    eax
                push    msg_inf               ; * выведем на экран инфорацию
                call    write_string          ; * о том, что файл инфицирован
;
                pop     eax
;
                mov     edx,entry_size        ; * расчитаем расположение
                mul     edx                   ; * обнаруженной вирусной
                add     eax,@001_type+1       ; * записи (в базе)
                push    eax
                call    write_string          ; * выведем на экран называние вируса
;
                call    cure_vir              ; * вызовем процедуру лечения
                jmp     show_eos
;═══════════════════════
; файл здоров
;
 file_healthy:  push    msg_healthy
 show_nfo:      call    write_string          ; * выведем на экран результат проверки
 show_eos:      push    msg_eos
                call    write_string
;
		call    debug_ca_base         ; Отладочная информация о числе циклов
;
 close_file:    cmp     dword [file_map_address],0
                jz      close_file_handle
;
                push    [file_map_address]
                push    0                     ; Освободим блок памяти выделенный
                push    [heap_handle]         ; под разбираемый файл
                invoke  HeapFree
                mov     dword [file_map_address],0               
;
 close_file_handle:
                push    [handle]
                invoke  CloseHandle
                cmp     byte [cure_result],1  ; * файл должен быть удален ?
                jnz     process_ret
                push    [fname]               ; - название файла
                invoke  DeleteFileA           ; * удалим файл
                inc     dword [deleted]
 process_ret:   push    [temp+008]
                push    [fname]
                invoke  SetFileAttributesA    ; восстановим аттрибуты файла
                popad
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────