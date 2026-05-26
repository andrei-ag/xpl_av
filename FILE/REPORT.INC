.code
;══════════════════════════════════════════════════════════════════════════════
; работа с файлом отчета
;
 init_rep       proc    fname:dword
;──────────────────────────────────────────────────────────────────────────────
 init_rep_start:mov     al,byte ptr [report]
                cmp     al,2
                jc      work_with_rep
                ret
;───────────────
 work_with_rep: push    0 0
                cmp     al,1
                jz      rep_create_nw
 rep_open_ext:  push    OPEN_EXISTING
                jmp     create_repf
 rep_create_nw: push    CREATE_ALWAYS
;
 create_repf:   push    0 0
                push    GENERIC_RNW fname 
                call    CreateFileA              ; откроем файл для чтения
                cmp     eax,-1                   ; выйдем в случае ошибки
                jnz     save_rep_hndl
;───────────────
                inc     byte ptr [report]
                jmp     init_rep_start
;───────────────
 save_rep_hndl: mov     dword ptr [rep_handle],eax
                cmp     byte ptr [report],0
                jnz     init_rep_ret
;
                push    FILE_END 0 0 eax         ; установим указатель в
                call    SetFilePointer           ; конец файла
 init_rep_ret:  ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 write_rep      proc    lbuf:dword, dlen: dword
;──────────────────────────────────────────────────────────────────────────────
                cmp     byte ptr [report],2
                jz      write_rep_ret
;───────────────
                push    eax ecx edx 
                push    0
                push    offset number
                push    dlen
                push    lbuf
                push    rep_handle
                call    WriteFile
                pop     edx ecx eax
 write_rep_ret: ret
 endp
;──────────────────────────────────────────────────────────────────────────────