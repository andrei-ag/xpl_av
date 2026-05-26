;══════════════════════
;  1) jne jo, jno, jb, jnb, je, jne,       │ 00FHEX, 080 ... 08FHEX, dword
;     jbe, ja, js, jns, jp, jnp, jl,
;     jnl, jle, jg dword
;
;     rdtsc                                │ 00FHEX, 031HEX
;     emms                                 │ 00FHEX, 077HEX
;     setns  reg (8 bit)                   │ 00FHEX, 099HEX, 11***REG
;     cpuid                                │ 00FHEX, 0A2HEX
;     bt     erega,eregb (32 bit)          │ 00FHEX, 0A3HEX, 11rgArgB
;     bts    erega,eregb (32 bit)          │ 00FHEX, 0ABHEX, 11rgArgB
;     btr    erega,eregb (32 bit)          │ 00FHEX, 0B3HEX, 11rgArgB
; 2a) movzx  ereg,byte ptr [rega+regb]     │ 00FHEX, 0B6HEX, 00reg100, 00rgBrgA
; 2b) movzx  ereg,reg (8 bit)              │ 00FHEX, 0B6HEX, 11ergREG
;
;  3) movzx  ereg,REG(16 bit)              │ 00FHEX, 0B7HEX, 11ergREG
;
;     bt     ereg,byte                     │ 00FHEX, 0BAHEX, 11100REG,     byte
;     bts    ereg,byte                     │ 00FHEX, 0BAHEX, 11101REG,     byte
;     btr    ereg,byte                     │ 00FHEX, 0BAHEX, 11110REG,     byte
;     btc    ereg,byte                     │ 00FHEX, 0BAHEX, 11111REG,     byte
;     btc    erega,eregb (32 bit)          │ 00FHEX, 0BBHEX, 11rgArgB
;  4) movsx  ereg,REG(8 bit)               │ 00FHEX, 0BEHEX, 11ergREG
;  5) movsx  ereg,REG(16 bit)              │ 00FHEX, 0BFHEX, 11ergREG
;
; MMX:
;
;     PUNPCKLBW MMa,MMb                    │ 00FHEX, 060HEX, 11MMaMMb
;     PUNPCKLWD MMa,MMb                    │ 00FHEX, 061HEX, 11MMaMMb
;     PUNPCKLDQ MMa,MMb                    │ 00FHEX, 062HEX, 11MMaMMb
;     packsswb  MMa,MMb                    │ 00FHEX, 063HEX, 11MMaMMb
;     PCMPGTB   MMa,MMb                    │ 00FHEX, 064HEX, 11MMaMMb
;     PCMPGTW   MMa,MMb                    │ 00FHEX, 065HEX, 11MMaMMb
;     PCMPGTD   MMa,MMb                    │ 00FHEX, 066HEX, 11MMaMMb
;     packuswb  MMa,MMb                    │ 00FHEX, 067HEX, 11MMaMMb
;     punpckhbw MMa,MMb                    │ 00FHEX, 068HEX, 11MMaMMb
;     PUNPCKHWD MMa,MMb                    │ 00FHEX, 069HEX, 11MMaMMb
;     punpckhdq MMa,MMb                    │ 00FHEX, 06AHEX, 11MMaMMb
;     packssdw  MMa,MMb                    │ 00FHEX, 06BHEX, 11MMaMMb
;     ???
;     movd      mm?,ereg                   │ 00FHEX, 06EHEX, 11MMXreg
;     movq      mm?,ereg                   │ 00FHEX, 06FHEX, 11MMXreg
;
;     PCMPEQB   MMa,MMb                    │ 00FHEX, 074HEX, 11regMMX
;     PCMPEQW   MMa,MMb                    │ 00FHEX, 075HEX, 11regMMX
;     PCMPEQD   MMa,MMb                    │ 00FHEX, 076HEX, 11regMMX
;     movd      ereg,mm?                   │ 00FHEX, 07EHEX, 11regMMX
;
;     ???                                  │ 00FHEX, 0D0HEX, 11MMaMMb
;     psrlw     MMa,MMb                    │ 00FHEX, 0D1HEX, 11MMaMMb
;     psrld     MMa,MMb                    │ 00FHEX, 0D2HEX, 11MMaMMb
;     psrlq     MMa,MMb                    │ 00FHEX, 0D3HEX, 11MMaMMb
;     ???                                  │ 00FHEX, 0D4HEX, 11MMaMMb
;     pmullw    MMa,MMb                    │ 00FHEX, 0D5HEX, 11MMaMMb
;     ???                                  │ 00FHEX, 0D6HEX, 11MMaMMb
;     pmovmskb  MMa,MMb                    │ 00FHEX, 0D7HEX, 11MMaMMb
;     psubusb   MMa,MMb                    │ 00FHEX, 0D8HEX, 11MMaMMb
;     psubusb   MMa,MMb                    │ 00FHEX, 0D9HEX, 11MMaMMb
;     pminub    MMa,MMb                    │ 00FHEX, 0DAHEX, 11MMaMMb
;     pand      MMa,MMb                    │ 00FHEX, 0DBHEX, 11MMaMMb
;     paddusb   MMa,MMb                    │ 00FHEX, 0DCHEX, 11MMaMMb
;     paddusw   MMa,MMb                    │ 00FHEX, 0DDHEX, 11MMaMMb
;     pmaxub    MMa,MMb                    │ 00FHEX, 0DEHEX, 11MMaMMb
;     pandn     MMa,MMb                    │ 00FHEX, 0DFHEX, 11MMaMMb
;
;     pavgb     MMa,MMb                    │ 00FHEX, 0E0HEX, 11MMaMMb
;     psraw     MMa,MMb                    │ 00FHEX, 0E1HEX, 11MMaMMb
;     psrad     MMa,MMb                    │ 00FHEX, 0E2HEX, 11MMaMMb
;     pavgw     MMa,MMb                    │ 00FHEX, 0E3HEX, 11MMaMMb
;     pmulhuw   MMa,MMb                    │ 00FHEX, 0E4HEX, 11MMaMMb
;     pmulhw    MMa,MMb                    │ 00FHEX, 0E5HEX, 11MMaMMb
;     ???                                  │ 00FHEX, 0E6HEX, 11MMaMMb
;     movntq    MMa,MMb                    │ 00FHEX, 0E7HEX, 11MMaMMb
;     psubsb    MMa,MMb                    │ 00FHEX, 0E8HEX, 11MMaMMb
;     psubsw    MMa,MMb                    │ 00FHEX, 0E9HEX, 11MMaMMb
;     pminsw    MMa,MMb                    │ 00FHEX, 0EAHEX, 11MMaMMb
;     por       MMa,MMb                    │ 00FHEX, 0EBHEX, 11MMaMMb
;     paddsb    MMa,MMb                    │ 00FHEX, 0ECHEX, 11MMaMMb
;     paddsw    MMa,MMb                    │ 00FHEX, 0EDHEX, 11MMaMMb
;     pmaxsw    MMa,MMb                    │ 00FHEX, 0EEHEX, 11MMaMMb
;     pxor      MMa,MMb                    │ 00FHEX, 0EFHEX, 11MMaMMb
;
;     ???                                  │ 00FHEX, 0F0HEX, 11MMaMMb
;     psllw     MMa,MMb                    │ 00FHEX, 0F1HEX, 11MMaMMb
;     pslld     MMa,MMb                    │ 00FHEX, 0F2HEX, 11MMaMMb
;     psllq     MMa,MMb                    │ 00FHEX, 0F3HEX, 11MMaMMb
;     pmaddwd   MMa,MMb                    │ 00FHEX, 0F5HEX, 11MMaMMb
;     psadbw    MMa,MMb                    │ 00FHEX, 0F6HEX, 11MMaMMb
;     maskmovq  MMa,MMb                    │ 00FHEX, 0F7HEX, 11MMaMMb
;     psubb     MMa,MMb                    │ 00FHEX, 0F8HEX, 11MMaMMb
;     psubw     MMa,MMb                    │ 00FHEX, 0F9HEX, 11MMaMMb
;     psubd     MMa,MMb                    │ 00FHEX, 0FAHEX, 11MMaMMb
;     ???                                  │ 00FHEX, 0FBHEX, 11MMaMMb
;     paddb     MMa,MMb                    │ 00FHEX, 0FCHEX, 11MMaMMb
;     paddw     MMa,MMb                    │ 00FHEX, 0FDHEX, 11MMaMMb
;     paddd     MMa,MMb                    │ 00FHEX, 0FEHEX, 11MMaMMb
;     ???                                  │ 00FHEX, 0FFHEX, 11MMaMMb
;
 @00F_disasm:   lodsw
                sub    ecx,ecx            
;
 @00F_disasm_start:
                inc    ecx
                cmp    al,031h
                jz    _@one_byte                 ; " rdtsc " 
                cmp    al,060h
                jb     unknown_instr             ; < " PUNPCKLBW MMa,MMb "                  
                cmp    al,06Bh
                jbe    @00F_disasm_three_bytes   ; <= " packssdw MMa,MMb "                  
                cmp    al,06Eh
                jz     @00F_disasm_three_bytes   ; " movd mm?,ereg "                  
                cmp    al,06Fh
                jz     @00F_disasm_three_bytes   ; " movq mm?,ereg "                  
                cmp    al,074h
                jz     @00F_disasm_three_bytes   ; " PCMPEQB MMa,MMb "                  
                cmp    al,075h
                jz     @00F_disasm_three_bytes   ; " PCMPEQW MMa,MMb "                  
                cmp    al,076h
                jz     @00F_disasm_three_bytes   ; " PCMPEQD MMa,MMb "                  
                cmp    al,07Eh
                jz     @00F_disasm_three_bytes   ; " movd ereg,mm? "                  
                cmp    al,077h
                jz    _@one_byte                 ; " emms "                   
;
                cmp    al,080h                   
                jc     unknown_instr             ; < JUMPS DWORD
                cmp    al,08Fh
                jna   _@cj_dword                 ; <= J?? DWORD
;
                cmp    al,099h
                jz     @00F_disasm_three_bytes   ; " setns reg (8 bit) "
                cmp    al,0A2h
                jz    _@one_byte                 ; " cpuid "                   
                cmp    al,0A3h                   
                jz     @00F_disasm_three_bytes   ; " bt erega,eregb(32 bit) "
                cmp    al,0ABh                   
                jz     @00F_disasm_three_bytes   ; " bts erega,eregb(32 bit) "
                cmp    al,0B3h                   
                jz     @00F_disasm_three_bytes   ; " btr erega,eregb(32 bit) "
                cmp    al,0B6h                    
                jz     @00F_disasm_2             ; "2a" or "2b"
                cmp    al,0B7h                   
                jz     @00F_disasm_three_bytes   ; " movzx ereg,REG(16 bit) "
                cmp    al,0BAh                   
                jz     @00F_bts_ereg_byte        ; " bts ereg,byte "
                cmp    al,0BBh                   
                jz     @00F_disasm_three_bytes   ; " btc erega,eregb(32 bit) "
                cmp    al,0BEh                   
                jz     @00F_disasm_three_bytes   ; " movsx ereg,REG(8 bit) "
                cmp    al,0BFh                   
                jz     @00F_disasm_three_bytes   ; " movsx ereg,REG(16 bit) "
;
                cmp    al,0D1h                   
                jb     unknown_instr             ; < " psrlw MMa,MMb "
                cmp    al,0D4h
                jz     unknown_instr
                cmp    al,0D6h
                jz     unknown_instr
                cmp    al,0E6h
                jz     unknown_instr
                cmp    al,0F0h
                jz     unknown_instr
                cmp    al,0FBh
                jz     unknown_instr
                cmp    al,0FFh
                jz     unknown_instr
                jmp    @00F_disasm_three_bytes   ; MMX garbage
;
;     bt     ereg,byte                     │ 00FHEX, 0BAHEX, 11100REG,     byte
;     bts    ereg,byte                     │ 00FHEX, 0BAHEX, 11101REG,     byte
;     btr    ereg,byte                     │ 00FHEX, 0BAHEX, 11110REG,     byte
;     btc    ereg,byte                     │ 00FHEX, 0BAHEX, 11111REG,     byte
;
 @00F_bts_ereg_byte:
                cmp    ah,11100000b
                jc     unknown_instr             ; < BT EAX, ...
;
                inc    ecx                       
                jmp    _@two_bytes               ; instr size = 4 bytes
;
;     setns  reg (8 bit)                   │ 00FHEX, 099HEX, 11***REG
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