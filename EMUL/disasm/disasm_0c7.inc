;══════════════════════
; 1a) mov dword ptr [ereg],dword              │ 0C7HEX, 00000reg,    dword
; 1b) mov dword ptr [esp],dword               │ 0C7HEX, 00000100,   024HEX, dword
; 1c) mov dword ptr [erega+eregb],dword       │ 0C7HEX, 00000100, 00rgBrgA, dword 
; 1d) mov dword ptr [dword],dword             │ 0C7HEX, 00000101,    dword, dword
;
;     mov dword ptr [ereg*2],dword            │ 0C7HEX, 00000100, 01reg101, 00000000, dword
;     mov dword ptr [ereg*2+dword],dword      │ 0C7HEX, 00000100, 01reg101,    dword, dword
;     mov dword ptr [ereg*4],dword            │ 0C7HEX, 00000100, 10reg101, 00000000, dword                             
;     mov dword ptr [ereg*4+dword],dword      │ 0C7HEX, 00000100, 10reg101,    dword, dword
;     mov dword ptr [ereg*8],dword            │ 0C7HEX, 00000100, 11reg101, 00000000, dword                                
;     mov dword ptr [ereg*8+dword],dword      │ 0C7HEX, 00000100, 11reg101,    dword, dword
;
;     mov dword ptr [erega+eregb*2],dword     │ 0C7HEX, 00000100, 01rgBrgA, dword
;     mov dword ptr [erega+eregb*4],dword     │ 0C7HEX, 00000100, 10rgBrgA, dword
;     mov dword ptr [erega+eregb*8],dword     │ 0C7HEX, 00000100, 11rgBrgA, dword
;                                             
; 1e) mov dword ptr [ebp],dword               │ 0C7HEX, 01000101, 00000000, dword 
; 1f) mov dword ptr [esp+byte],dword          │ 0C7HEX, 01000100,   024HEX,  byte, dword
; 1g) mov dword ptr [ereg+byte],dword         │ 0C7HEX, 01000reg,     byte, dword
; 1h) mov dword ptr [erega+eregb+byte],dword  │ 0C7HEX, 01000100, 00rgBrgA,  byte, dword
;
; 1i) mov dword ptr [ereg+dword],dword        │ 0C7HEX, 10000reg,    dword, dword
; 1j) mov dword ptr [ESP+dword],dword         │ 0C7HEX, 10000100,   024HEX, dword, dword
; 1k) mov dword ptr [erega+eregb+dword],dword │ 0C7HEX, 10000100, 00rgBrgA, dword, dword
;
;  2) mov ereg,dword                          │ 0C7HEX, 10000reg,    dword
;
;  По способу разбора совпадают с "0FFh", отличаются лишь
; размером (больше на 4 байта)
;
 @0C7_disasm:   lodsb
                sub    ecx,ecx
;
                push   eax
                push   edx
;
                sub    ah,ah
                cmp    al,00111111b
                jbe    @0C7_disasm_test
;                 
                mov    ah,01000000b
                cmp    al,01111111b                              
                jbe    @0C7_disasm_test
;
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @0C7_disasm_test
;
                mov    ah,11000000b
 @0C7_disasm_test:
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    al,000b
                pop    edx
                pop    eax
                jnz    unknown_instr
;
                add    cl,4               ; увеличим размер
                cmp    al,11000000b
                jb     @0FF_disasm_test_x   ; 1 (i - k)
;
;  2) mov ereg,dword                          │ 0C7HEX, 10000reg,    dword
;
                add    cl,2
                jmp    disasm_ret