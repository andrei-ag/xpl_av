;────────────────────────
; * * * Имитация исполнения инструкции "stos b/w/d"
;
;
 proc emul_stos              
;
                mov     esi,edi
                mov     ebx,dword [regs+7*4]
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_stos_ret
;
; Сформируем в буфере следующий код
; 
; MOV EDI, РАСЧИТАННОЕ ВЫШЕ СМЕЩЕНИЕ
; STOS(B/W/D)
; RET
;
; и запустим его на выполнение
;
                mov     edi,temp_instr_buf
                push    edi
                mov     al,0BFh            ; ID = Инструкции (MOV EDI, ...)
                stosb                      
                mov     eax,ebx
                stosd                      ; СМЕЩЕНИЕ
;
                push    ecx esi
                rep     movsb              ; перенесем инструкцию (stos...)
                pop     esi ecx
; 
                mov     al,0c3h
                stosb                      ; RET
;
                mov     eax,dword [regs+0*4]
                pop     edi
                push    esi
                call    edi                ; выполним инструкции
                pop     esi               
;
;  Рассчитаем размер перенесенных байтов по ID инструкции и увеличим
; на него значение регистра EDI
;
                sub     ecx,ecx
                lodsb
;                
                inc     ecx                ; + 1 байт
                cmp     al,0aah
                jz      stos_set_size      ; -> STOSB (1 байт)
                cmp     al,66h             
                jz      its_stos_w         ; -> STOSW (2 байта)
 its_stos_d:    inc     ecx                ; -> STOSD (4 байта)
                inc     ecx
 its_stos_w:    inc     ecx

 stos_set_size: add     dword [regs+7*4],ecx
 emul_stos_ret: ret
;
 endp