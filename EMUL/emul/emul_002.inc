;───────────────────────
; add reg,byte ptr [EREG]               │ 002HEX, 00regERG
; add reg,byte ptr [esp]                │ 002HEX, 00reg100,   024HEX
; add reg,byte ptr [erega+eregb]        │ 002HEX, 00reg100, 00rgBrgA 
; add reg,byte ptr [dword]              │ 002HEX, 00reg101,    dword
;
; add reg,byte ptr [ereg*2]             │ 002HEX, 00reg100, 01reg101, 00000000
; add reg,byte ptr [ereg*2+dword]       │ 002HEX, 00reg100, 01reg101,    dword
; add reg,byte ptr [ereg*4]             │ 002HEX, 00reg100, 10reg101, 00000000
; add reg,byte ptr [ereg*4+dword]       │ 002HEX, 00reg100, 10reg101,    dword
; add reg,byte ptr [ereg*8]             │ 002HEX, 00reg100, 11reg101, 00000000
; add reg,byte ptr [ereg*8+dword]       │ 002HEX, 00reg100, 11reg101,    dword
;
; add reg,byte ptr [erega+eregb*2]      │ 002HEX, 00reg100, 01rgBrgA
; add reg,byte ptr [erega+eregb*4]      │ 002HEX, 00reg100, 10rgBrgA
; add reg,byte ptr [erega+eregb*8]      │ 002HEX, 00reg100, 11rgBrgA
;
; add reg,byte ptr [ebp]                │ 002HEX, 01reg101, 00000000 
; add reg,byte ptr [esp+byte]           │ 002HEX, 01reg100,   024HEX, byte
; add reg,byte ptr [EREG+byte]          │ 002HEX, 01regERG,     byte
; add reg,byte ptr [erega+eregb+byte]   │ 002HEX, 01reg100, 00rgBrgA, byte 
;
; add reg,byte ptr [EREG+dword]         │ 002HEX, 10regERG,    dword
; add reg,byte ptr [ESP+dword]          │ 002HEX, 10reg100,   024HEX, dword
; add reg,byte ptr [erega+eregb+dword]  │ 002HEX, 10reg100, 00rgBrgA, dword 
;
 emul_002:      
                jmp    emul_08A