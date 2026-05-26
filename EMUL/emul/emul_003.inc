;───────────────────────
; add eregb,dword ptr [erega]            │ 003HEX, 00rgBrgA
; add ereg,dword ptr [esp]               │ 003HEX, 00reg100,   024HEX
; add ereg,dword ptr [erega+eregb]       │ 003HEX, 00reg100, 00rgBrgA 
; add ereg,dword ptr [dword]             │ 003HEX, 00reg101,    dword
;
; add ereg,dword ptr [ereg*2]            │ 003HEX, 00reg100, 01reg101, 00000000
; add ereg,dword ptr [ereg*2+dword]      │ 003HEX, 00reg100, 01reg101,    dword
; add ereg,dword ptr [ereg*4]            │ 003HEX, 00reg100, 10reg101, 00000000
; add ereg,dword ptr [ereg*4+dword]      │ 003HEX, 00reg100, 10reg101,    dword
; add ereg,dword ptr [ereg*8]            │ 003HEX, 00reg100, 11reg101, 00000000
; add ereg,dword ptr [ereg*8+dword]      │ 003HEX, 00reg100, 11reg101,    dword
;
; add ereg,dword ptr [erega+eregb*2]     │ 003HEX, 00reg100, 01rgBrgA
; add ereg,dword ptr [erega+eregb*4]     │ 003HEX, 00reg100, 10rgBrgA
; add ereg,dword ptr [erega+eregb*8]     │ 003HEX, 00reg100, 11rgBrgA
;
; add ereg,dword ptr [ebp]               │ 003HEX, 01reg101, 00000000 
; add ereg,dword ptr [esp+byte]          │ 003HEX, 01reg100,   024HEX, byte
; add eregb,dword ptr [erega+byte]       │ 003HEX, 01rgBrgA,     byte
; add ereg,dword ptr [erega+eregb+byte]  │ 003HEX, 01reg100, 00rgBrgA, byte 
;
; add eregb,dword ptr [erega+dword]      │ 003HEX, 10rgBrgA,    dword
; add ereg,dword ptr [ESP+dword]         │ 003HEX, 10reg100,   024HEX, dword
; add ereg,dword ptr [erega+eregb+dword] │ 003HEX, 10reg100, 00rgBrgA, dword 
;
 emul_003:      
                jmp     emul_08A