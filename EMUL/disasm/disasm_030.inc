;══════════════════════
; 1a) xor byte ptr [EREG],reg              │ 030HEX, 00regERG
; 1b) xor byte ptr [esp],reg               │ 030HEX, 00reg100,   024HEX
; 1c) xor byte ptr [erega+eregb],reg       │ 030HEX, 00reg100, 00rgBrgA 
; 1d) xor byte ptr [dword],reg             │ 030HEX, 00reg101,    dword
;
;     xor byte ptr [ereg*2],reg            │ 030HEX, 00reg100, 01reg101, 00000000
;     xor byte ptr [ereg*2+dword],reg      │ 030HEX, 00reg100, 01reg101,    dword
;     xor byte ptr [ereg*4],reg            │ 030HEX, 00reg100, 10reg101, 00000000
;     xor byte ptr [ereg*4+dword],reg      │ 030HEX, 00reg100, 10reg101,    dword
;     xor byte ptr [ereg*8],reg            │ 030HEX, 00reg100, 11reg101, 00000000
;     xor byte ptr [ereg*8+dword],reg      │ 030HEX, 00reg100, 11reg101,    dword
;
;     xor byte ptr [erega+eregb*2],reg     │ 030HEX, 00reg100, 01rgBrgA
;     xor byte ptr [erega+eregb*4],reg     │ 030HEX, 00reg100, 10rgBrgA
;     xor byte ptr [erega+eregb*8],reg     │ 030HEX, 00reg100, 11rgBrgA
;
; 2a) xor byte ptr [ebp],reg               │ 030HEX, 01reg101, 00000000 
; 2b) xor byte ptr [esp+byte],reg          │ 030HEX, 01reg100,   024HEX, byte
; 2c) xor byte ptr [EREG+byte],reg         │ 030HEX, 01regERG,     byte
; 2d) xor byte ptr [erega+eregb+byte],reg  │ 030HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) xor byte ptr [EREG+dword],reg        │ 030HEX, 10regERG,    dword
; 3b) xor byte ptr [ESP+dword],reg         │ 030HEX, 10reg100,   024HEX, dword
; 3c) xor byte ptr [erega+eregb+dword],reg │ 030HEX, 10reg100, 00rgBrgA, dword 
;
;  4) xor rega, regb (8 bit)               │ 030HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @030_disasm:   jmp    @089_disasm