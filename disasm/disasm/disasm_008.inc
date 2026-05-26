;══════════════════════
; 1a) or byte ptr [EREG],reg               │ 008HEX, 00regERG
; 1b) or byte ptr [esp],reg                │ 008HEX, 00reg100,   024HEX
; 1c) or byte ptr [erega+eregb],reg        │ 008HEX, 00reg100, 00rgBrgA 
; 1d) or byte ptr [dword],reg              │ 008HEX, 00reg101,    dword
;
;     or byte ptr [ereg*2],reg             │ 008HEX, 00reg100, 01reg101, 00000000
;     or byte ptr [ereg*2+dword],reg       │ 008HEX, 00reg100, 01reg101,    dword
;     or byte ptr [ereg*4],reg             │ 008HEX, 00reg100, 10reg101, 00000000
;     or byte ptr [ereg*4+dword],reg       │ 008HEX, 00reg100, 10reg101,    dword
;     or byte ptr [ereg*8],reg             │ 008HEX, 00reg100, 11reg101, 00000000
;     or byte ptr [ereg*8+dword],reg       │ 008HEX, 00reg100, 11reg101,    dword
;
;     or byte ptr [erega+eregb*2],reg      │ 008HEX, 00reg100, 01rgBrgA
;     or byte ptr [erega+eregb*4],reg      │ 008HEX, 00reg100, 10rgBrgA
;     or byte ptr [erega+eregb*8],reg      │ 008HEX, 00reg100, 11rgBrgA
;
; 2a) or byte ptr [ebp],reg                │ 008HEX, 01reg101, 00000000 
; 2b) or byte ptr [esp+byte],reg           │ 008HEX, 01reg100,   024HEX, byte
; 2c) or byte ptr [EREG+byte],reg          │ 008HEX, 01regERG,     byte
; 2d) or byte ptr [erega+eregb+byte],reg   │ 008HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) or byte ptr [EREG+dword],reg         │ 008HEX, 10regERG,    dword
; 3b) or byte ptr [ESP+dword],reg          │ 008HEX, 10reg100,   024HEX, dword
; 3c) or byte ptr [erega+eregb+dword],reg  │ 008HEX, 10reg100, 00rgBrgA, dword 
;
;  4) or rega, regb (8 bit)                │ 008HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @008_disasm:   jmp    @089_disasm