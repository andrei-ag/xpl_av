;───────────────────────
; or byte ptr [EREG],reg               │ 008HEX, 00regERG
; or byte ptr [esp],reg                │ 008HEX, 00reg100,   024HEX
; or byte ptr [erega+eregb],reg        │ 008HEX, 00reg100, 00rgBrgA 
; or byte ptr [dword],reg              │ 008HEX, 00reg101,    dword
;
; or byte ptr [ereg*2],reg             │ 008HEX, 00reg100, 01reg101, 00000000
; or byte ptr [ereg*2+dword],reg       │ 008HEX, 00reg100, 01reg101,    dword
; or byte ptr [ereg*4],reg             │ 008HEX, 00reg100, 10reg101, 00000000
; or byte ptr [ereg*4+dword],reg       │ 008HEX, 00reg100, 10reg101,    dword
; or byte ptr [ereg*8],reg             │ 008HEX, 00reg100, 11reg101, 00000000
; or byte ptr [ereg*8+dword],reg       │ 008HEX, 00reg100, 11reg101,    dword
;
; or byte ptr [erega+eregb*2],reg      │ 008HEX, 00reg100, 01rgBrgA
; or byte ptr [erega+eregb*4],reg      │ 008HEX, 00reg100, 10rgBrgA
; or byte ptr [erega+eregb*8],reg      │ 008HEX, 00reg100, 11rgBrgA
;
; or byte ptr [ebp],reg                │ 008HEX, 01reg101, 00000000 
; or byte ptr [esp+byte],reg           │ 008HEX, 01reg100,   024HEX, byte
; or byte ptr [EREG+byte],reg          │ 008HEX, 01regERG,     byte
; or byte ptr [erega+eregb+byte],reg   │ 008HEX, 01reg100, 00rgBrgA, byte 
;
; or byte ptr [EREG+dword],reg         │ 008HEX, 10regERG,    dword
; or byte ptr [ESP+dword],reg          │ 008HEX, 10reg100,   024HEX, dword
; or byte ptr [erega+eregb+dword],reg  │ 008HEX, 10reg100, 00rgBrgA, dword 
;
 emul_008:      push    0d80h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret