;───────────────────────
; xxx word ptr [ereg],word              │ 066HEX, 081HEX, 00XXXerg,     word
; xxx word ptr [esp],word               │ 066HEX, 081HEX, 00XXX100,   024HEX, word
; xxx word ptr [erega+eregb],word       │ 066HEX, 081HEX, 00XXX100, 00rgBrgA, word
; xxx word ptr [dword],word             │ 066HEX, 081HEX, 00XXX101,    dword, word
;
; xxx word ptr [ereg*2],word            │ 066HEX, 081HEX, 00XXX100, 01reg101, 00000000, word
; xxx word ptr [ereg*2+dword],word      │ 066HEX, 081HEX, 00XXX100, 01reg101,    dword, word
; xxx word ptr [ereg*4],word            │ 066HEX, 081HEX, 00XXX100, 10reg101, 00000000, word
; xxx word ptr [ereg*4+dword],word      │ 066HEX, 081HEX, 00XXX100, 10reg101,    dword, word
; xxx word ptr [ereg*8],word            │ 066HEX, 081HEX, 00XXX100, 11reg101, 00000000, word
; xxx word ptr [ereg*8+dword],word      │ 066HEX, 081HEX, 00XXX100, 11reg101,    dword, word
;
; xxx word ptr [erega+eregb*2],word     │ 066HEX, 081HEX, 00XXX100, 01rgBrgA,  word
; xxx word ptr [erega+eregb*4],word     │ 066HEX, 081HEX, 00XXX100, 10rgBrgA,  word
; xxx word ptr [erega+eregb*8],word     │ 066HEX, 081HEX, 00XXX100, 11rgBrgA,  word
;
; xxx word ptr [erega+eregb+byte],word  │ 066HEX, 081HEX, 01XXX101,     byte, word
; xxx word ptr [ebp],word               │ 066HEX, 081HEX, 01XXX101,   000HEX, word
; xxx word ptr [ebp+byte],word          │ 066HEX, 081HEX, 01XXX101,     byte, word
; xxx word ptr [ereg+byte],word         │ 066HEX, 081HEX, 01XXXreg,     byte, word
;
; xxx word ptr [ereg+dword],word        │ 066HEX, 081HEX, 10XXXreg,    dword
; xxx word ptr [esp+dword],word         │ 066HEX, 081HEX, 10XXX100,   024HEX, dword
; xxx word ptr [erega+eregb+dword],word │ 066HEX, 081HEX, 10XXX101,    dword, word
;                                
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
; stosw                                 │ 066HEX, 0A7HEX  
; stosw                                 │ 066HEX, 0ABHEX  
; lodsw                                 │ 066HEX, 0ADHEX
;
 emul_066:      
                lodsb
                lodsb
;
; *** ОПРЕДЕЛЕНИЕ ТИПА ИНСТРУКЦИИ ***
;
                cmp     al,0a7h
                jz      emul_cmps_bwd   ; CMPSW
                cmp     al,0abh
                jz      emul_stos_bwd   ; STOSW
                cmp     al,0adh
                jz      emul_lods_bwd   ; LODSW
                cmp     al,081h
                jz      emul_066_081
                jmp     semul_err_ret
;
; xxx word ptr [...],word              │ 066HEX, 081HEX, 00XXXerg,     word
;
 emul_066_081:
                dec     esi
                push    ebp
                lea     ebp,[edi+ecx-2]
                call    emul_count_offset      ; корректируем смещение (если нужно)
                pop     ebp                    ; и разбираем структуру инструкции
                jc      semul_err_ret
;
; *** ЭМУЛЯЦИЯ ИНСТРУКЦИИ ***
;
		call    calc_offset        ; получим итоговое смещение
                                           ; скорректируем относительно
                                           ; буфера (с учетом секций)
                call    test_off
                jc      semul_err_ret
;
; Create instruction " XXX WORD PTR [OFFSET],KEY " and run it
;
                xor     al,ah
                sub     ah,ah
                push    eax
                call    two_regs
                pop     eax
                xor     al,dl
                xor     al,00000101b
;
                mov     dx,word [edi+ecx-2] ; KEY (word) 
;
                mov     edi,temp_instr_buf
                mov     esi,edi                
;
                push    eax
                mov     ax,8166h
                stosw                      ; ID инструкции (первые 2 байта)
                pop     eax
                stosb                      ; ID инструкции (последний байт)
                mov     eax,ebx                        
                stosd                      ; Store OFFSET 
                mov     eax,edx
                stosw                      ; Store key (word)
;
                sub     ecx,ecx
                mov     cl,3+4+2           ; размер инструкции
                call    run_instr          ; выполним инструкцию
                jmp     spec_emul_ret