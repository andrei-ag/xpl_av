;───────────────────────
; adc reg,byte ptr [EREG]               │ 012HEX, 00regERG
; adc reg,byte ptr [esp]                │ 012HEX, 00reg100,   024HEX
; adc reg,byte ptr [erega+eregb]        │ 012HEX, 00reg100, 00rgBrgA 
; adc reg,byte ptr [dword]              │ 012HEX, 00reg101,    dword
;
; adc reg,byte ptr [ereg*2]             │ 012HEX, 00reg100, 01reg101, 00000000
; adc reg,byte ptr [ereg*2+dword]       │ 012HEX, 00reg100, 01reg101,    dword
; adc reg,byte ptr [ereg*4]             │ 012HEX, 00reg100, 10reg101, 00000000
; adc reg,byte ptr [ereg*4+dword]       │ 012HEX, 00reg100, 10reg101,    dword
; adc reg,byte ptr [ereg*8]             │ 012HEX, 00reg100, 11reg101, 00000000
; adc reg,byte ptr [ereg*8+dword]       │ 012HEX, 00reg100, 11reg101,    dword
;
; adc reg,byte ptr [erega+eregb*2]      │ 012HEX, 00reg100, 01rgBrgA
; adc reg,byte ptr [erega+eregb*4]      │ 012HEX, 00reg100, 10rgBrgA
; adc reg,byte ptr [erega+eregb*8]      │ 012HEX, 00reg100, 11rgBrgA
;
; adc reg,byte ptr [ebp]                │ 012HEX, 01reg101, 00000000 
; adc reg,byte ptr [esp+byte]           │ 012HEX, 01reg100,   024HEX, byte
; adc reg,byte ptr [EREG+byte]          │ 012HEX, 01regERG,     byte
; adc reg,byte ptr [erega+eregb+byte]   │ 012HEX, 01reg100, 00rgBrgA, byte 
;
; adc reg,byte ptr [EREG+dword]         │ 012HEX, 10regERG,    dword
; adc reg,byte ptr [ESP+dword]          │ 012HEX, 10reg100,   024HEX, dword
; adc reg,byte ptr [erega+eregb+dword]  │ 012HEX, 10reg100, 00rgBrgA, dword 
;
 emul_012:      
                jmp    emul_08A