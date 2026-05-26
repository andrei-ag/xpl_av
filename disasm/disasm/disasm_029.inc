;══════════════════════
; 1a) sub dword ptr [EREG],reg              │ 029HEX, 00regERG
; 1b) sub dword ptr [esp],reg               │ 029HEX, 00reg100,   024HEX
; 1c) sub dword ptr [erega+eregb],reg       │ 029HEX, 00reg100, 00rgBrgA 
; 1d) sub dword ptr [dword],reg             │ 029HEX, 00reg101,    dword
;                                         
;     sub dword ptr [ereg*2],reg            │ 029HEX, 00reg100, 01reg101, 00000000
;     sub dword ptr [ereg*2+dword],reg      │ 029HEX, 00reg100, 01reg101,    dword
;     sub dword ptr [ereg*4],reg            │ 029HEX, 00reg100, 10reg101, 00000000
;     sub dword ptr [ereg*4+dword],reg      │ 029HEX, 00reg100, 10reg101,    dword
;     sub dword ptr [ereg*8],reg            │ 029HEX, 00reg100, 11reg101, 00000000
;     sub dword ptr [ereg*8+dword],reg      │ 029HEX, 00reg100, 11reg101,    dword
;
;     sub dword ptr [erega+eregb*2],reg     │ 029HEX, 00reg100, 01rgBrgA
;     sub dword ptr [erega+eregb*4],reg     │ 029HEX, 00reg100, 10rgBrgA
;     sub dword ptr [erega+eregb*8],reg     │ 029HEX, 00reg100, 11rgBrgA
;
; 2a) sub dword ptr [ebp],reg               │ 029HEX, 01reg101, 00000000 
; 2b) sub dword ptr [esp+byte],reg          │ 029HEX, 01reg100,   024HEX, byte
; 2c) sub dword ptr [EREG+byte],reg         │ 029HEX, 01regERG,     byte
; 2d) sub dword ptr [erega+eregb+byte],reg  │ 029HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sub dword ptr [EREG+dword],reg        │ 029HEX, 10regERG,    dword
; 3b) sub dword ptr [ESP+dword],reg         │ 029HEX, 10reg100,   024HEX, dword
; 3c) sub dword ptr [erega+eregb+dword],reg │ 029HEX, 10reg100, 00rgBrgA, dword 
;
;  4) sub erega, eregb                      │ 029HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @029_disasm:   jmp    @089_disasm