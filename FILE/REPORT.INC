;══════════════════════════════════════════════════════════════════════════════
; работа с файлом отчета
;
 proc init_rep          fname
;──────────────────────────────────────────────────────────────────────────────
 init_rep_start:mov     al,byte [report]
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
 create_repf:
;
; откроем файл для чтения
;
 invoke         CreateFileA,[fname],GENERIC_RNW,0,0        
;      
                cmp     eax,-1                   ; выйдем в случае ошибки
                jnz     save_rep_hndl
;───────────────
                inc     byte [report]
                jmp     init_rep_start
;───────────────
 save_rep_hndl: mov     dword [rep_handle],eax
                cmp     byte [report],0
                jnz     init_rep_ret
;
                push    FILE_END 0 0 eax         ; установим указатель в
                invoke  SetFilePointer           ; конец файла
 init_rep_ret:  ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc write_rep         lbuf, dlen
;──────────────────────────────────────────────────────────────────────────────
                cmp     byte [report],2
                jz      write_rep_ret
;───────────────
                push    eax ecx edx 
;
 invoke         WriteFile,[rep_handle],[lbuf],[dlen],number,0
;
                pop     edx ecx eax
 write_rep_ret: ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; вывод отчета о проверке
;
 proc report_msg        
;──────────────────────────────────────────────────────────────────────────────
                push    msg_scanned
                call    write_string
                push    [scanned]
                call    show_hex
                push    msg_eos
                call    write_string
;
                push    msg_infected
                call    write_string
                push    [infected]
                call    show_hex
                push    msg_eos
                call    write_string
;
                push    msg_cleaned
                call    write_string
                push    [cleaned]
                call    show_hex
                push    msg_eos
                call    write_string
;
                push    msg_deleted
                call    write_string
                push    [deleted]
                call    show_hex
                push    msg_eos
                call    write_string
;
                push    msg_modification
                call    write_string
                push    [modification]
                call    show_hex
                push    msg_eos
                call    write_string
                push    msg_eos
                call    write_string
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; вывод на экран имeни файла
;
 proc show_fname        fname
;──────────────────────────────────────────────────────────────────────────────
                push    esi
                mov     esi,[fname]              ; ошибка открытия файла
                cmp     dword [temp+004],4
                jc      show_fname_dsp
                add     esi,dword [temp+004]
;
 show_fname_dsp:
                push    esi
                call    write_string
                push    msg_first
                call    write_string
                pop     esi
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────