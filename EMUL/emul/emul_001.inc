;───────────────────────
; add dword ptr [EREG],ereg              │ 001HEX, 00regERG
; add dword ptr [esp],ereg               │ 001HEX, 00reg100,   024HEX
; add dword ptr [erega+eregb],ereg       │ 001HEX, 00reg100, 00rgBrgA 
; add dword ptr [dword],ereg             │ 001HEX, 00reg101,    dword
;
; add dword ptr [ereg*2],ereg            │ 001HEX, 00reg100, 01reg101, 00000000
; add dword ptr [ereg*2+dword],ereg      │ 001HEX, 00reg100, 01reg101,    dword
; add dword ptr [ereg*4],ereg            │ 001HEX, 00reg100, 10reg101, 00000000
; add dword ptr [ereg*4+dword],ereg      │ 001HEX, 00reg100, 10reg101,    dword
; add dword ptr [ereg*8],ereg            │ 001HEX, 00reg100, 11reg101, 00000000
; add dword ptr [ereg*8+dword],ereg      │ 001HEX, 00reg100, 11reg101,    dword
;
; add dword ptr [erega+eregb*2],ereg     │ 001HEX, 00reg100, 01rgBrgA
; add dword ptr [erega+eregb*4],ereg     │ 001HEX, 00reg100, 10rgBrgA
; add dword ptr [erega+eregb*8],ereg     │ 001HEX, 00reg100, 11rgBrgA
;
; add dword ptr [ebp],ereg               │ 001HEX, 01reg101, 00000000 
; add dword ptr [esp+byte],ereg          │ 001HEX, 01reg100,   024HEX, byte
; add dword ptr [EREG+byte],ereg         │ 001HEX, 01regERG,     byte
; add dword ptr [erega+eregb+byte],ereg  │ 001HEX, 01reg100, 00rgBrgA, byte 
;
; add dword ptr [EREG+dword],ereg        │ 001HEX, 10regERG,    dword
; add dword ptr [ESP+dword],ereg         │ 001HEX, 10reg100,   024HEX, dword
; add dword ptr [erega+eregb+dword],ereg │ 001HEX, 10reg100, 00rgBrgA, dword 
;
 emul_001:      push    0581h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret