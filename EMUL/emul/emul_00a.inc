;───────────────────────
; or reg,byte ptr [EREG]               │ 00AHEX, 00regERG
; or reg,byte ptr [esp]                │ 00AHEX, 00reg100,   024HEX
; or reg,byte ptr [erega+eregb]        │ 00AHEX, 00reg100, 00rgBrgA 
; or reg,byte ptr [dword]              │ 00AHEX, 00reg101,    dword
;
; or reg,byte ptr [ereg*2]             │ 00AHEX, 00reg100, 01reg101, 00000000
; or reg,byte ptr [ereg*2+dword]       │ 00AHEX, 00reg100, 01reg101,    dword
; or reg,byte ptr [ereg*4]             │ 00AHEX, 00reg100, 10reg101, 00000000
; or reg,byte ptr [ereg*4+dword]       │ 00AHEX, 00reg100, 10reg101,    dword
; or reg,byte ptr [ereg*8]             │ 00AHEX, 00reg100, 11reg101, 00000000
; or reg,byte ptr [ereg*8+dword]       │ 00AHEX, 00reg100, 11reg101,    dword
;
; or reg,byte ptr [erega+eregb*2]      │ 00AHEX, 00reg100, 01rgBrgA
; or reg,byte ptr [erega+eregb*4]      │ 00AHEX, 00reg100, 10rgBrgA
; or reg,byte ptr [erega+eregb*8]      │ 00AHEX, 00reg100, 11rgBrgA
;
; or reg,byte ptr [ebp]                │ 00AHEX, 01reg101, 00000000 
; or reg,byte ptr [esp+byte]           │ 00AHEX, 01reg100,   024HEX, byte
; or reg,byte ptr [EREG+byte]          │ 00AHEX, 01regERG,     byte
; or reg,byte ptr [erega+eregb+byte]   │ 00AHEX, 01reg100, 00rgBrgA, byte 
;
; or reg,byte ptr [EREG+dword]         │ 00AHEX, 10regERG,    dword
; or reg,byte ptr [ESP+dword]          │ 00AHEX, 10reg100,   024HEX, dword
; or reg,byte ptr [erega+eregb+dword]  │ 00AHEX, 10reg100, 00rgBrgA, dword 
;
 emul_00A:      
                jmp    emul_08A