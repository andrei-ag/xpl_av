;══════════════════════
; 1a) adc dword ptr [EREG],reg              │ 011HEX, 00regERG
; 1b) adc dword ptr [esp],reg               │ 011HEX, 00reg100,   024HEX
; 1c) adc dword ptr [erega+eregb],reg       │ 011HEX, 00reg100, 00rgBrgA 
; 1d) adc dword ptr [dword],reg             │ 011HEX, 00reg101,    dword
;
;     adc dword ptr [ereg*2],reg            │ 011HEX, 00reg100, 01reg101, 00000000
;     adc dword ptr [ereg*2+dword],reg      │ 011HEX, 00reg100, 01reg101,    dword
;     adc dword ptr [ereg*4],reg            │ 011HEX, 00reg100, 10reg101, 00000000
;     adc dword ptr [ereg*4+dword],reg      │ 011HEX, 00reg100, 10reg101,    dword
;     adc dword ptr [ereg*8],reg            │ 011HEX, 00reg100, 11reg101, 00000000
;     adc dword ptr [ereg*8+dword],reg      │ 011HEX, 00reg100, 11reg101,    dword
;
;     adc dword ptr [erega+eregb*2],reg     │ 011HEX, 00reg100, 01rgBrgA
;     adc dword ptr [erega+eregb*4],reg     │ 011HEX, 00reg100, 10rgBrgA
;     adc dword ptr [erega+eregb*8],reg     │ 011HEX, 00reg100, 11rgBrgA
;
; 2a) adc dword ptr [ebp],reg               │ 011HEX, 01reg101, 00000000 
; 2b) adc dword ptr [esp+byte],reg          │ 011HEX, 01reg100,   024HEX, byte
; 2c) adc dword ptr [EREG+byte],reg         │ 011HEX, 01regERG,     byte
; 2d) adc dword ptr [erega+eregb+byte],reg  │ 011HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) adc dword ptr [EREG+dword],reg        │ 011HEX, 10regERG,    dword
; 3b) adc dword ptr [ESP+dword],reg         │ 011HEX, 10reg100,   024HEX, dword
; 3c) adc dword ptr [erega+eregb+dword],reg │ 011HEX, 10reg100, 00rgBrgA, dword 
;
;  4) adc erega, eregb                      │ 011HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @011_disasm:   jmp    @089_disasm 