;───────────────────────
; and dword ptr [EREG],ereg              │ 021HEX, 00regERG
; and dword ptr [esp],ereg               │ 021HEX, 00reg100,   024HEX
; and dword ptr [erega+eregb],ereg       │ 021HEX, 00reg100, 00rgBrgA 
; and dword ptr [dword],ereg             │ 021HEX, 00reg101,    dword
;                                         
; and dword ptr [ereg*2],ereg            │ 021HEX, 00reg100, 01reg101, 00000000
; and dword ptr [ereg*2+dword],ereg      │ 021HEX, 00reg100, 01reg101,    dword
; and dword ptr [ereg*4],ereg            │ 021HEX, 00reg100, 10reg101, 00000000
; and dword ptr [ereg*4+dword],ereg      │ 021HEX, 00reg100, 10reg101,    dword
; and dword ptr [ereg*8],ereg            │ 021HEX, 00reg100, 11reg101, 00000000
; and dword ptr [ereg*8+dword],ereg      │ 021HEX, 00reg100, 11reg101,    dword
;
; and dword ptr [erega+eregb*2],ereg     │ 021HEX, 00reg100, 01rgBrgA
; and dword ptr [erega+eregb*4],ereg     │ 021HEX, 00reg100, 10rgBrgA
; and dword ptr [erega+eregb*8],ereg     │ 021HEX, 00reg100, 11rgBrgA
;
; and dword ptr [ebp],ereg               │ 021HEX, 01reg101, 00000000 
; and dword ptr [esp+byte],ereg          │ 021HEX, 01reg100,   024HEX, byte
; and dword ptr [EREG+byte],ereg         │ 021HEX, 01regERG,     byte
; and dword ptr [erega+eregb+byte],ereg  │ 021HEX, 01reg100, 00rgBrgA, byte 
;
; and dword ptr [EREG+dword],ereg        │ 021HEX, 10regERG,    dword
; and dword ptr [ESP+dword],ereg         │ 021HEX, 10reg100,   024HEX, dword
; and dword ptr [erega+eregb+dword],ereg │ 021HEX, 10reg100, 00rgBrgA, dword 
;
 emul_021:      push    2581h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret