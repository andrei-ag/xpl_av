;───────────────────────
; or eregb,dword ptr [erega]            │ 00BHEX, 00rgBrgA
; or ereg,dword ptr [esp]               │ 00BHEX, 00reg100,   024HEX
; or ereg,dword ptr [erega+eregb]       │ 00BHEX, 00reg100, 00rgBrgA 
; or ereg,dword ptr [dword]             │ 00BHEX, 00reg101,    dword
;
; or ereg,dword ptr [ereg*2]            │ 00BHEX, 00reg100, 01reg101, 00000000
; or ereg,dword ptr [ereg*2+dword]      │ 00BHEX, 00reg100, 01reg101,    dword
; or ereg,dword ptr [ereg*4]            │ 00BHEX, 00reg100, 10reg101, 00000000
; or ereg,dword ptr [ereg*4+dword]      │ 00BHEX, 00reg100, 10reg101,    dword
; or ereg,dword ptr [ereg*8]            │ 00BHEX, 00reg100, 11reg101, 00000000
; or ereg,dword ptr [ereg*8+dword]      │ 00BHEX, 00reg100, 11reg101,    dword
;
; or ereg,dword ptr [erega+eregb*2]     │ 00BHEX, 00reg100, 01rgBrgA
; or ereg,dword ptr [erega+eregb*4]     │ 00BHEX, 00reg100, 10rgBrgA
; or ereg,dword ptr [erega+eregb*8]     │ 00BHEX, 00reg100, 11rgBrgA
;
; or ereg,dword ptr [ebp]               │ 00BHEX, 01reg101, 00000000 
; or ereg,dword ptr [esp+byte]          │ 00BHEX, 01reg100,   024HEX, byte
; or eregb,dword ptr [erega+byte]       │ 00BHEX, 01rgBrgA,     byte
; or ereg,dword ptr [erega+eregb+byte]  │ 00BHEX, 01reg100, 00rgBrgA, byte 
;
; or eregb,dword ptr [erega+dword]      │ 00BHEX, 10rgBrgA,    dword
; or ereg,dword ptr [ESP+dword]         │ 00BHEX, 10reg100,   024HEX, dword
; or ereg,dword ptr [erega+eregb+dword] │ 00BHEX, 10reg100, 00rgBrgA, dword 
;
 emul_00B:      
                jmp     emul_08A