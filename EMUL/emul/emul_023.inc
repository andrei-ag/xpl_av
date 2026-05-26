;───────────────────────
; and eregb,dword ptr [erega]            │ 023HEX, 00rgBrgA
; and ereg,dword ptr [esp]               │ 023HEX, 00reg100,   024HEX
; and ereg,dword ptr [erega+eregb]       │ 023HEX, 00reg100, 00rgBrgA 
; and ereg,dword ptr [dword]             │ 023HEX, 00reg101,    dword
;
; and ereg,dword ptr [ereg*2]            │ 023HEX, 00reg100, 01reg101, 00000000
; and ereg,dword ptr [ereg*2+dword]      │ 023HEX, 00reg100, 01reg101,    dword
; and ereg,dword ptr [ereg*4]            │ 023HEX, 00reg100, 10reg101, 00000000
; and ereg,dword ptr [ereg*4+dword]      │ 023HEX, 00reg100, 10reg101,    dword
; and ereg,dword ptr [ereg*8]            │ 023HEX, 00reg100, 11reg101, 00000000
; and ereg,dword ptr [ereg*8+dword]      │ 023HEX, 00reg100, 11reg101,    dword
;
; and ereg,dword ptr [erega+eregb*2]     │ 023HEX, 00reg100, 01rgBrgA
; and ereg,dword ptr [erega+eregb*4]     │ 023HEX, 00reg100, 10rgBrgA
; and ereg,dword ptr [erega+eregb*8]     │ 023HEX, 00reg100, 11rgBrgA
;
; and ereg,dword ptr [ebp]               │ 023HEX, 01reg101, 00000000 
; and ereg,dword ptr [esp+byte]          │ 023HEX, 01reg100,   024HEX, byte
; and eregb,dword ptr [erega+byte]       │ 023HEX, 01rgBrgA,     byte
; and ereg,dword ptr [erega+eregb+byte]  │ 023HEX, 01reg100, 00rgBrgA, byte 
;
; and eregb,dword ptr [erega+dword]      │ 023HEX, 10rgBrgA,    dword
; and ereg,dword ptr [ESP+dword]         │ 023HEX, 10reg100,   024HEX, dword
; and ereg,dword ptr [erega+eregb+dword] │ 023HEX, 10reg100, 00rgBrgA, dword 
;
 emul_023:           
                jmp     emul_08A