;───────────────────────
; sub dword ptr [EREG],ereg              │ 029HEX, 00regERG
; sub dword ptr [esp],ereg               │ 029HEX, 00reg100,   024HEX
; sub dword ptr [erega+eregb],ereg       │ 029HEX, 00reg100, 00rgBrgA 
; sub dword ptr [dword],ereg             │ 029HEX, 00reg101,    dword
;                                         
; sub dword ptr [ereg*2],ereg            │ 029HEX, 00reg100, 01reg101, 00000000
; sub dword ptr [ereg*2+dword],ereg      │ 029HEX, 00reg100, 01reg101,    dword
; sub dword ptr [ereg*4],ereg            │ 029HEX, 00reg100, 10reg101, 00000000
; sub dword ptr [ereg*4+dword],ereg      │ 029HEX, 00reg100, 10reg101,    dword
; sub dword ptr [ereg*8],ereg            │ 029HEX, 00reg100, 11reg101, 00000000
; sub dword ptr [ereg*8+dword],ereg      │ 029HEX, 00reg100, 11reg101,    dword
;
; sub dword ptr [erega+eregb*2],ereg     │ 029HEX, 00reg100, 01rgBrgA
; sub dword ptr [erega+eregb*4],ereg     │ 029HEX, 00reg100, 10rgBrgA
; sub dword ptr [erega+eregb*8],ereg     │ 029HEX, 00reg100, 11rgBrgA
;
; sub dword ptr [ebp],ereg               │ 029HEX, 01reg101, 00000000 
; sub dword ptr [esp+byte],ereg          │ 029HEX, 01reg100,   024HEX, byte
; sub dword ptr [EREG+byte],ereg         │ 029HEX, 01regERG,     byte
; sub dword ptr [erega+eregb+byte],ereg  │ 029HEX, 01reg100, 00rgBrgA, byte 
;
; sub dword ptr [EREG+dword],ereg        │ 029HEX, 10regERG,    dword
; sub dword ptr [ESP+dword],ereg         │ 029HEX, 10reg100,   024HEX, dword
; sub dword ptr [erega+eregb+dword],ereg │ 029HEX, 10reg100, 00rgBrgA, dword 
;
 emul_029:      push    2d81h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret