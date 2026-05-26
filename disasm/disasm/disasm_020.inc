;══════════════════════
; 1a) and byte ptr [EREG],reg              │ 020HEX, 00regERG
; 1b) and byte ptr [esp],reg               │ 020HEX, 00reg100,   024HEX
; 1c) and byte ptr [erega+eregb],reg       │ 020HEX, 00reg100, 00rgBrgA 
; 1d) and byte ptr [dword],reg             │ 020HEX, 00reg101,    dword
;
;     and byte ptr [ereg*2],reg            │ 020HEX, 00reg100, 01reg101, 00000000
;     and byte ptr [ereg*2+dword],reg      │ 020HEX, 00reg100, 01reg101,    dword
;     and byte ptr [ereg*4],reg            │ 020HEX, 00reg100, 10reg101, 00000000
;     and byte ptr [ereg*4+dword],reg      │ 020HEX, 00reg100, 10reg101,    dword
;     and byte ptr [ereg*8],reg            │ 020HEX, 00reg100, 11reg101, 00000000
;     and byte ptr [ereg*8+dword],reg      │ 020HEX, 00reg100, 11reg101,    dword
;
;     and byte ptr [erega+eregb*2],reg     │ 020HEX, 00reg100, 01rgBrgA
;     and byte ptr [erega+eregb*4],reg     │ 020HEX, 00reg100, 10rgBrgA
;     and byte ptr [erega+eregb*8],reg     │ 020HEX, 00reg100, 11rgBrgA
;
; 2a) and byte ptr [ebp],reg               │ 020HEX, 01reg101, 00000000 
; 2b) and byte ptr [esp+byte],reg          │ 020HEX, 01reg100,   024HEX, byte
; 2c) and byte ptr [EREG+byte],reg         │ 020HEX, 01regERG,     byte
; 2d) and byte ptr [erega+eregb+byte],reg  │ 020HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) and byte ptr [EREG+dword],reg        │ 020HEX, 10regERG,    dword
; 3b) and byte ptr [ESP+dword],reg         │ 020HEX, 10reg100,   024HEX, dword
; 3c) and byte ptr [erega+eregb+dword],reg │ 020HEX, 10reg100, 00rgBrgA, dword 
;
;  4) and rega, regb (8 bit)               │ 020HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @020_disasm:   jmp    @089_disasm