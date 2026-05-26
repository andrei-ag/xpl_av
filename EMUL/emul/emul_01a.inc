;───────────────────────
; sbb reg,byte ptr [EREG]               │ 01AHEX, 00regERG
; sbb reg,byte ptr [esp]                │ 01AHEX, 00reg100,   024HEX
; sbb reg,byte ptr [erega+eregb]        │ 01AHEX, 00reg100, 00rgBrgA 
; sbb reg,byte ptr [dword]              │ 01AHEX, 00reg101,    dword
;
; sbb reg,byte ptr [ereg*2]             │ 01AHEX, 00reg100, 01reg101, 00000000
; sbb reg,byte ptr [ereg*2+dword]       │ 01AHEX, 00reg100, 01reg101,    dword
; sbb reg,byte ptr [ereg*4]             │ 01AHEX, 00reg100, 10reg101, 00000000
; sbb reg,byte ptr [ereg*4+dword]       │ 01AHEX, 00reg100, 10reg101,    dword
; sbb reg,byte ptr [ereg*8]             │ 01AHEX, 00reg100, 11reg101, 00000000
; sbb reg,byte ptr [ereg*8+dword]       │ 01AHEX, 00reg100, 11reg101,    dword
;
; sbb reg,byte ptr [erega+eregb*2]      │ 01AHEX, 00reg100, 01rgBrgA
; sbb reg,byte ptr [erega+eregb*4]      │ 01AHEX, 00reg100, 10rgBrgA
; sbb reg,byte ptr [erega+eregb*8]      │ 01AHEX, 00reg100, 11rgBrgA
;
; sbb reg,byte ptr [ebp]                │ 01AHEX, 01reg101, 00000000 
; sbb reg,byte ptr [esp+byte]           │ 01AHEX, 01reg100,   024HEX, byte
; sbb reg,byte ptr [EREG+byte]          │ 01AHEX, 01regERG,     byte
; sbb reg,byte ptr [erega+eregb+byte]   │ 01AHEX, 01reg100, 00rgBrgA, byte 
;
; sbb reg,byte ptr [EREG+dword]         │ 01AHEX, 10regERG,    dword
; sbb reg,byte ptr [ESP+dword]          │ 01AHEX, 10reg100,   024HEX, dword
; sbb reg,byte ptr [erega+eregb+dword]  │ 01AHEX, 10reg100, 00rgBrgA, dword 
;
 emul_01A:      
                jmp    emul_08A