;══════════════════════
; 1a) and dword ptr [EREG],reg              │ 021HEX, 00regERG
; 1b) and dword ptr [esp],reg               │ 021HEX, 00reg100,   024HEX
; 1c) and dword ptr [erega+eregb],reg       │ 021HEX, 00reg100, 00rgBrgA 
; 1d) and dword ptr [dword],reg             │ 021HEX, 00reg101,    dword
;                                         
;     and dword ptr [ereg*2],reg            │ 021HEX, 00reg100, 01reg101, 00000000
;     and dword ptr [ereg*2+dword],reg      │ 021HEX, 00reg100, 01reg101,    dword
;     and dword ptr [ereg*4],reg            │ 021HEX, 00reg100, 10reg101, 00000000
;     and dword ptr [ereg*4+dword],reg      │ 021HEX, 00reg100, 10reg101,    dword
;     and dword ptr [ereg*8],reg            │ 021HEX, 00reg100, 11reg101, 00000000
;     and dword ptr [ereg*8+dword],reg      │ 021HEX, 00reg100, 11reg101,    dword
;
;     and dword ptr [erega+eregb*2],reg     │ 021HEX, 00reg100, 01rgBrgA
;     and dword ptr [erega+eregb*4],reg     │ 021HEX, 00reg100, 10rgBrgA
;     and dword ptr [erega+eregb*8],reg     │ 021HEX, 00reg100, 11rgBrgA
;
; 2a) and dword ptr [ebp],reg               │ 021HEX, 01reg101, 00000000 
; 2b) and dword ptr [esp+byte],reg          │ 021HEX, 01reg100,   024HEX, byte
; 2c) and dword ptr [EREG+byte],reg         │ 021HEX, 01regERG,     byte
; 2d) and dword ptr [erega+eregb+byte],reg  │ 021HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) and dword ptr [EREG+dword],reg        │ 021HEX, 10regERG,    dword
; 3b) and dword ptr [ESP+dword],reg         │ 021HEX, 10reg100,   024HEX, dword
; 3c) and dword ptr [erega+eregb+dword],reg │ 021HEX, 10reg100, 00rgBrgA, dword 
;
;  4) and erega, eregb                      │ 021HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @021_disasm:   jmp    @089_disasm