;══════════════════════
; 1a) xxx dword ptr [ereg],dword              │ 081HEX, 00XXXreg,    dword
; 1b) xxx dword ptr [esp],dword               │ 081HEX, 00XXX100,   024HEX, dword
; 1c) xxx dword ptr [erega+eregb],dword       │ 081HEX, 00XXX100, 00rgBrgA, dword 
; 1d) xxx dword ptr [dword],dword             │ 081HEX, 00XXX101,    dword, dword
;
;     xxx dword ptr [ereg*2],dword            │ 081HEX, 00XXX100, 01reg101, 00000000, dword
;     xxx dword ptr [ereg*2+dword],dword      │ 081HEX, 00XXX100, 01reg101,    dword, dword
;     xxx dword ptr [ereg*4],dword            │ 081HEX, 00XXX100, 10reg101, 00000000, dword                             
;     xxx dword ptr [ereg*4+dword],dword      │ 081HEX, 00XXX100, 10reg101,    dword, dword
;     xxx dword ptr [ereg*8],dword            │ 081HEX, 00XXX100, 11reg101, 00000000, dword                                
;     xxx dword ptr [ereg*8+dword],dword      │ 081HEX, 00XXX100, 11reg101,    dword, dword
;
;     xxx dword ptr [erega+eregb*2],dword     │ 081HEX, 00XXX100, 01rgBrgA, dword
;     xxx dword ptr [erega+eregb*4],dword     │ 081HEX, 00XXX100, 10rgBrgA, dword
;     xxx dword ptr [erega+eregb*8],dword     │ 081HEX, 00XXX100, 11rgBrgA, dword
;                                             
; 1e) xxx dword ptr [ebp],dword               │ 081HEX, 01XXX101, 00000000, dword 
; 1f) xxx dword ptr [esp+byte],dword          │ 081HEX, 01XXX100,   024HEX,  byte, dword
; 1g) xxx dword ptr [ereg+byte],dword         │ 081HEX, 01XXXreg,     byte, dword
; 1h) xxx dword ptr [erega+eregb+byte],dword  │ 081HEX, 01XXX100, 00rgBrgA,  byte, dword 
;
; 1i) xxx dword ptr [ereg+dword],dword        │ 081HEX, 10XXXreg,    dword, dword
; 1j) xxx dword ptr [ESP+dword],dword         │ 081HEX, 10XXX100,   024HEX, dword, dword
; 1k) xxx dword ptr [erega+eregb+dword],dword │ 081HEX, 10XXX100, 00rgBrgA, dword, dword
;
;  2) xxx reg,dword                           │ 081HEX, 11XXXreg, dword
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB,
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
;  По составу соответствуют "xxx byte ptr [...],byte" (80h),
; только на 3 байта больше
;
 @081_disasm:   lodsb
                sub    ecx,ecx
                add    cl,3
                jmp    @080_disasm_test