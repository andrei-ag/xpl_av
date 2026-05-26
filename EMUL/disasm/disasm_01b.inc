;══════════════════════
; 1a) sbb eregb,dword ptr [erega]            │ 01BHEX, 00rgBrgA
; 1b) sbb ereg,dword ptr [esp]               │ 01BHEX, 00reg100,   024HEX
; 1c) sbb ereg,dword ptr [erega+eregb]       │ 01BHEX, 00reg100, 00rgBrgA 
; 1d) sbb ereg,dword ptr [dword]             │ 01BHEX, 00reg101,    dword
;
;     sbb ereg,dword ptr [ereg*2]            │ 01BHEX, 00reg100, 01reg101, 00000000
;     sbb ereg,dword ptr [ereg*2+dword]      │ 01BHEX, 00reg100, 01reg101,    dword
;     sbb ereg,dword ptr [ereg*4]            │ 01BHEX, 00reg100, 10reg101, 00000000
;     sbb ereg,dword ptr [ereg*4+dword]      │ 01BHEX, 00reg100, 10reg101,    dword
;     sbb ereg,dword ptr [ereg*8]            │ 01BHEX, 00reg100, 11reg101, 00000000
;     sbb ereg,dword ptr [ereg*8+dword]      │ 01BHEX, 00reg100, 11reg101,    dword
;
;     sbb ereg,dword ptr [erega+eregb*2]     │ 01BHEX, 00reg100, 01rgBrgA
;     sbb ereg,dword ptr [erega+eregb*4]     │ 01BHEX, 00reg100, 10rgBrgA
;     sbb ereg,dword ptr [erega+eregb*8]     │ 01BHEX, 00reg100, 11rgBrgA
;
; 2a) sbb ereg,dword ptr [ebp]               │ 01BHEX, 01reg101, 00000000 
; 2b) sbb ereg,dword ptr [esp+byte]          │ 01BHEX, 01reg100,   024HEX, byte
; 2c) sbb eregb,dword ptr [erega+byte]       │ 01BHEX, 01rgBrgA,     byte
; 2d) sbb ereg,dword ptr [erega+eregb+byte]  │ 01BHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sbb eregb,dword ptr [erega+dword]      │ 01BHEX, 10rgBrgA,    dword
; 3b) sbb ereg,dword ptr [ESP+dword]         │ 01BHEX, 10reg100,   024HEX, dword
; 3c) sbb ereg,dword ptr [erega+eregb+dword] │ 01BHEX, 10reg100, 00rgBrgA, dword 
;
;  4) sbb erega, eregb                       │ 01BHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @01B_disasm:   jmp    @089_disasm