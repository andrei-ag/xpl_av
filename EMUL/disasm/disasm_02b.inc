;══════════════════════
; 1a) sub eregb,dword ptr [erega]            │ 02BHEX, 00rgBrgA
; 1b) sub ereg,dword ptr [esp]               │ 02BHEX, 00reg100,   024HEX
; 1c) sub ereg,dword ptr [erega+eregb]       │ 02BHEX, 00reg100, 00rgBrgA 
; 1d) sub ereg,dword ptr [dword]             │ 02BHEX, 00reg101,    dword
;
;     sub ereg,dword ptr [ereg*2]            │ 02BHEX, 00reg100, 01reg101, 00000000
;     sub ereg,dword ptr [ereg*2+dword]      │ 02BHEX, 00reg100, 01reg101,    dword
;     sub ereg,dword ptr [ereg*4]            │ 02BHEX, 00reg100, 10reg101, 00000000
;     sub ereg,dword ptr [ereg*4+dword]      │ 02BHEX, 00reg100, 10reg101,    dword
;     sub ereg,dword ptr [ereg*8]            │ 02BHEX, 00reg100, 11reg101, 00000000
;     sub ereg,dword ptr [ereg*8+dword]      │ 02BHEX, 00reg100, 11reg101,    dword
;
;     sub ereg,dword ptr [erega+eregb*2]     │ 02BHEX, 00reg100, 01rgBrgA
;     sub ereg,dword ptr [erega+eregb*4]     │ 02BHEX, 00reg100, 10rgBrgA
;     sub ereg,dword ptr [erega+eregb*8]     │ 02BHEX, 00reg100, 11rgBrgA
;
; 2a) sub ereg,dword ptr [ebp]               │ 02BHEX, 01reg101, 00000000 
; 2b) sub ereg,dword ptr [esp+byte]          │ 02BHEX, 01reg100,   024HEX, byte
; 2c) sub eregb,dword ptr [erega+byte]       │ 02BHEX, 01rgBrgA,     byte
; 2d) sub ereg,dword ptr [erega+eregb+byte]  │ 02BHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sub eregb,dword ptr [erega+dword]      │ 02BHEX, 10rgBrgA,    dword
; 3b) sub ereg,dword ptr [ESP+dword]         │ 02BHEX, 10reg100,   024HEX, dword
; 3c) sub ereg,dword ptr [erega+eregb+dword] │ 02BHEX, 10reg100, 00rgBrgA, dword 
;
;  4) sub erega, eregb                       │ 02BHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @02B_disasm:   jmp    @089_disasm