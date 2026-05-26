;══════════════════════
; 1a) lea eregb,dword ptr [erega]            │ 08DHEX, 00rgBrgA
; 1b) lea ereg,dword ptr [esp]               │ 08DHEX, 00reg100,   024HEX
; 1c) lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
; 1d) lea ereg,dword ptr [dword]             │ 08DHEX, 00reg101,    dword
;
;     lea ereg,dword ptr [ereg*2]            │ 08DHEX, 00reg100, 01reg101, 00000000
;     lea ereg,dword ptr [ereg*2+dword]      │ 08DHEX, 00reg100, 01reg101,    dword
;     lea ereg,dword ptr [ereg*4]            │ 08DHEX, 00reg100, 10reg101, 00000000
;     lea ereg,dword ptr [ereg*4+dword]      │ 08DHEX, 00reg100, 10reg101,    dword
;     lea ereg,dword ptr [ereg*8]            │ 08DHEX, 00reg100, 11reg101, 00000000
;     lea ereg,dword ptr [ereg*8+dword]      │ 08DHEX, 00reg100, 11reg101,    dword
;
;     lea ereg,dword ptr [erega+eregb*2]     │ 08DHEX, 00reg100, 01rgBrgA
;     lea ereg,dword ptr [erega+eregb*4]     │ 08DHEX, 00reg100, 10rgBrgA
;     lea ereg,dword ptr [erega+eregb*8]     │ 08DHEX, 00reg100, 11rgBrgA
;
; 2a) lea ereg,dword ptr [ebp]               │ 08DHEX, 01reg101, 00000000 
; 2b) lea ereg,dword ptr [esp+byte]          │ 08DHEX, 01reg100,   024HEX, byte
; 2c) lea eregb,dword ptr [erega+byte]       │ 08DHEX, 01rgBrgA,     byte
; 2d) lea ereg,dword ptr [erega+eregb+byte]  │ 08DHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) lea eregb,dword ptr [erega+dword]      │ 08DHEX, 10rgBrgA,    dword
; 3b) lea ereg,dword ptr [ESP+dword]         │ 08DHEX, 10reg100,   024HEX, dword
; 3c) lea ereg,dword ptr [erega+eregb+dword] │ 08DHEX, 10reg100, 00rgBrgA, dword 
;
;  4) lea erega, eregb                       │ 08DHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @08D_disasm:   
                jmp    @089_disasm