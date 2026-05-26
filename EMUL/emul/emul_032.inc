;───────────────────────
; xor reg,byte ptr [EREG]               │ 032HEX, 00regERG
; xor reg,byte ptr [esp]                │ 032HEX, 00reg100,   024HEX
; xor reg,byte ptr [erega+eregb]        │ 032HEX, 00reg100, 00rgBrgA 
; xor reg,byte ptr [dword]              │ 032HEX, 00reg101,    dword
;
; xor reg,byte ptr [ereg*2]             │ 032HEX, 00reg100, 01reg101, 00000000
; xor reg,byte ptr [ereg*2+dword]       │ 032HEX, 00reg100, 01reg101,    dword
; xor reg,byte ptr [ereg*4]             │ 032HEX, 00reg100, 10reg101, 00000000
; xor reg,byte ptr [ereg*4+dword]       │ 032HEX, 00reg100, 10reg101,    dword
; xor reg,byte ptr [ereg*8]             │ 032HEX, 00reg100, 11reg101, 00000000
; xor reg,byte ptr [ereg*8+dword]       │ 032HEX, 00reg100, 11reg101,    dword
;
; xor reg,byte ptr [erega+eregb*2]      │ 032HEX, 00reg100, 01rgBrgA
; xor reg,byte ptr [erega+eregb*4]      │ 032HEX, 00reg100, 10rgBrgA
; xor reg,byte ptr [erega+eregb*8]      │ 032HEX, 00reg100, 11rgBrgA
;
; xor reg,byte ptr [ebp]                │ 032HEX, 01reg101, 00000000 
; xor reg,byte ptr [esp+byte]           │ 032HEX, 01reg100,   024HEX, byte
; xor reg,byte ptr [EREG+byte]          │ 032HEX, 01regERG,     byte
; xor reg,byte ptr [erega+eregb+byte]   │ 032HEX, 01reg100, 00rgBrgA, byte 
;
; xor reg,byte ptr [EREG+dword]         │ 032HEX, 10regERG,    dword
; xor reg,byte ptr [ESP+dword]          │ 032HEX, 10reg100,   024HEX, dword
; xor reg,byte ptr [erega+eregb+dword]  │ 032HEX, 10reg100, 00rgBrgA, dword 
;
 emul_032:      
                jmp    emul_08A