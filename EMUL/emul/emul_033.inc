;───────────────────────
; xor eregb,dword ptr [erega]            │ 033HEX, 00rgBrgA
; xor ereg,dword ptr [esp]               │ 033HEX, 00reg100,   024HEX
; xor ereg,dword ptr [erega+eregb]       │ 033HEX, 00reg100, 00rgBrgA 
; xor ereg,dword ptr [dword]             │ 033HEX, 00reg101,    dword
;
; xor ereg,dword ptr [ereg*2]            │ 033HEX, 00reg100, 01reg101, 00000000
; xor ereg,dword ptr [ereg*2+dword]      │ 033HEX, 00reg100, 01reg101,    dword
; xor ereg,dword ptr [ereg*4]            │ 033HEX, 00reg100, 10reg101, 00000000
; xor ereg,dword ptr [ereg*4+dword]      │ 033HEX, 00reg100, 10reg101,    dword
; xor ereg,dword ptr [ereg*8]            │ 033HEX, 00reg100, 11reg101, 00000000
; xor ereg,dword ptr [ereg*8+dword]      │ 033HEX, 00reg100, 11reg101,    dword
;
; xor ereg,dword ptr [erega+eregb*2]     │ 033HEX, 00reg100, 01rgBrgA
; xor ereg,dword ptr [erega+eregb*4]     │ 033HEX, 00reg100, 10rgBrgA
; xor ereg,dword ptr [erega+eregb*8]     │ 033HEX, 00reg100, 11rgBrgA
;
; xor ereg,dword ptr [ebp]               │ 033HEX, 01reg101, 00000000 
; xor ereg,dword ptr [esp+byte]          │ 033HEX, 01reg100,   024HEX, byte
; xor eregb,dword ptr [erega+byte]       │ 033HEX, 01rgBrgA,     byte
; xor ereg,dword ptr [erega+eregb+byte]  │ 033HEX, 01reg100, 00rgBrgA, byte 
;
; xor eregb,dword ptr [erega+dword]      │ 033HEX, 10rgBrgA,    dword
; xor ereg,dword ptr [ESP+dword]         │ 033HEX, 10reg100,   024HEX, dword
; xor ereg,dword ptr [erega+eregb+dword] │ 033HEX, 10reg100, 00rgBrgA, dword 
;
 emul_033:      
                jmp     emul_08A