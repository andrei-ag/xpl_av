;───────────────────────
; adc eregb,dword ptr [erega]            │ 013HEX, 00rgBrgA
; adc ereg,dword ptr [esp]               │ 013HEX, 00reg100,   024HEX
; adc ereg,dword ptr [erega+eregb]       │ 013HEX, 00reg100, 00rgBrgA 
; adc ereg,dword ptr [dword]             │ 013HEX, 00reg101,    dword
;
; adc ereg,dword ptr [ereg*2]            │ 013HEX, 00reg100, 01reg101, 00000000
; adc ereg,dword ptr [ereg*2+dword]      │ 013HEX, 00reg100, 01reg101,    dword
; adc ereg,dword ptr [ereg*4]            │ 013HEX, 00reg100, 10reg101, 00000000
; adc ereg,dword ptr [ereg*4+dword]      │ 013HEX, 00reg100, 10reg101,    dword
; adc ereg,dword ptr [ereg*8]            │ 013HEX, 00reg100, 11reg101, 00000000
; adc ereg,dword ptr [ereg*8+dword]      │ 013HEX, 00reg100, 11reg101,    dword
;
; adc ereg,dword ptr [erega+eregb*2]     │ 013HEX, 00reg100, 01rgBrgA
; adc ereg,dword ptr [erega+eregb*4]     │ 013HEX, 00reg100, 10rgBrgA
; adc ereg,dword ptr [erega+eregb*8]     │ 013HEX, 00reg100, 11rgBrgA
;
; adc ereg,dword ptr [ebp]               │ 013HEX, 01reg101, 00000000 
; adc ereg,dword ptr [esp+byte]          │ 013HEX, 01reg100,   024HEX, byte
; adc eregb,dword ptr [erega+byte]       │ 013HEX, 01rgBrgA,     byte
; adc ereg,dword ptr [erega+eregb+byte]  │ 013HEX, 01reg100, 00rgBrgA, byte 
;
; adc eregb,dword ptr [erega+dword]      │ 013HEX, 10rgBrgA,    dword
; adc ereg,dword ptr [ESP+dword]         │ 013HEX, 10reg100,   024HEX, dword
; adc ereg,dword ptr [erega+eregb+dword] │ 013HEX, 10reg100, 00rgBrgA, dword 
;
 emul_013:      
                jmp     emul_08A