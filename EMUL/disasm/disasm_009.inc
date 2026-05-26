;══════════════════════
; 1a) or dword ptr [EREG],reg              │ 009HEX, 00regERG
; 1b) or dword ptr [esp],reg               │ 009HEX, 00reg100,   024HEX
; 1c) or dword ptr [erega+eregb],reg       │ 009HEX, 00reg100, 00rgBrgA 
; 1d) or dword ptr [dword],reg             │ 009HEX, 00reg101,    dword
;                                         
;     or dword ptr [ereg*2],reg            │ 009HEX, 00reg100, 01reg101, 00000000
;     or dword ptr [ereg*2+dword],reg      │ 009HEX, 00reg100, 01reg101,    dword
;     or dword ptr [ereg*4],reg            │ 009HEX, 00reg100, 10reg101, 00000000
;     or dword ptr [ereg*4+dword],reg      │ 009HEX, 00reg100, 10reg101,    dword
;     or dword ptr [ereg*8],reg            │ 009HEX, 00reg100, 11reg101, 00000000
;     or dword ptr [ereg*8+dword],reg      │ 009HEX, 00reg100, 11reg101,    dword
;
;     or dword ptr [erega+eregb*2],reg     │ 009HEX, 00reg100, 01rgBrgA
;     or dword ptr [erega+eregb*4],reg     │ 009HEX, 00reg100, 10rgBrgA
;     or dword ptr [erega+eregb*8],reg     │ 009HEX, 00reg100, 11rgBrgA
;
; 2a) or dword ptr [ebp],reg               │ 009HEX, 01reg101, 00000000 
; 2b) or dword ptr [esp+byte],reg          │ 009HEX, 01reg100,   024HEX, byte
; 2c) or dword ptr [EREG+byte],reg         │ 009HEX, 01regERG,     byte
; 2d) or dword ptr [erega+eregb+byte],reg  │ 009HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) or dword ptr [EREG+dword],reg        │ 009HEX, 10regERG,    dword
; 3b) or dword ptr [ESP+dword],reg         │ 009HEX, 10reg100,   024HEX, dword
; 3c) or dword ptr [erega+eregb+dword],reg │ 009HEX, 10reg100, 00rgBrgA, dword 
;
;  4) or erega, eregb                      │ 009HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @009_disasm:   jmp    @089_disasm