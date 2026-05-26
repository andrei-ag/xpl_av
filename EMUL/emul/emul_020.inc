;───────────────────────
; and byte ptr [EREG],reg              │ 020HEX, 00regERG
; and byte ptr [esp],reg               │ 020HEX, 00reg100,   024HEX
; and byte ptr [erega+eregb],reg       │ 020HEX, 00reg100, 00rgBrgA 
; and byte ptr [dword],reg             │ 020HEX, 00reg101,    dword
;
; and byte ptr [ereg*2],reg            │ 020HEX, 00reg100, 01reg101, 00000000
; and byte ptr [ereg*2+dword],reg      │ 020HEX, 00reg100, 01reg101,    dword
; and byte ptr [ereg*4],reg            │ 020HEX, 00reg100, 10reg101, 00000000
; and byte ptr [ereg*4+dword],reg      │ 020HEX, 00reg100, 10reg101,    dword
; and byte ptr [ereg*8],reg            │ 020HEX, 00reg100, 11reg101, 00000000
; and byte ptr [ereg*8+dword],reg      │ 020HEX, 00reg100, 11reg101,    dword
;
; and byte ptr [erega+eregb*2],reg     │ 020HEX, 00reg100, 01rgBrgA
; and byte ptr [erega+eregb*4],reg     │ 020HEX, 00reg100, 10rgBrgA
; and byte ptr [erega+eregb*8],reg     │ 020HEX, 00reg100, 11rgBrgA
;
; and byte ptr [ebp],reg               │ 020HEX, 01reg101, 00000000 
; and byte ptr [esp+byte],reg          │ 020HEX, 01reg100,   024HEX, byte
; and byte ptr [EREG+byte],reg         │ 020HEX, 01regERG,     byte
; and byte ptr [erega+eregb+byte],reg  │ 020HEX, 01reg100, 00rgBrgA, byte 
;
; and byte ptr [EREG+dword],reg        │ 020HEX, 10regERG,    dword
; and byte ptr [ESP+dword],reg         │ 020HEX, 10reg100,   024HEX, dword
; and byte ptr [erega+eregb+dword],reg │ 020HEX, 10reg100, 00rgBrgA, dword 
;
 emul_020:      push    2580h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret