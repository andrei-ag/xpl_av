;══════════════════════                                                         
; 1a) xxx dword ptr [ereg]                │ 0DAHEX, 00XXXreg
; 1b) xxx dword ptr [esp]                 │ 0DAHEX, 00XXX100,   024HEX
; 1c) xxx dword ptr [erega+eregb]         │ 0DAHEX, 00XXX100, 00rgBrgA 
;
;     xxx dword ptr [ereg*2]              │ 0DAHEX, 00XXX100, 01reg101, 00000000                                
;     xxx dword ptr [ereg*2+dword]        │ 0DAHEX, 00XXX100, 01reg101,    dword
;     xxx dword ptr [ereg*4]              │ 0DAHEX, 00XXX100, 10reg101, 00000000                             
;     xxx dword ptr [ereg*4+dword]        │ 0DAHEX, 00XXX100, 10reg101,    dword
;     xxx dword ptr [ereg*8]              │ 0DAHEX, 00XXX100, 11reg101, 00000000                                
;     xxx dword ptr [ereg*8+dword]        │ 0DAHEX, 00XXX100, 11reg101,    dword
;
;     xxx dword ptr [erega+eregb*2]       │ 0DAHEX, 00XXX100, 01rgBrgA
;     xxx dword ptr [erega+eregb*4]       │ 0DAHEX, 00XXX100, 10rgBrgA
;     xxx dword ptr [erega+eregb*8]       │ 0DAHEX, 00XXX100, 11rgBrgA
;                                
; 1d) xxx dword ptr [dword]               │ 0DAHEX, 00XXX101,    dword
;
; 1e) xxx dword ptr [ebp]                 │ 0DAHEX, 01XXX101, 00000000 
; 1f) xxx dword ptr [esp+byte]            │ 0DAHEX, 01XXX100,   024HEX,  byte
; 1g) xxx dword ptr [ereg+byte]           │ 0DAHEX, 01XXXreg,     byte
; 1h) xxx dword ptr [erega+eregb+byte]    │ 0DAHEX, 01XXX100, 00rgBrgA,  byte 
;
; 1i) xxx dword ptr [ereg+dword]          │ 0DAHEX, 10XXXreg,    dword
; 1j) xxx dword ptr [ESP+dword]           │ 0DAHEX, 10XXX100,   024HEX, dword
; 1k) xxx dword ptr [erega+eregb+dword]   │ 0DAHEX, 10XXX100, 00rgBrgA, dword
;
;     xxx dword ptr [erega+eregb*2+dword] │ 0DAHEX, 10XXX100, 01rgBrgA, dword
;     xxx dword ptr [erega+eregb*4+dword] │ 0DAHEX, 10XXX100, 10rgBrgA, dword
;     xxx dword ptr [erega+eregb*8+dword] │ 0DAHEX, 10XXX100, 11rgBrgA, dword
;
;  2) fcmovb st,st(?)                     │ 0DAHEX, 11000???
;     fcmove st,st(?)                     │ 0DAHEX, 11001???
;     fcmovbe st,st(?)                    │ 0DAHEX, 11010???
;     fcmovu st,st(?)                     │ 0DAHEX, 11011???
;     fisub ereg                          │ 0DAHEX, 11100reg
;     fisubr ereg                         │ 0DAHEX, 11101reg
;     fidiv ereg                          │ 0DAHEX, 11110reg
;     fidivr ereg                         │ 0DAHEX, 11111reg
;
; LEGEND: 000 - FIADD, 001 - FIMUL,  010 - FICOM, 011 - FICOMP,
;         100 - FISUB, 101 - FISUBR, 110 - FIDIV, 111 - FIDIVR 
;
 @0DA_disasm:   lodsb
                sub    ecx,ecx
;
                cmp    al,11000000b
                jc     @0FF_disasm_test_x
;
;  2) fcmovb st,st(?)                     │ 0DAHEX, 11000???
;     fcmove st,st(?)                     │ 0DAHEX, 11001???
;     fcmovbe st,st(?)                    │ 0DAHEX, 11010???
;     fcmovu st,st(?)                     │ 0DAHEX, 11011???
;     fisub ereg                          │ 0DAHEX, 11100reg
;     fisubr ereg                         │ 0DAHEX, 11101reg
;     fidiv ereg                          │ 0DAHEX, 11110reg
;     fidivr ereg                         │ 0DAHEX, 11111reg
;
                jmp    _@two_bytes