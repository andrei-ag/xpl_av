;───────────────────────
; xxx dword ptr [erega],byte             │ 083HEX, 00XXXreg,     byte
; xxx dword ptr [esp],byte               │ 083HEX, 00XXX100,   024HEX,  byte
; xxx dword ptr [erega+eregb],byte       │ 083HEX, 00XXX100, 00rgBrgA,  byte 
; xxx dword ptr [dword],byte             │ 083HEX, 00XXX101,    dword,  byte
;
; xxx dword ptr [ereg*2],byte            │ 080HEX, 00XXX100, 01reg101, 00000000,  byte
; xxx dword ptr [ereg*2+dword],byte      │ 080HEX, 00XXX100, 01reg101,    dword,  byte
; xxx dword ptr [ereg*4],byte            │ 080HEX, 00XXX100, 10reg101, 00000000,  byte                             
; xxx dword ptr [ereg*4+dword],byte      │ 080HEX, 00XXX100, 10reg101,    dword,  byte
; xxx dword ptr [ereg*8],byte            │ 080HEX, 00XXX100, 11reg101, 00000000,  byte                                
; xxx dword ptr [ereg*8+dword],byte      │ 080HEX, 00XXX100, 11reg101,    dword,  byte
;
; xxx dword ptr [erega+eregb*2],byte     │ 080HEX, 00XXX100, 01rgBrgA,  byte
; xxx dword ptr [erega+eregb*4],byte     │ 080HEX, 00XXX100, 10rgBrgA,  byte
; xxx dword ptr [erega+eregb*8],byte     │ 080HEX, 00XXX100, 11rgBrgA,  byte
;                                             
; xxx dword ptr [ebp],byte               │ 083HEX, 01XXX101, 00000000,  byte 
; xxx dword ptr [esp+byte],byte          │ 083HEX, 01XXX100,   024HEX,  byte, byte
; xxx dword ptr [ereg+byte],byte         │ 083HEX, 01XXXreg,     byte,  byte
; xxx dword ptr [erega+eregb+byte],byte  │ 083HEX, 01XXX100, 00rgBrgA,  byte, byte 
;
; xxx dword ptr [ereg+dword],byte        │ 083HEX, 10XXXreg,    dword,  byte
; xxx dword ptr [ESP+dword],byte         │ 083HEX, 10XXX100,   024HEX, dword, byte
; xxx dword ptr [erega+eregb+dword],byte │ 083HEX, 10XXX100, 00rgBrgA, dword, byte
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
 emul_083:      jmp    emul_080