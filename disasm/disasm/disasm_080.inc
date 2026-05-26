;══════════════════════
; 1a) xxx byte ptr [ereg],byte              │ 080HEX, 00XXXreg,     byte
; 1b) xxx byte ptr [esp],byte               │ 080HEX, 00XXX100,   024HEX,  byte
; 1c) xxx byte ptr [erega+eregb],byte       │ 080HEX, 00XXX100, 00rgBrgA,  byte 
; 1d) xxx byte ptr [dword],byte             │ 080HEX, 00XXX101,    dword,  byte
;
;     xxx byte ptr [ereg*2],byte            │ 080HEX, 00XXX100, 01reg101, 00000000,  byte
;     xxx byte ptr [ereg*2+dword],byte      │ 080HEX, 00XXX100, 01reg101,    dword,  byte
;     xxx byte ptr [ereg*4],byte            │ 080HEX, 00XXX100, 10reg101, 00000000,  byte                             
;     xxx byte ptr [ereg*4+dword],byte      │ 080HEX, 00XXX100, 10reg101,    dword,  byte
;     xxx byte ptr [ereg*8],byte            │ 080HEX, 00XXX100, 11reg101, 00000000,  byte                                
;     xxx byte ptr [ereg*8+dword],byte      │ 080HEX, 00XXX100, 11reg101,    dword,  byte
;
;     xxx byte ptr [erega+eregb*2],byte     │ 080HEX, 00XXX100, 01rgBrgA,  byte
;     xxx byte ptr [erega+eregb*4],byte     │ 080HEX, 00XXX100, 10rgBrgA,  byte
;     xxx byte ptr [erega+eregb*8],byte     │ 080HEX, 00XXX100, 11rgBrgA,  byte
;                                             
; 1e) xxx byte ptr [ebp],byte               │ 080HEX, 01XXX101, 00000000,  byte 
; 1f) xxx byte ptr [esp+byte],byte          │ 080HEX, 01XXX100,   024HEX,  byte
; 1g) xxx byte ptr [ereg+byte],byte         │ 080HEX, 01XXXreg,     byte,  byte
; 1h) xxx byte ptr [erega+eregb+byte],byte  │ 080HEX, 01XXX100, 00rgBrgA,  byte, byte 
;
; 1i) xxx byte ptr [ereg+dword],byte        │ 080HEX, 10XXXreg,    dword   byte,
; 1j) xxx byte ptr [ESP+dword],byte         │ 080HEX, 10XXX100,   024HEX, dword, byte
; 1k) xxx byte ptr [erega+eregb+dword],byte │ 080HEX, 10XXX100, 00rgBrgA, dword, byte
;
;  2) xxx reg,byte                          │ 080HEX, 11XXXreg, byte
;
; LEGEND: 000 - ADD, 001 - OR,  010 - ADC, 011 - SBB,
;         100 - AND, 101 - SUB, 110 - XOR, 111 - CMP
;
; Инструкции 1a-1k разбираются так же как и те, что начинаются с "0FFh"
; Различия лишь в размере (текущие инструкции на один байт больше)
;
 @080_disasm:   lodsb
                sub    ecx,ecx
 @080_disasm_test:
                inc    ecx
                cmp    al,11000000b
                jc     @0FF_disasm_test_x
;
;  2) xxx reg,byte                          │ 080HEX, 11XXXreg, byte
;
                add    cl,2
                jmp    disasm_ret