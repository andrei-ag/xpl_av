;───────────────────────
; xxx dword ptr [ereg],dword              │ 081HEX, 00XXXreg,    dword
; xxx dword ptr [esp],dword               │ 081HEX, 00XXX100,   024HEX, dword
; xxx dword ptr [erega+eregb],dword       │ 081HEX, 00XXX100, 00rgBrgA, dword 
; xxx dword ptr [dword],dword             │ 081HEX, 00XXX101,    dword, dword
;
; xxx dword ptr [ereg*2],dword            │ 081HEX, 00XXX100, 01reg101, 00000000, dword
; xxx dword ptr [ereg*2+dword],dword      │ 081HEX, 00XXX100, 01reg101,    dword, dword
; xxx dword ptr [ereg*4],dword            │ 081HEX, 00XXX100, 10reg101, 00000000, dword                             
; xxx dword ptr [ereg*4+dword],dword      │ 081HEX, 00XXX100, 10reg101,    dword, dword
; xxx dword ptr [ereg*8],dword            │ 081HEX, 00XXX100, 11reg101, 00000000, dword                                
; xxx dword ptr [ereg*8+dword],dword      │ 081HEX, 00XXX100, 11reg101,    dword, dword
;
; xxx dword ptr [erega+eregb*2],dword     │ 081HEX, 00XXX100, 01rgBrgA, dword
; xxx dword ptr [erega+eregb*4],dword     │ 081HEX, 00XXX100, 10rgBrgA, dword
; xxx dword ptr [erega+eregb*8],dword     │ 081HEX, 00XXX100, 11rgBrgA, dword
;
; xxx dword ptr [ebp],dword               │ 081HEX, 01XXX101, 00000000, dword 
; xxx dword ptr [esp+byte],dword          │ 081HEX, 01XXX100,   024HEX,  byte, dword
; xxx dword ptr [ereg+byte],dword         │ 081HEX, 01XXXreg,     byte, dword
; xxx dword ptr [erega+eregb+byte],dword  │ 081HEX, 01XXX100, 00rgBrgA,  byte, dword 
;
; xxx dword ptr [ereg+dword],dword        │ 081HEX, 10XXXreg,    dword, dword
; xxx dword ptr [ESP+dword],dword         │ 081HEX, 10XXX100,   024HEX, dword, dword
; xxx dword ptr [erega+eregb+dword],dword │ 081HEX, 10XXX100, 00rgBrgA, dword, dword
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB,
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
 emul_081:    
                push    ebp
                lea     ebp,[edi+ecx-4]
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
; Create instruction " XXX DWORD PTR [OFFSET],KEY " and run it
;
                xor     al,ah
                sub     ah,ah
                push    eax
                call    two_regs
                pop     eax
                xor     al,dl
                xor     al,00000101b
                mov     ah,al
                mov     al,byte [edi]
;
                mov     edx,dword [edi+ecx-4] ; KEY (dword) 
;
                mov     edi,temp_instr_buf
                mov     esi,edi                
                stosw                      ; Store instruction ID
                mov     eax,ebx                        
                stosd                      ; Store OFFSET 
                mov     eax,edx
                stosd                      ; Store key (dword)
;
                sub     ecx,ecx
                mov     cl,2+4+4           ; размер инструкции
                call    run_instr          ; выполним инструкцию
                jmp     spec_emul_ret