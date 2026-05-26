;───────────────────────
; xor byte ptr [EREG],reg              │ 030HEX, 00regERG
; xor byte ptr [esp],reg               │ 030HEX, 00reg100,   024HEX
; xor byte ptr [erega+eregb],reg       │ 030HEX, 00reg100, 00rgBrgA 
; xor byte ptr [dword],reg             │ 030HEX, 00reg101,    dword
;
; xor byte ptr [ereg*2],reg            │ 030HEX, 00reg100, 01reg101, 00000000
; xor byte ptr [ereg*2+dword],reg      │ 030HEX, 00reg100, 01reg101,    dword
; xor byte ptr [ereg*4],reg            │ 030HEX, 00reg100, 10reg101, 00000000
; xor byte ptr [ereg*4+dword],reg      │ 030HEX, 00reg100, 10reg101,    dword
; xor byte ptr [ereg*8],reg            │ 030HEX, 00reg100, 11reg101, 00000000
; xor byte ptr [ereg*8+dword],reg      │ 030HEX, 00reg100, 11reg101,    dword
;
; xor byte ptr [erega+eregb*2],reg     │ 030HEX, 00reg100, 01rgBrgA
; xor byte ptr [erega+eregb*4],reg     │ 030HEX, 00reg100, 10rgBrgA
; xor byte ptr [erega+eregb*8],reg     │ 030HEX, 00reg100, 11rgBrgA
;
; xor byte ptr [ebp],reg               │ 030HEX, 01reg101, 00000000 
; xor byte ptr [esp+byte],reg          │ 030HEX, 01reg100,   024HEX, byte
; xor byte ptr [EREG+byte],reg         │ 030HEX, 01regERG,     byte
; xor byte ptr [erega+eregb+byte],reg  │ 030HEX, 01reg100, 00rgBrgA, byte 
;
; xor byte ptr [EREG+dword],reg        │ 030HEX, 10regERG,    dword
; xor byte ptr [ESP+dword],reg         │ 030HEX, 10reg100,   024HEX, dword
; xor byte ptr [erega+eregb+dword],reg │ 030HEX, 10reg100, 00rgBrgA, dword 
;
 emul_030:      push    3580h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret