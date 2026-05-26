;══════════════════════
; 1a) xxx dword ptr [ereg],byte              │ 083HEX, 00XXXreg,     byte
; 1b) xxx dword ptr [esp],byte               │ 083HEX, 00XXX100,   024HEX,  byte
; 1c) xxx dword ptr [erega+eregb],byte       │ 083HEX, 00XXX100, 00rgBrgA,  byte 
; 1d) xxx dword ptr [dword],byte             │ 083HEX, 00XXX101,    dword,  byte
;
;     xxx dword ptr [ereg*2],byte            │ 083HEX, 00XXX100, 01reg101, 00000000,  byte
;     xxx dword ptr [ereg*2+dword],byte      │ 083HEX, 00XXX100, 01reg101,    dword,  byte
;     xxx dword ptr [ereg*4],byte            │ 083HEX, 00XXX100, 10reg101, 00000000,  byte                             
;     xxx dword ptr [ereg*4+dword],byte      │ 083HEX, 00XXX100, 10reg101,    dword,  byte
;     xxx dword ptr [ereg*8],byte            │ 083HEX, 00XXX100, 11reg101, 00000000,  byte                                
;     xxx dword ptr [ereg*8+dword],byte      │ 083HEX, 00XXX100, 11reg101,    dword,  byte
;
;     xxx dword ptr [erega+eregb*2],byte     │ 083HEX, 00XXX100, 01rgBrgA,  byte
;     xxx dword ptr [erega+eregb*4],byte     │ 083HEX, 00XXX100, 10rgBrgA,  byte
;     xxx dword ptr [erega+eregb*8],byte     │ 083HEX, 00XXX100, 11rgBrgA,  byte
;                                             
; 1e) xxx dword ptr [ebp],byte               │ 083HEX, 01XXX101, 00000000,  byte 
; 1f) xxx dword ptr [esp+byte],byte          │ 083HEX, 01XXX100,   024HEX,  byte, byte
; 1g) xxx dword ptr [ereg+byte],byte         │ 083HEX, 01XXXreg,     byte,  byte
; 1h) xxx dword ptr [erega+eregb+byte],byte  │ 083HEX, 01XXX100, 00rgBrgA,  byte, byte 
;
; 1i) xxx dword ptr [ereg+dword],byte        │ 083HEX, 10XXXreg,    dword,  byte
; 1j) xxx dword ptr [ESP+dword],byte         │ 083HEX, 10XXX100,   024HEX, dword, byte
; 1k) xxx dword ptr [erega+eregb+dword],byte │ 083HEX, 10XXX100, 00rgBrgA, dword, byte
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB,
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
;  2) add reg,byte                          │ 083HEX, 11000reg, byte
;  3) or  reg,byte                          │ 083HEX, 11001reg, byte
;  4) adc reg,byte                          │ 083HEX, 11010reg, byte
;  5) sbb reg,byte                          │ 083HEX, 11011reg, byte
;  6) and reg,byte                          │ 083HEX, 11100reg, byte
;  7) sub reg,byte                          │ 083HEX, 11101reg, byte
;  8) xor reg,byte                          │ 083HEX, 11110reg, byte
;  9) cmp reg,byte                          │ 083HEX, 11111reg, byte
;
; Разбираются аналогично "080h", совпадает даже размер
;
 @083_disasm:   jmp    @080_disasm