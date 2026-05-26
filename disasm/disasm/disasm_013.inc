;══════════════════════
; 1a) adc eregb,dword ptr [erega]            │ 013HEX, 00rgBrgA
; 1b) adc ereg,dword ptr [esp]               │ 013HEX, 00reg100,   024HEX
; 1c) adc ereg,dword ptr [erega+eregb]       │ 013HEX, 00reg100, 00rgBrgA 
; 1d) adc ereg,dword ptr [dword]             │ 013HEX, 00reg101,    dword
;
;     adc ereg,dword ptr [ereg*2]            │ 013HEX, 00reg100, 01reg101, 00000000
;     adc ereg,dword ptr [ereg*2+dword]      │ 013HEX, 00reg100, 01reg101,    dword
;     adc ereg,dword ptr [ereg*4]            │ 013HEX, 00reg100, 10reg101, 00000000
;     adc ereg,dword ptr [ereg*4+dword]      │ 013HEX, 00reg100, 10reg101,    dword
;     adc ereg,dword ptr [ereg*8]            │ 013HEX, 00reg100, 11reg101, 00000000
;     adc ereg,dword ptr [ereg*8+dword]      │ 013HEX, 00reg100, 11reg101,    dword
;
;     adc ereg,dword ptr [erega+eregb*2]     │ 013HEX, 00reg100, 01rgBrgA
;     adc ereg,dword ptr [erega+eregb*4]     │ 013HEX, 00reg100, 10rgBrgA
;     adc ereg,dword ptr [erega+eregb*8]     │ 013HEX, 00reg100, 11rgBrgA
;
; 2a) adc ereg,dword ptr [ebp]               │ 013HEX, 01reg101, 00000000 
; 2b) adc ereg,dword ptr [esp+byte]          │ 013HEX, 01reg100,   024HEX, byte
; 2c) adc eregb,dword ptr [erega+byte]       │ 013HEX, 01rgBrgA,     byte
; 2d) adc ereg,dword ptr [erega+eregb+byte]  │ 013HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) adc eregb,dword ptr [erega+dword]      │ 013HEX, 10rgBrgA,    dword
; 3b) adc ereg,dword ptr [ESP+dword]         │ 013HEX, 10reg100,   024HEX, dword
; 3c) adc ereg,dword ptr [erega+eregb+dword] │ 013HEX, 10reg100, 00rgBrgA, dword 
;
;  4) adc erega, eregb                       │ 013HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @013_disasm:   jmp    @089_disasm