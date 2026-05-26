.code
;──────────────────────────────────────────────────────────────────────────────
; создание дампов проэмулированного файла (key "/d")
;
 debug_dump     proc    fname: dword
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     esi,fname                  ; esi - имя эмулируемого файла
                push    esi
                call    lstrlenA                   ; получим длину имяни в eax
                mov     dword ptr [temp+016],eax
                mov     dword ptr [esi+eax],'pmd.' ; добавим к имяни '.dmp'
                push    0 0
                push    CREATE_ALWAYS
                push    0 0
                push    GENERIC_WRITE
                push    fname
                call    CreateFileA
                cmp     eax,-1
                jz      deb_dump_ret
;
                push    eax           ;* запомним файловый номер
                push    0
                push    offset number
                push    4000h         ; размер дампа - 4000h байт
                                      ; писать с
                push    [emul_stop]   ; позиции, в которой остановился эмулятор
;               push    [temp+012]    ; точки входа в файл
                push    eax           ; файловый номер
                call    WriteFile     ; запишем дамп в файл

                call    CloseHandle   ; закроем файл (файловый номер уже в стеке(

 deb_dump_ret:  mov     esi,fname
                mov     eax,dword ptr [temp+016]
                mov     byte ptr [esi+eax],0     ; востановим имя файла в
                                                 ; исходное состояние
                popad
                ret
 endp
