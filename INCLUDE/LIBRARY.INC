.code
;══════════════════════════════════════════════════════════════════════════════
 get_command    proc
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx ebx edi esi
                call    GetCommandLineA
                inc     eax                      ; eax - расположение параметров
                xchg    esi,eax                  ; esi <> eax
                call    str_to_low
                sub     eax,eax
                sub     ecx,ecx
                mov     cl,150                   ; максимальная длина параметров
;───────────────
; пропустим первый параметр
;
 to_second_lp:  lodsb
                cmp     al,'"'
                jz      to_second_nxt
                cmp     al,0
                jz      get_command_r
                cmp     al,20h
                jz      load_switches
 to_second_nxt: loop    to_second_lp
                sub     eax,eax
 get_command_r: pop     esi edi ebx edx ecx
                ret
;───────────────
 load_switches: cmp     byte ptr [esi],0
                jz      get_command_r-2
                cmp     byte ptr [esi],'/'
                jnz     _load_path
 second_switch: lodsb
                lodsw
                cmp     al,'*'
                jz      set_all_hdd
                cmp     al,'c'
                jz      set_cure
                cmp     ax,'ar'
                jz      set_rep_app
                cmp     ax,'cr'
                jz      set_rep_crt
                cmp     al,'d'
                jnz     get_command_r-2          ; ошибка - неизвестная команда
;───────────────
 set_deb_mode:  inc     byte ptr [debug_mode]
                jmp     load_swtch_lp
;───────────────
 set_all_hdd:   call    getfixed_drive
                jmp     load_swtch_lp
;───────────────
 set_rep_crt:   mov     byte ptr [report],1
                dec     ecx
                inc     esi
                jmp     load_swtch_lp
;
 set_rep_app:   mov     byte ptr [report],0
                dec     ecx
                inc     esi
                jmp     load_swtch_lp
;───────────────
 set_cure:      mov     byte ptr [try_cure],1    ; лечить файлы
 load_swtch_lp: sub     ecx,3
                cmp     ecx,0
                ja      load_switches
                jmp     get_command_r-2
;───────────────
; если был указан ключ "/*" (проверка всех дисков), то проверку в указанном
; каталоге не проводим.
;
 _load_path:    mov     edi,offset find_buf+1
;
                cmp     byte ptr [fixed_drives],0
                jz      load_path
                mov     al,byte ptr [fixed_drives+1]
                add     al,'a'
                stosb
                mov     ax,'\:'
                stosw
                jmp     load_path_end
;
 load_path:     sub     edx,edx
 load_path_lp:  lodsb
                cmp     al,0
                jz      load_path_end
                cmp     al,'"'
                jz      load_path_end
                stosb
                ;inc     edx
                loop    load_path_lp
;───────────────
 load_path_end: sub     eax,eax
                stosb
                push    offset find_buf+1
                call    lstrlenA
                jmp     get_command_r
 endp
;──────────────────────────────────────────────────────────────────────────────
 getfixed_drive proc
;──────────────────────────────────────────────────────────────────────────────
                pushad
                mov     edi,offset fixed_drives+1
;
 drive_loop:    cmp     byte ptr [drive_path],'z'
                ja      gfix_drv_ret             ; "z" последний диск
                push    offset drive_path
                call    GetDriveTypeA            ; получим тип диска
                cmp     al,DRIVE_FIXED
                jnz     next_drive               ; если не жесткий, пропустим
;───────────────
                inc     byte ptr [fixed_drives]  ;
                mov     al,byte ptr [drive_path]
                sub     al,'a'
                stosb                            ;
 next_drive:    inc     byte ptr [drive_path]
                jmp     drive_loop
;─ ─ ─ ─ ─ ─ ─ ─
 gfix_drv_ret:  popad
                mov     byte ptr [drive_path],'a'
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; eax - value to align
; ecx - align to
;
 align_value    proc
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx
                sub     edx,edx
                div     ecx
;
                or      edx,edx
                jz      align_val_ret
;
                inc     eax
 align_val_ret: mul     ecx
                pop     edx ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 str_to_low     proc
;──────────────────────────────────────────────────────────────────────────────
                push    esi
 str2low_lp:    cmp     byte ptr [esi],0
                jz      str2low_ret
                cmp     byte ptr [esi],'A'
                jc      str2low_next
                cmp     byte ptr [esi],'Z'
                ja      str2low_next
                add     byte ptr [esi],20h
 str2low_next:  inc     esi
                jmp     str2low_lp
 str2low_ret:   pop     esi
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 read_file      proc    lbuf:dword, dlen: dword
;──────────────────────────────────────────────────────────────────────────────
                push    0
                push    offset number
                push    dlen
                push    lbuf
                push    handle
                call    ReadFile
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 write_file     proc    lbuf:dword, dlen: dword
;──────────────────────────────────────────────────────────────────────────────
                push    0
                push    offset number
                push    dlen
                push    lbuf
                push    handle
                call    WriteFile
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────