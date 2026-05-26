;───────────────────────
; sub byte ptr [EREG],reg              │ 028HEX, 00regERG
; sub byte ptr [esp],reg               │ 028HEX, 00reg100,   024HEX
; sub byte ptr [erega+eregb],reg       │ 028HEX, 00reg100, 00rgBrgA 
; sub byte ptr [dword],reg             │ 028HEX, 00reg101,    dword
;
; sub byte ptr [ereg*2],reg            │ 028HEX, 00reg100, 01reg101, 00000000
; sub byte ptr [ereg*2+dword],reg      │ 028HEX, 00reg100, 01reg101,    dword
; sub byte ptr [ereg*4],reg            │ 028HEX, 00reg100, 10reg101, 00000000
; sub byte ptr [ereg*4+dword],reg      │ 028HEX, 00reg100, 10reg101,    dword
; sub byte ptr [ereg*8],reg            │ 028HEX, 00reg100, 11reg101, 00000000
; sub byte ptr [ereg*8+dword],reg      │ 028HEX, 00reg100, 11reg101,    dword
;
; sub byte ptr [erega+eregb*2],reg     │ 028HEX, 00reg100, 01rgBrgA
; sub byte ptr [erega+eregb*4],reg     │ 028HEX, 00reg100, 10rgBrgA
; sub byte ptr [erega+eregb*8],reg     │ 028HEX, 00reg100, 11rgBrgA
;
; sub byte ptr [ebp],reg               │ 028HEX, 01reg101, 00000000 
; sub byte ptr [esp+byte],reg          │ 028HEX, 01reg100,   024HEX, byte
; sub byte ptr [EREG+byte],reg         │ 028HEX, 01regERG,     byte
; sub byte ptr [erega+eregb+byte],reg  │ 028HEX, 01reg100, 00rgBrgA, byte 
;
; sub byte ptr [EREG+dword],reg        │ 028HEX, 10regERG,    dword
; sub byte ptr [ESP+dword],reg         │ 028HEX, 10reg100,   024HEX, dword
; sub byte ptr [erega+eregb+dword],reg │ 028HEX, 10reg100, 00rgBrgA, dword 
;
 emul_028:      push    2d80h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret