.code
;──────────────────────────────────────────────────────────────────────────────
; get_type - определяет тип открытого файла
;
; on start :     _eh - буфер под даные о заголовке (не менее 40 байт)
;                _ph - буфер под даные о заголовке PE (не менее 52 байт)
;              fhndl - номер открытого файла
;              fname - путь и имя открытого файла (ASCII)
; on exit  :     eax - тип файла: 0 - com (dos)
;                                 1 - exe (dos)
;                                 2 - PE  (win32)
;                eax = 10 - неизвестный тип файла
;
 get_type       proc   fname: dword, fhndl:dword, _ph:dword, _eh:dword
;──────────────────────────────────────────────────────────────────────────────
 read_exe_hdr:  sub     eax,eax
                push    edi esi ebp ebx ecx eax
                mov     ebx,offset _eh
                mov     esi,offset _ph
                push    03FH ebx                 ; прочитаем заголовок
                call    read_file                ; exe-файла
;
                cmp     word ptr [ebx],'ZM'
                jnz     test_dos_com             ; проверим, может это COM-файл
;
 chk_reloc_pos: cmp     byte ptr [ebx+18h],40h   ; проверим позицию первого
                jc      its_dos_exe              ; элемента Relocation Table
;
                sub     edx,edx
                mov     dx,word ptr [ebx+03Ch]
                mov     dword ptr [ph_loc],edx   ; запомним смещение заголовка
                push    FILE_BEGIN 0 edx fhndl   ; установим указатель на
                call    SetFilePointer           ; начало заголовка PE
;
 read_pe_hdr:   push    ph_size esi              ; прочитаем часть
                call    read_file                ; (92 байта из) заголовка PE
                cmp     word ptr [esi],'EP'      ; проверим, может это не PE-файл
                jnz     its_dos_exe
;───────────────
 its_pe:        pop     eax
                mov     al,2
                jmp     get_type_ret+1
;───────────────
 its_dos_exe:   pop     eax
                inc     eax
                jmp     get_type_ret+1
;───────────────
 test_dos_com:  mov     esi,fname
                push    esi
                call    lstrlenA
                cmp     dword ptr [esi+eax-4],'MOC.'
                jz      get_type_ret
                cmp     dword ptr [esi+eax-4],'moc.'
                jz      get_type_ret
;
                pop     eax
                mov     al,10
                push    eax
;═══════════════
 get_type_ret:  pop     eax ecx ebx ebp esi edi
                ret
 endp