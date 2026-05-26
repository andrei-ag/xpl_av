;══════════════════════
; 1a) and reg,byte ptr [EREG]               │ 022HEX, 00regERG
; 1b) and reg,byte ptr [esp]                │ 022HEX, 00reg100,   024HEX
; 1c) and reg,byte ptr [erega+eregb]        │ 022HEX, 00reg100, 00rgBrgA 
; 1d) and reg,byte ptr [dword]              │ 022HEX, 00reg101,    dword
;
;     and reg,byte ptr [ereg*2]             │ 022HEX, 00reg100, 01reg101, 00000000
;     and reg,byte ptr [ereg*2+dword]       │ 022HEX, 00reg100, 01reg101,    dword
;     and reg,byte ptr [ereg*4]             │ 022HEX, 00reg100, 10reg101, 00000000
;     and reg,byte ptr [ereg*4+dword]       │ 022HEX, 00reg100, 10reg101,    dword
;     and reg,byte ptr [ereg*8]             │ 022HEX, 00reg100, 11reg101, 00000000
;     and reg,byte ptr [ereg*8+dword]       │ 022HEX, 00reg100, 11reg101,    dword
;
;     and reg,byte ptr [erega+eregb*2]      │ 022HEX, 00reg100, 01rgBrgA
;     and reg,byte ptr [erega+eregb*4]      │ 022HEX, 00reg100, 10rgBrgA
;     and reg,byte ptr [erega+eregb*8]      │ 022HEX, 00reg100, 11rgBrgA
;
; 2a) and reg,byte ptr [ebp]                │ 022HEX, 01reg101, 00000000 
; 2b) and reg,byte ptr [esp+byte]           │ 022HEX, 01reg100,   024HEX, byte
; 2c) and reg,byte ptr [EREG+byte]          │ 022HEX, 01regERG,     byte
; 2d) and reg,byte ptr [erega+eregb+byte]   │ 022HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) and reg,byte ptr [EREG+dword]         │ 022HEX, 10regERG,    dword
; 3b) and reg,byte ptr [ESP+dword]          │ 022HEX, 10reg100,   024HEX, dword
; 3c) and reg,byte ptr [erega+eregb+dword]  │ 022HEX, 10reg100, 00rgBrgA, dword 
;
;  4) and rega, regb (8 bit)                │ 022HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @022_disasm:   jmp    @089_disasm