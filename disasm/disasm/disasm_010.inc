;══════════════════════
; 1a) adc byte ptr [EREG],reg              │ 010HEX, 00regERG
; 1b) adc byte ptr [esp],reg               │ 010HEX, 00reg100,   024HEX
; 1c) adc byte ptr [erega+eregb],reg       │ 010HEX, 00reg100, 00rgBrgA 
; 1d) adc byte ptr [dword],reg             │ 010HEX, 00reg101,    dword
;
;     adc byte ptr [ereg*2],reg            │ 010HEX, 00reg100, 01reg101, 00000000
;     adc byte ptr [ereg*2+dword],reg      │ 010HEX, 00reg100, 01reg101,    dword
;     adc byte ptr [ereg*4],reg            │ 010HEX, 00reg100, 10reg101, 00000000
;     adc byte ptr [ereg*4+dword],reg      │ 010HEX, 00reg100, 10reg101,    dword
;     adc byte ptr [ereg*8],reg            │ 010HEX, 00reg100, 11reg101, 00000000
;     adc byte ptr [ereg*8+dword],reg      │ 010HEX, 00reg100, 11reg101,    dword
;
;     adc byte ptr [erega+eregb*2],reg     │ 010HEX, 00reg100, 01rgBrgA
;     adc byte ptr [erega+eregb*4],reg     │ 010HEX, 00reg100, 10rgBrgA
;     adc byte ptr [erega+eregb*8],reg     │ 010HEX, 00reg100, 11rgBrgA
;
; 2a) adc byte ptr [ebp],reg               │ 010HEX, 01reg101, 00000000 
; 2b) adc byte ptr [esp+byte],reg          │ 010HEX, 01reg100,   024HEX, byte
; 2c) adc byte ptr [EREG+byte],reg         │ 010HEX, 01regERG,     byte
; 2d) adc byte ptr [erega+eregb+byte],reg  │ 010HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) adc byte ptr [EREG+dword],reg        │ 010HEX, 10regERG,    dword
; 3b) adc byte ptr [ESP+dword],reg         │ 010HEX, 10reg100,   024HEX, dword
; 3c) adc byte ptr [erega+eregb+dword],reg │ 010HEX, 10reg100, 00rgBrgA, dword 
;
;  4) adc rega, regb (8 bit)               │ 010HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @010_disasm:   jmp    @089_disasm