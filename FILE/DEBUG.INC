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