;───────────────────────
; or dword ptr [EREG],ereg              │ 009HEX, 00regERG
; or dword ptr [esp],ereg               │ 009HEX, 00reg100,   024HEX
; or dword ptr [erega+eregb],ereg       │ 009HEX, 00reg100, 00rgBrgA 
;                                         
; or dword ptr [ereg*2],ereg            │ 009HEX, 00reg100, 01reg101, 00000000
; or dword ptr [ereg*2+dword],ereg      │ 009HEX, 00reg100, 01reg101,    dword
; or dword ptr [ereg*4],ereg            │ 009HEX, 00reg100, 10reg101, 00000000
; or dword ptr [ereg*4+dword],ereg      │ 009HEX, 00reg100, 10reg101,    dword
; or dword ptr [ereg*8],ereg            │ 009HEX, 00reg100, 11reg101, 00000000
; or dword ptr [ereg*8+dword],ereg      │ 009HEX, 00reg100, 11reg101,    dword
;
; or dword ptr [erega+eregb*2],ereg     │ 009HEX, 00reg100, 01rgBrgA
; or dword ptr [erega+eregb*4],ereg     │ 009HEX, 00reg100, 10rgBrgA
; or dword ptr [erega+eregb*8],ereg     │ 009HEX, 00reg100, 11rgBrgA
;
; or dword ptr [ebp],ereg               │ 009HEX, 01reg101, 00000000 
; or dword ptr [esp+byte],ereg          │ 009HEX, 01reg100,   024HEX, byte
; or dword ptr [EREG+byte],ereg         │ 009HEX, 01regERG,     byte
; or dword ptr [erega+eregb+byte],ereg  │ 009HEX, 01reg100, 00rgBrgA, byte 
;
; or dword ptr [EREG+dword],ereg        │ 009HEX, 10regERG,    dword
; or dword ptr [ESP+dword],ereg         │ 009HEX, 10reg100,   024HEX, dword
; or dword ptr [erega+eregb+dword],ereg │ 009HEX, 10reg100, 00rgBrgA, dword 
;
 emul_009:      push    0d81h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret