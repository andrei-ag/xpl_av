;══════════════════════
; 1a) mov reg,byte ptr [EREG]               │ 08AHEX, 00regERG
; 1b) mov reg,byte ptr [esp]                │ 08AHEX, 00reg100,   024HEX
; 1c) mov reg,byte ptr [erega+eregb]        │ 08AHEX, 00reg100, 00rgBrgA 
; 1d) mov reg,byte ptr [dword]              │ 08AHEX, 00reg101,    dword
;
;     mov reg,byte ptr [ereg*2]             │ 08AHEX, 00reg100, 01reg101, 00000000
;     mov reg,byte ptr [ereg*2+dword]       │ 08AHEX, 00reg100, 01reg101,    dword
;     mov reg,byte ptr [ereg*4]             │ 08AHEX, 00reg100, 10reg101, 00000000
;     mov reg,byte ptr [ereg*4+dword]       │ 08AHEX, 00reg100, 10reg101,    dword
;     mov reg,byte ptr [ereg*8]             │ 08AHEX, 00reg100, 11reg101, 00000000
;     mov reg,byte ptr [ereg*8+dword]       │ 08AHEX, 00reg100, 11reg101,    dword
;
;     mov reg,byte ptr [erega+eregb*2]      │ 08AHEX, 00reg100, 01rgBrgA
;     mov reg,byte ptr [erega+eregb*4]      │ 08AHEX, 00reg100, 10rgBrgA
;     mov reg,byte ptr [erega+eregb*8]      │ 08AHEX, 00reg100, 11rgBrgA
;
; 2a) mov reg,byte ptr [ebp]                │ 08AHEX, 01reg101, 00000000 
; 2b) mov reg,byte ptr [esp+byte]           │ 08AHEX, 01reg100,   024HEX, byte
; 2c) mov reg,byte ptr [EREG+byte]          │ 08AHEX, 01regERG,     byte
; 2d) mov reg,byte ptr [erega+eregb+byte]   │ 08AHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) mov reg,byte ptr [EREG+dword]         │ 08AHEX, 10regERG,    dword
; 3b) mov reg,byte ptr [ESP+dword]          │ 08AHEX, 10reg100,   024HEX, dword
; 3c) mov reg,byte ptr [erega+eregb+dword]  │ 08AHEX, 10reg100, 00rgBrgA, dword 
;
;  4) mov rega, regb (8 bit)                │ 08AHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @08A_disasm:   jmp    @089_disasm