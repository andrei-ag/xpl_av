;────────────────────────
; * * * Имитация исполнения инструкции "cmps b/w/d"
;

;
; Для имитации выполнения CMPS B/W/D сформируем в буфере следующий код
; 
; PUSH ESI EDI
; MOV ESI, OFFSET
; MOV EDI, OFFSET
; CMPS(B/W/D)
; POP EDI ESI
; RET
;
; и запустим его на выполнение
;
 proc emul_cmps              
;
                pushad
                push    edi
                pop     esi                ; ESI указывает на начало инструкции
;
                mov     edi,temp_instr_buf
                mov     ax,5756h           ; ID = PUSH ESI EDI
                stosw                      ; Store ID
                mov     al,0BEh            ; ID = MOV ESI, dword
                stosb                      ; Store ID

                mov     ebx,dword [regs+6*4] 
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_cmps_ret
;
                mov     eax,ebx
                stosd                      ; Store DWORD
                mov     al,0BFh            ; ID = MOV EDI, dword
                stosb                      ; Store ID
;
                mov     ebx,dword [regs+7*4]
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      emul_cmps_ret
;
                mov     eax,ebx
                stosd                      ; Store DWORD
;
                push    esi
                push    ecx
                rep     movsb              ; Store CMPS BYTE/WORD/DWORD
                pop     ecx
                pop     esi
;
                mov     ax,5e5fh           ; ID = POP EDI ESI
                stosw
;
                push    esi
                add     cl,1+1+5+5+1+1     ; размер инструкций
                mov     esi,temp_instr_buf
                call    run_instr          ; выполним инструкцию
                pop     esi
;
;  Рассчитаем размер перенесенных байтов по ID инструкции и увеличим
; на него значение регистров ESI, EDI
;
                sub     ecx,ecx
                lodsb
;                
                inc     ecx                ; + 1 байт
                cmp     al,0a6h
                jz      cmps_set_size      ; -> CMPSB (1 байт)
                cmp     al,66h             
                jz      its_cmps_w         ; -> CMPSW (2 байта)
 its_cmps_d:    inc     ecx                ; -> CMPSD (4 байта)
                inc     ecx
 its_cmps_w:    inc     ecx

 cmps_set_size: add     dword [regs+6*4],ecx
		add     dword [regs+7*4],ecx
;
 emul_cmps_ret: popad
                ret
;
 endp