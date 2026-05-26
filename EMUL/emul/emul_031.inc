;───────────────────────
; xor dword ptr [EREG],ereg              │ 031HEX, 00regERG
; xor dword ptr [esp],ereg               │ 031HEX, 00reg100,   024HEX
; xor dword ptr [erega+eregb],ereg       │ 031HEX, 00reg100, 00rgBrgA 
; xor dword ptr [dword],ereg             │ 031HEX, 00reg101,    dword
;                                         
; xor dword ptr [ereg*2],ereg            │ 031HEX, 00reg100, 01reg101, 00000000
; xor dword ptr [ereg*2+dword],ereg      │ 031HEX, 00reg100, 01reg101,    dword
; xor dword ptr [ereg*4],ereg            │ 031HEX, 00reg100, 10reg101, 00000000
; xor dword ptr [ereg*4+dword],ereg      │ 031HEX, 00reg100, 10reg101,    dword
; xor dword ptr [ereg*8],ereg            │ 031HEX, 00reg100, 11reg101, 00000000
; xor dword ptr [ereg*8+dword],ereg      │ 031HEX, 00reg100, 11reg101,    dword
;
; xor dword ptr [erega+eregb*2],ereg     │ 031HEX, 00reg100, 01rgBrgA
; xor dword ptr [erega+eregb*4],ereg     │ 031HEX, 00reg100, 10rgBrgA
; xor dword ptr [erega+eregb*8],ereg     │ 031HEX, 00reg100, 11rgBrgA
;
; xor dword ptr [ebp],ereg               │ 031HEX, 01reg101, 00000000 
; xor dword ptr [esp+byte],ereg          │ 031HEX, 01reg100,   024HEX, byte
; xor dword ptr [EREG+byte],ereg         │ 031HEX, 01regERG,     byte
; xor dword ptr [erega+eregb+byte],ereg  │ 031HEX, 01reg100, 00rgBrgA, byte 
;
; xor dword ptr [EREG+dword],ereg        │ 031HEX, 10regERG,    dword
; xor dword ptr [ESP+dword],ereg         │ 031HEX, 10reg100,   024HEX, dword
; xor dword ptr [erega+eregb+dword],ereg │ 031HEX, 10reg100, 00rgBrgA, dword 
;
 emul_031:      push    3581h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret