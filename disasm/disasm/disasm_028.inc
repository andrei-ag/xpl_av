;══════════════════════
; 1a) sub byte ptr [EREG],reg              │ 028HEX, 00regERG
; 1b) sub byte ptr [esp],reg               │ 028HEX, 00reg100,   024HEX
; 1c) sub byte ptr [erega+eregb],reg       │ 028HEX, 00reg100, 00rgBrgA 
; 1d) sub byte ptr [dword],reg             │ 028HEX, 00reg101,    dword
;
;     sub byte ptr [ereg*2],reg            │ 028HEX, 00reg100, 01reg101, 00000000
;     sub byte ptr [ereg*2+dword],reg      │ 028HEX, 00reg100, 01reg101,    dword
;     sub byte ptr [ereg*4],reg            │ 028HEX, 00reg100, 10reg101, 00000000
;     sub byte ptr [ereg*4+dword],reg      │ 028HEX, 00reg100, 10reg101,    dword
;     sub byte ptr [ereg*8],reg            │ 028HEX, 00reg100, 11reg101, 00000000
;     sub byte ptr [ereg*8+dword],reg      │ 028HEX, 00reg100, 11reg101,    dword
;
;     sub byte ptr [erega+eregb*2],reg     │ 028HEX, 00reg100, 01rgBrgA
;     sub byte ptr [erega+eregb*4],reg     │ 028HEX, 00reg100, 10rgBrgA
;     sub byte ptr [erega+eregb*8],reg     │ 028HEX, 00reg100, 11rgBrgA
;
; 2a) sub byte ptr [ebp],reg               │ 028HEX, 01reg101, 00000000 
; 2b) sub byte ptr [esp+byte],reg          │ 028HEX, 01reg100,   024HEX, byte
; 2c) sub byte ptr [EREG+byte],reg         │ 028HEX, 01regERG,     byte
; 2d) sub byte ptr [erega+eregb+byte],reg  │ 028HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sub byte ptr [EREG+dword],reg        │ 028HEX, 10regERG,    dword
; 3b) sub byte ptr [ESP+dword],reg         │ 028HEX, 10reg100,   024HEX, dword
; 3c) sub byte ptr [erega+eregb+dword],reg │ 028HEX, 10reg100, 00rgBrgA, dword 
;
;  4) sub rega, regb (8 bit)               │ 028HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @028_disasm:   jmp    @089_disasm