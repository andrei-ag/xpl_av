;───────────────────────
; sub eregb,dword ptr [erega]            │ 02BHEX, 00rgBrgA
; sub ereg,dword ptr [esp]               │ 02BHEX, 00reg100,   024HEX
; sub ereg,dword ptr [erega+eregb]       │ 02BHEX, 00reg100, 00rgBrgA 
; sub ereg,dword ptr [dword]             │ 02BHEX, 00reg101,    dword
;
; sub ereg,dword ptr [ereg*2]            │ 02BHEX, 00reg100, 01reg101, 00000000
; sub ereg,dword ptr [ereg*2+dword]      │ 02BHEX, 00reg100, 01reg101,    dword
; sub ereg,dword ptr [ereg*4]            │ 02BHEX, 00reg100, 10reg101, 00000000
; sub ereg,dword ptr [ereg*4+dword]      │ 02BHEX, 00reg100, 10reg101,    dword
; sub ereg,dword ptr [ereg*8]            │ 02BHEX, 00reg100, 11reg101, 00000000
; sub ereg,dword ptr [ereg*8+dword]      │ 02BHEX, 00reg100, 11reg101,    dword
;
; sub ereg,dword ptr [erega+eregb*2]     │ 02BHEX, 00reg100, 01rgBrgA
; sub ereg,dword ptr [erega+eregb*4]     │ 02BHEX, 00reg100, 10rgBrgA
; sub ereg,dword ptr [erega+eregb*8]     │ 02BHEX, 00reg100, 11rgBrgA
;
; sub ereg,dword ptr [ebp]               │ 02BHEX, 01reg101, 00000000 
; sub ereg,dword ptr [esp+byte]          │ 02BHEX, 01reg100,   024HEX, byte
; sub eregb,dword ptr [erega+byte]       │ 02BHEX, 01rgBrgA,     byte
; sub ereg,dword ptr [erega+eregb+byte]  │ 02BHEX, 01reg100, 00rgBrgA, byte 
;
; sub eregb,dword ptr [erega+dword]      │ 02BHEX, 10rgBrgA,    dword
; sub ereg,dword ptr [ESP+dword]         │ 02BHEX, 10reg100,   024HEX, dword
; sub ereg,dword ptr [erega+eregb+dword] │ 02BHEX, 10reg100, 00rgBrgA, dword 
;
 emul_02B:           
                jmp     emul_08A