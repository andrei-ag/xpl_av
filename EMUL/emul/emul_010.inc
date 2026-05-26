;───────────────────────
; adc byte ptr [EREG],reg              │ 010HEX, 00regERG
; adc byte ptr [esp],reg               │ 010HEX, 00reg100,   024HEX
; adc byte ptr [erega+eregb],reg       │ 010HEX, 00reg100, 00rgBrgA 
; adc byte ptr [dword],reg             │ 010HEX, 00reg101,    dword
;
; adc byte ptr [ereg*2],reg            │ 010HEX, 00reg100, 01reg101, 00000000
; adc byte ptr [ereg*2+dword],reg      │ 010HEX, 00reg100, 01reg101,    dword
; adc byte ptr [ereg*4],reg            │ 010HEX, 00reg100, 10reg101, 00000000
; adc byte ptr [ereg*4+dword],reg      │ 010HEX, 00reg100, 10reg101,    dword
; adc byte ptr [ereg*8],reg            │ 010HEX, 00reg100, 11reg101, 00000000
; adc byte ptr [ereg*8+dword],reg      │ 010HEX, 00reg100, 11reg101,    dword
;
; adc byte ptr [erega+eregb*2],reg     │ 010HEX, 00reg100, 01rgBrgA
; adc byte ptr [erega+eregb*4],reg     │ 010HEX, 00reg100, 10rgBrgA
; adc byte ptr [erega+eregb*8],reg     │ 010HEX, 00reg100, 11rgBrgA
;
; adc byte ptr [ebp],reg               │ 010HEX, 01reg101, 00000000 
; adc byte ptr [esp+byte],reg          │ 010HEX, 01reg100,   024HEX, byte
; adc byte ptr [EREG+byte],reg         │ 010HEX, 01regERG,     byte
; adc byte ptr [erega+eregb+byte],reg  │ 010HEX, 01reg100, 00rgBrgA, byte 
;
; adc byte ptr [EREG+dword],reg        │ 010HEX, 10regERG,    dword
; adc byte ptr [ESP+dword],reg         │ 010HEX, 10reg100,   024HEX, dword
; adc byte ptr [erega+eregb+dword],reg │ 010HEX, 10reg100, 00rgBrgA, dword 
;
 emul_010:      push    1580h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret