;────────────────────────
; * * * Имитация исполнения инструкции "lods b/w/d"
;

;
 proc emul_lods              
;
                mov     esi,edi
		mov     ebx,dword [regs+6*4]
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_lods_ret
;
; Сформируем в буфере следующий код
; 
; MOV ESI, РАСЧИТАННОЕ ВЫШЕ СМЕЩЕНИЕ
; LODS(B/W/D)
; RET
;
; и запустим его на выполнение
;
                mov     edi,temp_instr_buf
                push    edi
                mov     al,0BEh            ; ID = Инструкции (MOV ESI, ...)
                stosb                      
                mov     eax,ebx
                stosd                      ; СМЕЩЕНИЕ
;
                push    esi ecx
                rep     movsb              ; перенесем инструкцию (lods...)
                pop     ecx esi
;
                mov     al,0c3h
                stosb                      ; RET
; 
                mov     eax,dword [regs+0*4]
                pop     edi
                push    esi
                call    edi                ; выполним инструкции
                pop     esi               
                mov     dword [regs+0*4],eax
;
;  Рассчитаем размер перенесенных байтов по ID инструкции и увеличим
; на него значение регистра ESI
;
                sub     ecx,ecx
                lodsb
;                
                inc     ecx                ; + 1 байт
                cmp     al,0ach
                jz      lods_set_size      ; -> LODSB (1 байт)
                cmp     al,66h             
                jz      its_lods_w         ; -> LODSW (2 байта)
 its_lods_d:    inc     ecx                ; -> LODSD (4 байта)
                inc     ecx
 its_lods_w:    inc     ecx

 lods_set_size: add     dword [regs+6*4],ecx
 emul_lods_ret: ret
;
 endp