;══════════════════════
; 1a) add byte ptr [EREG],reg              │ 000HEX, 00regERG
; 1b) add byte ptr [esp],reg               │ 000HEX, 00reg100,   024HEX
; 1c) add byte ptr [erega+eregb],reg       │ 000HEX, 00reg100, 00rgBrgA 
; 1d) add byte ptr [dword],reg             │ 000HEX, 00reg101,    dword
;
;     add byte ptr [ereg*2],reg            │ 000HEX, 00reg100, 01reg101, 00000000
;     add byte ptr [ereg*2+dword],reg      │ 000HEX, 00reg100, 01reg101,    dword
;     add byte ptr [ereg*4],reg            │ 000HEX, 00reg100, 10reg101, 00000000
;     add byte ptr [ereg*4+dword],reg      │ 000HEX, 00reg100, 10reg101,    dword
;     add byte ptr [ereg*8],reg            │ 000HEX, 00reg100, 11reg101, 00000000
;     add byte ptr [ereg*8+dword],reg      │ 000HEX, 00reg100, 11reg101,    dword
;
;     add byte ptr [erega+eregb*2],reg     │ 000HEX, 00reg100, 01rgBrgA
;     add byte ptr [erega+eregb*4],reg     │ 000HEX, 00reg100, 10rgBrgA
;     add byte ptr [erega+eregb*8],reg     │ 000HEX, 00reg100, 11rgBrgA
;
; 2a) add byte ptr [ebp],reg               │ 000HEX, 01reg101, 00000000 
; 2b) add byte ptr [esp+byte],reg          │ 000HEX, 01reg100,   024HEX, byte
; 2c) add byte ptr [EREG+byte],reg         │ 000HEX, 01regERG,     byte
; 2d) add byte ptr [erega+eregb+byte],reg  │ 000HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) add byte ptr [EREG+dword],reg        │ 000HEX, 10regERG,    dword
; 3b) add byte ptr [ESP+dword],reg         │ 000HEX, 10reg100,   024HEX, dword
; 3c) add byte ptr [erega+eregb+dword],reg │ 000HEX, 10reg100, 00rgBrgA, dword 
;
;  4) add rega, regb (8 bit)               │ 000HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @000_disasm:   jmp    @089_disasm 