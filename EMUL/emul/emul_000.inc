;───────────────────────
; add byte ptr [EREG],reg              │ 000HEX, 00regERG
; add byte ptr [esp],reg               │ 000HEX, 00reg100,   024HEX
; add byte ptr [erega+eregb],reg       │ 000HEX, 00reg100, 00rgBrgA 
; add byte ptr [dword],reg             │ 000HEX, 00reg101,    dword
;
; add byte ptr [ereg*2],reg            │ 000HEX, 00reg100, 01reg101, 00000000
; add byte ptr [ereg*2+dword],reg      │ 000HEX, 00reg100, 01reg101,    dword
; add byte ptr [ereg*4],reg            │ 000HEX, 00reg100, 10reg101, 00000000
; add byte ptr [ereg*4+dword],reg      │ 000HEX, 00reg100, 10reg101,    dword
; add byte ptr [ereg*8],reg            │ 000HEX, 00reg100, 11reg101, 00000000
; add byte ptr [ereg*8+dword],reg      │ 000HEX, 00reg100, 11reg101,    dword
;
; add byte ptr [erega+eregb*2],reg     │ 000HEX, 00reg100, 01rgBrgA
; add byte ptr [erega+eregb*4],reg     │ 000HEX, 00reg100, 10rgBrgA
; add byte ptr [erega+eregb*8],reg     │ 000HEX, 00reg100, 11rgBrgA
;
; add byte ptr [ebp],reg               │ 000HEX, 01reg101, 00000000 
; add byte ptr [esp+byte],reg          │ 000HEX, 01reg100,   024HEX, byte
; add byte ptr [EREG+byte],reg         │ 000HEX, 01regERG,     byte
; add byte ptr [erega+eregb+byte],reg  │ 000HEX, 01reg100, 00rgBrgA, byte 
;
; add byte ptr [EREG+dword],reg        │ 000HEX, 10regERG,    dword
; add byte ptr [ESP+dword],reg         │ 000HEX, 10reg100,   024HEX, dword
; add byte ptr [erega+eregb+dword],reg │ 000HEX, 10reg100, 00rgBrgA, dword 
;
 emul_000:      push    0580h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret