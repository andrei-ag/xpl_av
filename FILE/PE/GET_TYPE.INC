;──────────────────────────────────────────────────────────────────────────────
; get_type - определяет тип открытого файла
;
; on start :     buf - буфер под даные о заголовках файла
;              fhndl - номер открытого файла
;              fname - путь и имя открытого файла (ASCII)
; on exit  :     eax - тип файла: 0 - com (dos)
;                                 1 - exe (dos)
;                                 2 - PE  (win32)
;                eax = 10 - неизвестный тип файла
;                buf = заголовок файла в зависимости от его типа
;
 proc get_type          fname, fhndl, buf
;──────────────────────────────────────────────────────────────────────────────
 read_exe_hdr:  sub     eax,eax
                push    edi
                push    esi
                push    ebp
                push    ebx
                push    ecx
                push    eax
;
                mov     ebx,[buf]
;
                push    040h
                push    ebx                      ; прочитаем заголовок
                call    read_file                ; exe-файла
;
                cmp     word [ebx],'MZ'
                jnz     test_dos_com             ; проверим, может это COM-файл
;
 chk_reloc_pos: cmp     byte [ebx+18h],40h       ; проверим позицию первого
                jc      its_dos_exe              ; элемента Relocation Table
;
                sub     edx,edx
                mov     dx,word [ebx+03Ch]
                cmp     edx,dword [temp+024]    ; Сравним с размером файла
                ja      its_dos_exe              
;
                mov     dword [ph_loc],edx       ; запомним смещение заголовка
;
                push    FILE_BEGIN
                push    0
                push    edx
                push    [fhndl]                  ; установим указатель на
                invoke  SetFilePointer           ; начало заголовка PE
;
 read_pe_hdr:   push    ph_size
                push    ebx                      ; прочитаем часть 
                call    read_file                ; PE-заголовка
;
                cmp     word [ebx],'PE'          ; проверим, может это не PE-файл
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
 test_dos_com:  mov     esi,[fname]
                push    esi
                invoke  lstrlenA
                cmp     dword [esi+eax-4],'.COM'
                jz      get_type_ret
                cmp     dword [esi+eax-4],'.com'
                jz      get_type_ret
;
                pop     eax
                mov     al,10
                push    eax
;═══════════════
 get_type_ret:  pop     eax
                pop     ecx
                pop     ebx
                pop     ebp
                pop     esi
                pop     edi
                ret
;
 endp