;══════════════════════                                                         
; 1a) xxx dword ptr [ereg]                │ 0DEHEX, 00XXXreg
; 1b) xxx dword ptr [esp]                 │ 0DEHEX, 00XXX100,   024HEX
; 1c) xxx dword ptr [erega+eregb]         │ 0DEHEX, 00XXX100, 00rgBrgA 
;
;     xxx dword ptr [ereg*2]              │ 0DEHEX, 00XXX100, 01reg101, 00000000                                
;     xxx dword ptr [ereg*2+dword]        │ 0DEHEX, 00XXX100, 01reg101,    dword
;     xxx dword ptr [ereg*4]              │ 0DEHEX, 00XXX100, 10reg101, 00000000                             
;     xxx dword ptr [ereg*4+dword]        │ 0DEHEX, 00XXX100, 10reg101,    dword
;     xxx dword ptr [ereg*8]              │ 0DEHEX, 00XXX100, 11reg101, 00000000                                
;     xxx dword ptr [ereg*8+dword]        │ 0DEHEX, 00XXX100, 11reg101,    dword
;
;     xxx dword ptr [erega+eregb*2]       │ 0DEHEX, 00XXX100, 01rgBrgA
;     xxx dword ptr [erega+eregb*4]       │ 0DEHEX, 00XXX100, 10rgBrgA
;     xxx dword ptr [erega+eregb*8]       │ 0DEHEX, 00XXX100, 11rgBrgA
;                                
; 1d) xxx dword ptr [dword]               │ 0DEHEX, 00XXX101,    dword
;
; 1e) xxx dword ptr [ebp]                 │ 0DEHEX, 01XXX101, 00000000 
; 1f) xxx dword ptr [esp+byte]            │ 0DEHEX, 01XXX100,   024HEX,  byte
; 1g) xxx dword ptr [ereg+byte]           │ 0DEHEX, 01XXXreg,     byte
; 1h) xxx dword ptr [erega+eregb+byte]    │ 0DEHEX, 01XXX100, 00rgBrgA,  byte 
;
; 1i) xxx dword ptr [ereg+dword]          │ 0DEHEX, 10XXXreg,    dword
; 1j) xxx dword ptr [ESP+dword]           │ 0DEHEX, 10XXX100,   024HEX, dword
; 1k) xxx dword ptr [erega+eregb+dword]   │ 0DEHEX, 10XXX100, 00rgBrgA, dword
;
;     xxx dword ptr [erega+eregb*2+dword] │ 0DEHEX, 10XXX100, 01rgBrgA, dword
;     xxx dword ptr [erega+eregb*4+dword] │ 0DEHEX, 10XXX100, 10rgBrgA, dword
;     xxx dword ptr [erega+eregb*8+dword] │ 0DEHEX, 10XXX100, 11rgBrgA, dword
;
;  2) faddp st(?),st                      │ 0DEHEX, 11000???
;     fmulp st(?),st                      │ 0DEHEX, 11001???
;     ficom ereg                          │ 0DEHEX, 11010reg
;     ficomp ereg                         │ 0DEHEX, 11011reg
;     fsubrp st(?),st                     │ 0DEHEX, 11100???
;     fsubp st(?),st                      │ 0DEHEX, 11101???
;     fdivrp st(?),st                     │ 0DEHEX, 11110???
;     fdivp st(?),st                      │ 0DEHEX, 11111???
;
;
; LEGEND: 000 - FIADD, 001 - FIMUL, 010 - FICOM, 011 - FICOMP,
;         100 - FISUB, 101 - FSUBR, 110 - FIDIV, 111 - FIDIVR 
;
 @0DE_disasm:   lodsb
                sub    ecx,ecx
;
                cmp    al,11000000b
                jc     @0FF_disasm_test_x
;
;  2) faddp st(?),st                      │ 0DEHEX, 11000???
;     fmulp st(?),st                      │ 0DEHEX, 11001???
;     ficom ereg                          │ 0DEHEX, 11010reg
;     ficomp ereg                         │ 0DEHEX, 11011reg
;     fsubrp st(?),st                     │ 0DEHEX, 11100???
;     fsubp st(?),st                      │ 0DEHEX, 11101???
;     fdivrp st(?),st                     │ 0DEHEX, 11110???
;     fdivp st(?),st                      │ 0DEHEX, 11111???
;
                jmp    _@two_bytes