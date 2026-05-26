;───────────────────────
; mov dword ptr [ereg],dword              │ 0C7HEX, 00000reg,    dword
; mov dword ptr [esp],dword               │ 0C7HEX, 00000100,   024HEX, dword
; mov dword ptr [erega+eregb],dword       │ 0C7HEX, 00000100, 00rgBrgA, dword 
; mov dword ptr [dword],dword             │ 0C7HEX, 00000101,    dword, dword
;
; mov dword ptr [ereg*2],dword            │ 0C7HEX, 00000100, 01reg101, 00000000, dword
; mov dword ptr [ereg*2+dword],dword      │ 0C7HEX, 00000100, 01reg101,    dword, dword
; mov dword ptr [ereg*4],dword            │ 0C7HEX, 00000100, 10reg101, 00000000, dword                             
; mov dword ptr [ereg*4+dword],dword      │ 0C7HEX, 00000100, 10reg101,    dword, dword
; mov dword ptr [ereg*8],dword            │ 0C7HEX, 00000100, 11reg101, 00000000, dword                                
; mov dword ptr [ereg*8+dword],dword      │ 0C7HEX, 00000100, 11reg101,    dword, dword
;
; mov dword ptr [erega+eregb*2],dword     │ 0C7HEX, 00000100, 01rgBrgA, dword
; mov dword ptr [erega+eregb*4],dword     │ 0C7HEX, 00000100, 10rgBrgA, dword
; mov dword ptr [erega+eregb*8],dword     │ 0C7HEX, 00000100, 11rgBrgA, dword
;                                             
; mov dword ptr [ebp],dword               │ 0C7HEX, 01000101, 00000000, dword 
; mov dword ptr [esp+byte],dword          │ 0C7HEX, 01000100,   024HEX,  byte, dword
; mov dword ptr [ereg+byte],dword         │ 0C7HEX, 01000reg,     byte, dword
; mov dword ptr [erega+eregb+byte],dword  │ 0C7HEX, 01000100, 00rgBrgA,  byte, dword
;
; mov dword ptr [ereg+dword],dword        │ 0C7HEX, 10000reg,    dword, dword
; mov dword ptr [ESP+dword],dword         │ 0C7HEX, 10000100,   024HEX, dword, dword
; mov dword ptr [erega+eregb+dword],dword │ 0C7HEX, 10000100, 00rgBrgA, dword, dword
;                      
 emul_0C7:      jmp    emul_081