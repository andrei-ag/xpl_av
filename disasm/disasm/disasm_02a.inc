;══════════════════════
; 1a) sub reg,byte ptr [EREG]               │ 02AHEX, 00regERG
; 1b) sub reg,byte ptr [esp]                │ 02AHEX, 00reg100,   024HEX
; 1c) sub reg,byte ptr [erega+eregb]        │ 02AHEX, 00reg100, 00rgBrgA 
; 1d) sub reg,byte ptr [dword]              │ 02AHEX, 00reg101,    dword
;
;     sub reg,byte ptr [ereg*2]             │ 02AHEX, 00reg100, 01reg101, 00000000
;     sub reg,byte ptr [ereg*2+dword]       │ 02AHEX, 00reg100, 01reg101,    dword
;     sub reg,byte ptr [ereg*4]             │ 02AHEX, 00reg100, 10reg101, 00000000
;     sub reg,byte ptr [ereg*4+dword]       │ 02AHEX, 00reg100, 10reg101,    dword
;     sub reg,byte ptr [ereg*8]             │ 02AHEX, 00reg100, 11reg101, 00000000
;     sub reg,byte ptr [ereg*8+dword]       │ 02AHEX, 00reg100, 11reg101,    dword
;
;     sub reg,byte ptr [erega+eregb*2]      │ 02AHEX, 00reg100, 01rgBrgA
;     sub reg,byte ptr [erega+eregb*4]      │ 02AHEX, 00reg100, 10rgBrgA
;     sub reg,byte ptr [erega+eregb*8]      │ 02AHEX, 00reg100, 11rgBrgA
;
; 2a) sub reg,byte ptr [ebp]                │ 02AHEX, 01reg101, 00000000 
; 2b) sub reg,byte ptr [esp+byte]           │ 02AHEX, 01reg100,   024HEX, byte
; 2c) sub reg,byte ptr [EREG+byte]          │ 02AHEX, 01regERG,     byte
; 2d) sub reg,byte ptr [erega+eregb+byte]   │ 02AHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sub reg,byte ptr [EREG+dword]         │ 02AHEX, 10regERG,    dword
; 3b) sub reg,byte ptr [ESP+dword]          │ 02AHEX, 10reg100,   024HEX, dword
; 3c) sub reg,byte ptr [erega+eregb+dword]  │ 02AHEX, 10reg100, 00rgBrgA, dword 
;
;  4) sub rega, regb (8 bit)                │ 02AHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @02A_disasm:   jmp    @089_disasm