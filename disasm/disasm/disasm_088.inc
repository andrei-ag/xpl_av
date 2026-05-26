;══════════════════════
; 1a) mov byte ptr [EREG],reg              │ 088HEX, 00regERG
; 1b) mov byte ptr [esp],reg               │ 088HEX, 00reg100,   024HEX
; 1c) mov byte ptr [erega+eregb],reg       │ 088HEX, 00reg100, 00rgBrgA 
; 1d) mov byte ptr [dword],reg             │ 088HEX, 00reg101,    dword
;
;     mov byte ptr [ereg*2],reg            │ 088HEX, 00reg100, 01reg101, 00000000
;     mov byte ptr [ereg*2+dword],reg      │ 088HEX, 00reg100, 01reg101,    dword
;     mov byte ptr [ereg*4],reg            │ 088HEX, 00reg100, 10reg101, 00000000
;     mov byte ptr [ereg*4+dword],reg      │ 088HEX, 00reg100, 10reg101,    dword
;     mov byte ptr [ereg*8],reg            │ 088HEX, 00reg100, 11reg101, 00000000
;     mov byte ptr [ereg*8+dword],reg      │ 088HEX, 00reg100, 11reg101,    dword
;
;     mov byte ptr [erega+eregb*2],reg     │ 088HEX, 00reg100, 01rgBrgA
;     mov byte ptr [erega+eregb*4],reg     │ 088HEX, 00reg100, 10rgBrgA
;     mov byte ptr [erega+eregb*8],reg     │ 088HEX, 00reg100, 11rgBrgA
;
; 2a) mov byte ptr [ebp],reg               │ 088HEX, 01reg101, 00000000 
; 2b) mov byte ptr [esp+byte],reg          │ 088HEX, 01reg100,   024HEX, byte
; 2c) mov byte ptr [EREG+byte],reg         │ 088HEX, 01regERG,     byte
; 2d) mov byte ptr [erega+eregb+byte],reg  │ 088HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) mov byte ptr [EREG+dword],reg        │ 088HEX, 10regERG,    dword
; 3b) mov byte ptr [ESP+dword],reg         │ 088HEX, 10reg100,   024HEX, dword
; 3c) mov byte ptr [erega+eregb+dword],reg │ 088HEX, 10reg100, 00rgBrgA, dword 
;
;  4) mov rega, regb (8 bit)               │ 088HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @088_disasm:   jmp    @089_disasm