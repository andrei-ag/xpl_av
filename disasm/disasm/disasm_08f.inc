;══════════════════════
; 1a) pop dword ptr [ereg]                │ 08FHEX, 00000reg
; 1b) pop dword ptr [esp]                 │ 08FHEX, 00000100,   024HEX
; 1c) pop dword ptr [erega+eregb]         │ 08FHEX, 00000100, 00rgBrgA 
; 1d) pop dword ptr [dword]               │ 08FHEX, 00000101,    dword
;
;     pop dword ptr [ereg*2]              │ 08FHEX, 00000100, 01reg101, 00000000                                
;     pop dword ptr [ereg*2+dword]        │ 08FHEX, 00000100, 01reg101,    dword
;     pop dword ptr [ereg*4]              │ 08FHEX, 00000100, 10reg101, 00000000                             
;     pop dword ptr [ereg*4+dword]        │ 08FHEX, 00000100, 10reg101,    dword
;     pop dword ptr [ereg*8]              │ 08FHEX, 00000100, 11reg101, 00000000                                
;     pop dword ptr [ereg*8+dword]        │ 08FHEX, 00000100, 11reg101,    dword
;
;     pop dword ptr [erega+eregb*2]       │ 08FHEX, 00000100, 01rgBrgA
;     pop dword ptr [erega+eregb*4]       │ 08FHEX, 00000100, 10rgBrgA
;     pop dword ptr [erega+eregb*8]       │ 08FHEX, 00000100, 11rgBrgA
;                                
; 1e) pop dword ptr [ebp]                 │ 08FHEX, 01000101, 00000000 
; 1f) pop dword ptr [esp+byte]            │ 08FHEX, 01000100,   024HEX,  byte
; 1g) pop dword ptr [ereg+byte]           │ 08FHEX, 01000reg,     byte
; 1h) pop dword ptr [erega+eregb+byte]    │ 08FHEX, 01000100, 00rgBrgA,  byte 
;
; 1i) pop dword ptr [ereg+dword]          │ 08FHEX, 10000reg,    dword
; 1j) pop dword ptr [ESP+dword]           │ 08FHEX, 10000100,   024HEX, dword
; 1k) pop dword ptr [erega+eregb+dword]   │ 08FHEX, 10000100, 00rgBrgA, dword
;
;  2) pop ereg                            │ 08FHEX, 10000reg
;
 @08F_disasm:   lodsb
                sub    ecx,ecx
;
                push   eax
                push   edx
;
                sub    ah,ah
                cmp    al,00111111b
                jbe    @08F_disasm_test
;                 
                mov    ah,01000000b
                cmp    al,01111111b
                jbe    @08F_disasm_test
;
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @08F_disasm_test
;
                mov    ah,11000000b
 @08F_disasm_test:
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    al,000b
                pop    edx
                pop    eax
                jnz    unknown_instr
;
                cmp    al,11000000b
                jb     @0FF_disasm_test_x   ; 1 (i - k)
;
;  2) pop ereg                            │ 08FHEX, 11000reg
;
                add    cl,2
                jmp    disasm_ret