;───────────────────────
; sub reg,byte ptr [EREG]               │ 02AHEX, 00regERG
; sub reg,byte ptr [esp]                │ 02AHEX, 00reg100,   024HEX
; sub reg,byte ptr [erega+eregb]        │ 02AHEX, 00reg100, 00rgBrgA 
; sub reg,byte ptr [dword]              │ 02AHEX, 00reg101,    dword
;
; sub reg,byte ptr [ereg*2]             │ 02AHEX, 00reg100, 01reg101, 00000000
; sub reg,byte ptr [ereg*2+dword]       │ 02AHEX, 00reg100, 01reg101,    dword
; sub reg,byte ptr [ereg*4]             │ 02AHEX, 00reg100, 10reg101, 00000000
; sub reg,byte ptr [ereg*4+dword]       │ 02AHEX, 00reg100, 10reg101,    dword
; sub reg,byte ptr [ereg*8]             │ 02AHEX, 00reg100, 11reg101, 00000000
; sub reg,byte ptr [ereg*8+dword]       │ 02AHEX, 00reg100, 11reg101,    dword
;
; sub reg,byte ptr [erega+eregb*2]      │ 02AHEX, 00reg100, 01rgBrgA
; sub reg,byte ptr [erega+eregb*4]      │ 02AHEX, 00reg100, 10rgBrgA
; sub reg,byte ptr [erega+eregb*8]      │ 02AHEX, 00reg100, 11rgBrgA
;
; sub reg,byte ptr [ebp]                │ 02AHEX, 01reg101, 00000000 
; sub reg,byte ptr [esp+byte]           │ 02AHEX, 01reg100,   024HEX, byte
; sub reg,byte ptr [EREG+byte]          │ 02AHEX, 01regERG,     byte
; sub reg,byte ptr [erega+eregb+byte]   │ 02AHEX, 01reg100, 00rgBrgA, byte 
;
; sub reg,byte ptr [EREG+dword]         │ 02AHEX, 10regERG,    dword
; sub reg,byte ptr [ESP+dword]          │ 02AHEX, 10reg100,   024HEX, dword
; sub reg,byte ptr [erega+eregb+dword]  │ 02AHEX, 10reg100, 00rgBrgA, dword 
;
 emul_02A:      
                jmp    emul_08A