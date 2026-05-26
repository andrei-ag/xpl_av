;══════════════════════
; 1a) mov eregb,dword ptr [erega]            │ 08BHEX, 00rgBrgA
; 1b) mov ereg,dword ptr [esp]               │ 08BHEX, 00reg100,   024HEX
; 1c) mov ereg,dword ptr [erega+eregb]       │ 08BHEX, 00reg100, 00rgBrgA 
; 1d) mov ereg,dword ptr [dword]             │ 08BHEX, 00reg101,    dword
;
;     mov ereg,dword ptr [ereg*2]            │ 08BHEX, 00reg100, 01reg101, 00000000
;     mov ereg,dword ptr [ereg*2+dword]      │ 08BHEX, 00reg100, 01reg101,    dword
;     mov ereg,dword ptr [ereg*4]            │ 08BHEX, 00reg100, 10reg101, 00000000
;     mov ereg,dword ptr [ereg*4+dword]      │ 08BHEX, 00reg100, 10reg101,    dword
;     mov ereg,dword ptr [ereg*8]            │ 08BHEX, 00reg100, 11reg101, 00000000
;     mov ereg,dword ptr [ereg*8+dword]      │ 08BHEX, 00reg100, 11reg101,    dword
;
;     mov ereg,dword ptr [erega+eregb*2]     │ 08BHEX, 00reg100, 01rgBrgA
;     mov ereg,dword ptr [erega+eregb*4]     │ 08BHEX, 00reg100, 10rgBrgA
;     mov ereg,dword ptr [erega+eregb*8]     │ 08BHEX, 00reg100, 11rgBrgA
;
; 2a) mov ereg,dword ptr [ebp]               │ 08BHEX, 01reg101, 00000000 
; 2b) mov ereg,dword ptr [esp+byte]          │ 08BHEX, 01reg100,   024HEX, byte
; 2c) mov eregb,dword ptr [erega+byte]       │ 08BHEX, 01rgBrgA,     byte
; 2d) mov ereg,dword ptr [erega+eregb+byte]  │ 08BHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) mov eregb,dword ptr [erega+dword]      │ 08BHEX, 10rgBrgA,    dword
; 3b) mov ereg,dword ptr [ESP+dword]         │ 08BHEX, 10reg100,   024HEX, dword
; 3c) mov ereg,dword ptr [erega+eregb+dword] │ 08BHEX, 10reg100, 00rgBrgA, dword 
;
;  4) mov erega, eregb                       │ 08BHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @08B_disasm:   jmp    @089_disasm