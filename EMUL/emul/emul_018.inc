;───────────────────────
; sbb byte ptr [EREG],reg              │ 018HEX, 00regERG
; sbb byte ptr [esp],reg               │ 018HEX, 00reg100,   024HEX
; sbb byte ptr [erega+eregb],reg       │ 018HEX, 00reg100, 00rgBrgA 
; sbb byte ptr [dword],reg             │ 018HEX, 00reg101,    dword
;
; sbb byte ptr [ereg*2],reg            │ 018HEX, 00reg100, 01reg101, 00000000
; sbb byte ptr [ereg*2+dword],reg      │ 018HEX, 00reg100, 01reg101,    dword
; sbb byte ptr [ereg*4],reg            │ 018HEX, 00reg100, 10reg101, 00000000
; sbb byte ptr [ereg*4+dword],reg      │ 018HEX, 00reg100, 10reg101,    dword
; sbb byte ptr [ereg*8],reg            │ 018HEX, 00reg100, 11reg101, 00000000
; sbb byte ptr [ereg*8+dword],reg      │ 018HEX, 00reg100, 11reg101,    dword
;
; sbb byte ptr [erega+eregb*2],reg     │ 018HEX, 00reg100, 01rgBrgA
; sbb byte ptr [erega+eregb*4],reg     │ 018HEX, 00reg100, 10rgBrgA
; sbb byte ptr [erega+eregb*8],reg     │ 018HEX, 00reg100, 11rgBrgA
;
; sbb byte ptr [ebp],reg               │ 018HEX, 01reg101, 00000000 
; sbb byte ptr [esp+byte],reg          │ 018HEX, 01reg100,   024HEX, byte
; sbb byte ptr [EREG+byte],reg         │ 018HEX, 01regERG,     byte
; sbb byte ptr [erega+eregb+byte],reg  │ 018HEX, 01reg100, 00rgBrgA, byte 
;
; sbb byte ptr [EREG+dword],reg        │ 018HEX, 10regERG,    dword
; sbb byte ptr [ESP+dword],reg         │ 018HEX, 10reg100,   024HEX, dword
; sbb byte ptr [erega+eregb+dword],reg │ 018HEX, 10reg100, 00rgBrgA, dword 
;
 emul_018:      push    1d80h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret