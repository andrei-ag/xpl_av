;══════════════════════
; 1a) add dword ptr [EREG],reg              │ 001HEX, 00regERG
; 1b) add dword ptr [esp],reg               │ 001HEX, 00reg100,   024HEX
; 1c) add dword ptr [erega+eregb],reg       │ 001HEX, 00reg100, 00rgBrgA 
; 1d) add dword ptr [dword],reg             │ 001HEX, 00reg101,    dword
;
;     add dword ptr [ereg*2],reg            │ 001HEX, 00reg100, 01reg101, 00000000
;     add dword ptr [ereg*2+dword],reg      │ 001HEX, 00reg100, 01reg101,    dword
;     add dword ptr [ereg*4],reg            │ 001HEX, 00reg100, 10reg101, 00000000
;     add dword ptr [ereg*4+dword],reg      │ 001HEX, 00reg100, 10reg101,    dword
;     add dword ptr [ereg*8],reg            │ 001HEX, 00reg100, 11reg101, 00000000
;     add dword ptr [ereg*8+dword],reg      │ 001HEX, 00reg100, 11reg101,    dword
;
;     add dword ptr [erega+eregb*2],reg     │ 001HEX, 00reg100, 01rgBrgA
;     add dword ptr [erega+eregb*4],reg     │ 001HEX, 00reg100, 10rgBrgA
;     add dword ptr [erega+eregb*8],reg     │ 001HEX, 00reg100, 11rgBrgA
;
; 2a) add dword ptr [ebp],reg               │ 001HEX, 01reg101, 00000000 
; 2b) add dword ptr [esp+byte],reg          │ 001HEX, 01reg100,   024HEX, byte
; 2c) add dword ptr [EREG+byte],reg         │ 001HEX, 01regERG,     byte
; 2d) add dword ptr [erega+eregb+byte],reg  │ 001HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) add dword ptr [EREG+dword],reg        │ 001HEX, 10regERG,    dword
; 3b) add dword ptr [ESP+dword],reg         │ 001HEX, 10reg100,   024HEX, dword
; 3c) add dword ptr [erega+eregb+dword],reg │ 001HEX, 10reg100, 00rgBrgA, dword 
;
;  4) add erega, eregb                      │ 001HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @001_disasm:   jmp    @089_disasm 