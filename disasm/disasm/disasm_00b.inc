;══════════════════════
; 1a) or eregb,dword ptr [erega]            │ 00BHEX, 00rgBrgA
; 1b) or ereg,dword ptr [esp]               │ 00BHEX, 00reg100,   024HEX
; 1c) or ereg,dword ptr [erega+eregb]       │ 00BHEX, 00reg100, 00rgBrgA 
; 1d) or ereg,dword ptr [dword]             │ 00BHEX, 00reg101,    dword
;
;     or ereg,dword ptr [ereg*2]            │ 00BHEX, 00reg100, 01reg101, 00000000
;     or ereg,dword ptr [ereg*2+dword]      │ 00BHEX, 00reg100, 01reg101,    dword
;     or ereg,dword ptr [ereg*4]            │ 00BHEX, 00reg100, 10reg101, 00000000
;     or ereg,dword ptr [ereg*4+dword]      │ 00BHEX, 00reg100, 10reg101,    dword
;     or ereg,dword ptr [ereg*8]            │ 00BHEX, 00reg100, 11reg101, 00000000
;     or ereg,dword ptr [ereg*8+dword]      │ 00BHEX, 00reg100, 11reg101,    dword
;
;     or ereg,dword ptr [erega+eregb*2]     │ 00BHEX, 00reg100, 01rgBrgA
;     or ereg,dword ptr [erega+eregb*4]     │ 00BHEX, 00reg100, 10rgBrgA
;     or ereg,dword ptr [erega+eregb*8]     │ 00BHEX, 00reg100, 11rgBrgA
;
; 2a) or ereg,dword ptr [ebp]               │ 00BHEX, 01reg101, 00000000 
; 2b) or ereg,dword ptr [esp+byte]          │ 00BHEX, 01reg100,   024HEX, byte
; 2c) or eregb,dword ptr [erega+byte]       │ 00BHEX, 01rgBrgA,     byte
; 2d) or ereg,dword ptr [erega+eregb+byte]  │ 00BHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) or eregb,dword ptr [erega+dword]      │ 00BHEX, 10rgBrgA,    dword
; 3b) or ereg,dword ptr [ESP+dword]         │ 00BHEX, 10reg100,   024HEX, dword
; 3c) or ereg,dword ptr [erega+eregb+dword] │ 00BHEX, 10reg100, 00rgBrgA, dword 
;
;  4) or erega, eregb                       │ 00BHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @00B_disasm:   
                jmp    @089_disasm