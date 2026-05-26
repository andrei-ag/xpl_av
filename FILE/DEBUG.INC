;──────────────────────────────────────────────────────────────────────────────
; создание дампов (key "/d")
;
 proc debug_dump        fname
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     esi,[fname]                ; esi - имя эмулируемого файла
                push    esi
                invoke  lstrlenA                   ; file name length in eax
;
                mov     dword [temp+016],eax
                mov     dword [esi+eax],'.dmp'     ; add '.dmp' to file name
                mov     byte [esi+eax+4],0

                push    0
                push    0
                push    CREATE_ALWAYS
                push    0
                push    0
                push    GENERIC_WRITE
                push    esi                        ; file name
                invoke  CreateFileA
                cmp     eax,-1
                jz      deb_dump_ret
;
                push    eax                        ;* запомним файловый номер
                push    0
                push    number
                push    dword [temp+024]           ; file size
                push    dword [file_map_address]   ; file location
                push    eax                        ; файловый номер
                invoke  WriteFile                  ; запишем дамп в файл

                invoke  CloseHandle                ; закроем файл

 deb_dump_ret:  mov     esi,[fname]
                mov     eax,dword [temp+016]
                mov     byte [esi+eax],0           ; востановим имя файла в
                                                  ; исходное состояние
                popad
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; вывод на экран базы данных кодо-анализатора по циклам
;
 proc debug_ca_base
;──────────────────────────────────────────────────────────────────────────────
                cmp     byte [debug_show_loops],0
                jnz     debug_ca_base_start
                ret
;
 debug_ca_base_start:
		push    eax edx ecx esi
                sub     edx,edx
                mov     esi,dword [ca_base_pointer]
                lodsd
                or      eax,eax
                jz      debug_ca_base_ret
                xchg    eax,ecx
 debug_ca_base_loop:
                cmp     dword [esi+8],this_is_loop
                jge     debug_ca_base_show
;
; Не циклический участок кода (малоповторений)
;
                add     esi,( 4 * 3 )
                jmp     debug_ca_base_next
;
 debug_ca_base_show:
                push    msg_ca_from
                call    write_string
                lodsd
                sub     eax,dword [file_map_address]
                add     eax,dword [image_base_of_pe]
                push    eax
                call    show_dword_in_hex
;
                push    msg_ca_to
                call    write_string
                lodsd
                sub     eax,dword [file_map_address]
                add     eax,dword [image_base_of_pe]
                push    eax
                call    show_dword_in_hex
;
                push    msg_ca_times
                call    write_string
                lodsd
                push    eax
                call    show_hex

                inc     edx
;
 debug_ca_base_next:
                loop    debug_ca_base_loop
;
                or      edx,edx
                jz      debug_ca_base_ret
                push    msg_eos
                call    write_string
                push    msg_eos
                call    write_string
;
 debug_ca_base_ret:
                pop     esi edx ecx eax
                ret  
 msg_ca_from            db 13,10,'from: ',0
 msg_ca_to              db '; to ',0
 msg_ca_times           db '; times: ',0
;
 endp