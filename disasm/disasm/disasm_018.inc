;══════════════════════
; 1a) sbb byte ptr [EREG],reg              │ 018HEX, 00regERG
; 1b) sbb byte ptr [esp],reg               │ 018HEX, 00reg100,   024HEX
; 1c) sbb byte ptr [erega+eregb],reg       │ 018HEX, 00reg100, 00rgBrgA 
; 1d) sbb byte ptr [dword],reg             │ 018HEX, 00reg101,    dword
;
;     sbb byte ptr [ereg*2],reg            │ 018HEX, 00reg100, 01reg101, 00000000
;     sbb byte ptr [ereg*2+dword],reg      │ 018HEX, 00reg100, 01reg101,    dword
;     sbb byte ptr [ereg*4],reg            │ 018HEX, 00reg100, 10reg101, 00000000
;     sbb byte ptr [ereg*4+dword],reg      │ 018HEX, 00reg100, 10reg101,    dword
;     sbb byte ptr [ereg*8],reg            │ 018HEX, 00reg100, 11reg101, 00000000
;     sbb byte ptr [ereg*8+dword],reg      │ 018HEX, 00reg100, 11reg101,    dword
;
;     sbb byte ptr [erega+eregb*2],reg     │ 018HEX, 00reg100, 01rgBrgA
;     sbb byte ptr [erega+eregb*4],reg     │ 018HEX, 00reg100, 10rgBrgA
;     sbb byte ptr [erega+eregb*8],reg     │ 018HEX, 00reg100, 11rgBrgA
;
; 2a) sbb byte ptr [ebp],reg               │ 018HEX, 01reg101, 00000000 
; 2b) sbb byte ptr [esp+byte],reg          │ 018HEX, 01reg100,   024HEX, byte
; 2c) sbb byte ptr [EREG+byte],reg         │ 018HEX, 01regERG,     byte
; 2d) sbb byte ptr [erega+eregb+byte],reg  │ 018HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sbb byte ptr [EREG+dword],reg        │ 018HEX, 10regERG,    dword
; 3b) sbb byte ptr [ESP+dword],reg         │ 018HEX, 10reg100,   024HEX, dword
; 3c) sbb byte ptr [erega+eregb+dword],reg │ 018HEX, 10reg100, 00rgBrgA, dword 
;
;  4) sbb rega, regb (8 bit)               │ 018HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @018_disasm:   jmp    @089_disasm