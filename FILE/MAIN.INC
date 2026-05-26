.code
;──────────────────────────────────────────────────────────────────────────────
; проверка файла "fname"
;
 process_file   proc    fname: dword
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     byte ptr [cure_result],0
 save_attribs:  push    fname
                call    GetFileAttributesA
                mov     dword ptr [temp+008],eax ; сохраним аттрибуты файла
;
 cleare_attr:   sub     eax,eax
                push    eax fname
                call    SetFileAttributesA       ; уберем все аттрибуты с файла
;
 open_file:     push    0 0 OPEN_EXISTING 0 0
                push    GENERIC_RNW fname
                call    CreateFileA              ; откроем файл для чтения
                cmp     eax,-1                   ; выйдем в случае ошибки
                jnz     read_fname
;───────────────
 error_io:      call    show_fname
                push    offset msg_io_error      ; ошибка открытия файла
 error_io_2:    call    write_string
                push    offset msg_eos
                call    write_string
                jmp     process_ret
;───────────────
 read_fname:    mov     dword ptr [handle],eax   ; сохраним файловый номер
;
                push    offset eh                ; - буфер под заголовок exe
                push    offset ph                ; -       под заголовок pe
                push    eax                      ; - файловый номер
                push    fname                    ; - имя файла
                call    get_type                 ; * определим тип файла
                cmp     al,2
                jnz     close_file               ; проверяем только PE-файлы
;───────────────
                call    show_fname
                inc     dword ptr [scanned]; увеличим счетчик провереных файлов
;───────────────
 get_file_size: push    0 handle
                call    GetFileSize              ; в eax размер файла
                cmp     eax,max_program_size     ; файл слишком велик ?
                jna     rd_whole_file
;
                push    offset msg_large
                call    write_string
                jmp     process_ret
;
 rd_whole_file: mov     dword ptr [temp+024],eax
                push    eax                      ; размер файла для "read_file"

                push    FILE_BEGIN 0 0 handle    ; установим указатель на
                call    SetFilePointer           ; самое начало файла

                push    offset buffer            ; прочитаем файл
                call    read_file                ; полностью в буфер "buffer"
;───────────────
 get_ep_pe:     push    offset ph                ; - буфер под заголовок pe
                push    handle                   ; - файловый номер
                call    get_pe_ep                ; * определим точку входа в PE
                or      eax,eax
                jnz     try_emulate
;───────────────
; точка входа не найдена или расположена в 0 секции (между заголовком
;                                                    PE и первой секцией)
;
                push    offset msg_pe_error
                jmp     error_io_2
;───────────────
 try_emulate:
;
; расчитаем старое смещение точки входа относительно буфера "buffer"
;
                add     eax,offset buffer
                mov     dword ptr [temp+012],eax
;
; расчитаем оригинальное расположение точки входа
; при загрузке программы в память
;
                mov     edx,dword ptr [ph.EntryPoint]
                add     edx,dword ptr [ph.ImageBase]
;
                mov     byte ptr [temp+020],0    ; * проинициализируем "temp+20"
;
; проверим файл на наличие в точке входа сигнатур запаковщиков
;
                push    2                        ; - тип файла (PE)
                call    det_pack                 ; * проверим
;─
; эмулируем файл
;─
 try_emul:      push    edx                      ; - старая точка входа
                push    eax                      ; - точка входа в буфере
                push    0                        ; - режим эмуляции (полный)
                push    2                        ; - тип файла (PE)
                call    emulate                  ; * эмулируем файл
;
                cmp     byte ptr [debug_mode],1  ; включен режим создания дампов?
                jnz     ill_or_healthy           ; нет
                push    fname                    ; - имя файла
                call    debug_dump               ; * создаем дамп
;
ill_or_healthy: mov     eax,dword ptr [ill_mark] ; eax - номер обнаруженного вируса
                or      eax,eax                  ; если eax == 0, значит
                jz      file_healthy             ; файл не заражен вирусом
;═══════════════════════
; в файле был обнаружен вирус (eax - номер найденной вирусной записи)
;
 file_ill:      inc     dword ptr [infected]; * увеличим счетчик больных файлов
                dec     eax                 ; - вирусные записи начинаются с нуля
                push    eax eax
                push    offset msg_inf        ; * выведем на экран инфорацию
                call    write_string          ; * о том, что файл инфицирован
                pop     eax
;
                mov     edx,entry_size        ; * расчитаем расположение
                mul     edx                   ; * обнаруженной вирусной
                add     eax,offset @001_type+1; * записи (в базе)
                push    eax
                call    write_string          ; * выведем на экран называние вируса
;
                call    cure_vir              ; * вызовем процедуру лечения
                jmp     show_eos
;═══════════════════════
; файл здоров
;
 file_healthy:  push    offset msg_healthy
 show_nfo:      call    write_string          ; * выведем на экран результат проверки
 show_eos:      push    offset msg_eos
                call    write_string
 close_file:    push    handle
                call    CloseHandle
                cmp     1 ptr [cure_result],1 ; * файл должен быть удален ?
                jnz     process_ret
                push    fname                 ; - название файла
                call    DeleteFileA           ; * удалим файл
                inc     dword ptr [deleted]
 process_ret:   push    [temp+008] fname
                call    SetFileAttributesA    ; восстановим аттрибуты файла
                popad
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; вывод отчета о проверке
;
 report_msg     proc
;──────────────────────────────────────────────────────────────────────────────
                push    offset msg_scanned
                call    write_string
                push    scanned
                call    show_hex
                push    offset msg_eos
                call    write_string
;
                push    offset msg_infected
                call    write_string
                push    infected
                call    show_hex
                push    offset msg_eos
                call    write_string
;
                push    offset msg_cleaned
                call    write_string
                push    cleaned
                call    show_hex
                push    offset msg_eos
                call    write_string
;
                push    offset msg_deleted
                call    write_string
                push    deleted
                call    show_hex
                push    offset msg_eos
                call    write_string
;
                push    offset msg_modification
                call    write_string
                push    modification
                call    show_hex
                push    offset msg_eos
                call    write_string
                push    offset msg_eos
                call    write_string
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; вывод на экран имяни файла
;
 show_fname     proc
;──────────────────────────────────────────────────────────────────────────────
                push    esi
                mov     esi,fname                ; ошибка открытия файла
                cmp     dword ptr [temp+004],4
                jc      show_fname_dsp
                add     esi,dword ptr [temp+004]
 show_fname_dsp:push    esi
                call    write_string
                push    offset msg_first
                call    write_string
                pop     esi
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────