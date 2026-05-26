;══════════════════════                                                         
; 1a) xxx dword ptr [ereg]                │ 0D8HEX, 00XXXreg
; 1b) xxx dword ptr [esp]                 │ 0D8HEX, 00XXX100,   024HEX
; 1c) xxx dword ptr [erega+eregb]         │ 0D8HEX, 00XXX100, 00rgBrgA 
;
;     xxx dword ptr [ereg*2]              │ 0D8HEX, 00XXX100, 01reg101, 00000000                                
;     xxx dword ptr [ereg*2+dword]        │ 0D8HEX, 00XXX100, 01reg101,    dword
;     xxx dword ptr [ereg*4]              │ 0D8HEX, 00XXX100, 10reg101, 00000000                             
;     xxx dword ptr [ereg*4+dword]        │ 0D8HEX, 00XXX100, 10reg101,    dword
;     xxx dword ptr [ereg*8]              │ 0D8HEX, 00XXX100, 11reg101, 00000000                                
;     xxx dword ptr [ereg*8+dword]        │ 0D8HEX, 00XXX100, 11reg101,    dword
;
;     xxx dword ptr [erega+eregb*2]       │ 0D8HEX, 00XXX100, 01rgBrgA
;     xxx dword ptr [erega+eregb*4]       │ 0D8HEX, 00XXX100, 10rgBrgA
;     xxx dword ptr [erega+eregb*8]       │ 0D8HEX, 00XXX100, 11rgBrgA
;                                
; 1d) xxx dword ptr [dword]               │ 0D8HEX, 00XXX101,    dword
;
; 1e) xxx dword ptr [ebp]                 │ 0D8HEX, 01XXX101, 00000000 
; 1f) xxx dword ptr [esp+byte]            │ 0D8HEX, 01XXX100,   024HEX,  byte
; 1g) xxx dword ptr [ereg+byte]           │ 0D8HEX, 01XXXreg,     byte
; 1h) xxx dword ptr [erega+eregb+byte]    │ 0D8HEX, 01XXX100, 00rgBrgA,  byte 
;
; 1i) xxx dword ptr [ereg+dword]          │ 0D8HEX, 10XXXreg,    dword
; 1j) xxx dword ptr [ESP+dword]           │ 0D8HEX, 10XXX100,   024HEX, dword
; 1k) xxx dword ptr [erega+eregb+dword]   │ 0D8HEX, 10XXX100, 00rgBrgA, dword
;
;     xxx dword ptr [erega+eregb*2+dword] │ 0D8HEX, 10XXX100, 01rgBrgA, dword
;     xxx dword ptr [erega+eregb*4+dword] │ 0D8HEX, 10XXX100, 10rgBrgA, dword
;     xxx dword ptr [erega+eregb*8+dword] │ 0D8HEX, 10XXX100, 11rgBrgA, dword
;
;  2) fadd st,st(?)                       │ 0D8HEX, 11000???
;     fmul st,st(?)                      │ 0D8HEX, 11001???
;     fcom st(?)                          │ 0D8HEX, 11010???
;     fcomp st(?)                         │ 0D8HEX, 11011???
;     fsub st,st(?)                       │ 0D8HEX, 11100???
;     fsubr st,st(?)                      │ 0D8HEX, 11101???
;     fdiv st,st(?)                       │ 0D8HEX, 11110???
;     fdivr st,st(?)                      │ 0D8HEX, 11111???
;
; LEGEND: 000 - FADD, 001 - FMUL,  010 - FCOM, 011 - FCOMP,
;         100 - FSUB, 101 - FSUBR, 110 - FDIV, 111 - FDIVR 
;
 @0D8_disasm:   lodsb
                sub    ecx,ecx
;
                cmp    al,11000000b
                jc     @0FF_disasm_test_x
;
;  2) fadd st,st(?)                       │ 0D8HEX, 11000???
;     fmulp st,st(?)                      │ 0D8HEX, 11001???
;     fcom st(?)                          │ 0D8HEX, 11010???
;     fcomp st(?)                         │ 0D8HEX, 11011???
;     fsub st,st(?)                       │ 0D8HEX, 11100???
;     fsubr st,st(?)                      │ 0D8HEX, 11101???
;     fdiv st,st(?)                       │ 0D8HEX, 11110???
;     fdivr st,st(?)                      │ 0D8HEX, 11111???
;
                jmp    _@two_bytes