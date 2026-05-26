;══════════════════════
;  1) jne jo, jno, jb, jnb, je, jne,       │ 00FHEX, 080 ... 08FHEX, dword
;     jbe, ja, js, jns, jp, jnp, jl,
;     jnl, jle, jg dword
;
; 2a) movzx  ereg,byte ptr [rega+regb]     │ 00FHEX, 0B6HEX, 00reg100, 00rgBrgA
; 2b) movzx  ereg,reg (8 bit)              │ 00FHEX, 0B6HEX, 11ergREG
;
;  3) movzx  ereg,REG(16 bit)              │ 00FHEX, 0B7HEX, 11ergREG
;
;  4) movsx  ereg,REG(8 bit)               │ 00FHEX, 0BEHEX, 11ergREG
;  5) movsx  ereg,REG(16 bit)              │ 00FHEX, 0BFHEX, 11ergREG
;
 @00F_disasm:   lodsw
                sub    ecx,ecx            
;
                inc    ecx
                cmp    al,080h                   
                jc     unknown_instr             ; < JUMPS DWORD
                cmp    al,08Fh
                jna   _@cj_dword                 ; <= J?? DWORD
;
                cmp    al,0B6h                    
                jz     @00F_disasm_2             ; "2a" or "2b"
                cmp    al,0B7h                   
                jz     @00F_disasm_three_bytes   ; " movzx ereg,REG(16 bit) "
                cmp    al,0BEh                   
                jz     @00F_disasm_three_bytes   ; " movsx ereg,REG(8 bit) "
                cmp    al,0BFh                   
                jz     @00F_disasm_three_bytes   ; " movsx ereg,REG(16 bit) "
                jmp    unknown_instr
;
; " movzx ereg, reg (8 or 16 bit) "
;
 @00F_disasm_three_bytes:
                cmp    ah,11000000b
                jc     unknown_instr
                jmp   _@two_bytes
;
; 2a) movzx  ereg,byte ptr [rega+regb]     │ 00FHEX, 0B6HEX, 00reg100, 00rgBrgA
; 2b) movzx  ereg,reg (8 bit)              │ 00FHEX, 0B6HEX, 11ergREG
;
 @00F_disasm_2: cmp    ah,11000000b
                jae    @00F_disasm_three_bytes   ; >= movzx eax,al
                cmp    ah,00111100b
                ja     unknown_instr             ; ???
;
                mov    al,ah
                call   two_regs
                cmp    dl,100b
                jnz    unknown_instr             ; not "2A"
;
; " movzx ereg,byte ptr [rega+regb] "
;
 @00F_disasm_ereg_byte_2eregs:
                sub    eax,eax
                lodsb 
                cmp    al,00111111b
                ja     unknown_instr             ; > movzx ereg,byte ptr [edi+edi]
;
                call   two_regs                  ; обрабатываем регистры
                inc    dword [dregs]
                mov    dword [dregs+004],eax
                inc    dword [dregs]
                mov    dword [dregs+008],edx
                sub    edx,edx
                inc    edx                       ; полная имитация
                add    cl,3                      ; размер инструкции (1 + 3)
	        jmp    disasm_ret