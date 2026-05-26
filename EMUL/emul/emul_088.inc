;───────────────────────
; mov byte ptr [EREG],reg              │ 088HEX, 00regERG
; mov byte ptr [esp],reg               │ 088HEX, 00reg100,   024HEX
; mov byte ptr [erega+eregb],reg       │ 088HEX, 00reg100, 00rgBrgA 
; mov byte ptr [dword],reg             │ 088HEX, 00reg101,    dword
;
; mov byte ptr [ereg*2],reg            │ 088HEX, 00reg100, 01reg101, 00000000
; mov byte ptr [ereg*2+dword],reg      │ 088HEX, 00reg100, 01reg101,    dword
; mov byte ptr [ereg*4],reg            │ 088HEX, 00reg100, 10reg101, 00000000
; mov byte ptr [ereg*4+dword],reg      │ 088HEX, 00reg100, 10reg101,    dword
; mov byte ptr [ereg*8],reg            │ 088HEX, 00reg100, 11reg101, 00000000
; mov byte ptr [ereg*8+dword],reg      │ 088HEX, 00reg100, 11reg101,    dword
;
; mov byte ptr [erega+eregb*2],reg     │ 088HEX, 00reg100, 01rgBrgA
; mov byte ptr [erega+eregb*4],reg     │ 088HEX, 00reg100, 10rgBrgA
; mov byte ptr [erega+eregb*8],reg     │ 088HEX, 00reg100, 11rgBrgA
;
; mov byte ptr [ebp],reg               │ 088HEX, 01reg101, 00000000 
; mov byte ptr [esp+byte],reg          │ 088HEX, 01reg100,   024HEX, byte
; mov byte ptr [EREG+byte],reg         │ 088HEX, 01regERG,     byte
; mov byte ptr [erega+eregb+byte],reg  │ 088HEX, 00reg100, 00rgBrgA, byte 
;
; mov byte ptr [EREG+dword],reg        │ 088HEX, 10regERG,    dword
; mov byte ptr [ESP+dword],reg         │ 088HEX, 10reg100,   024HEX, dword
; mov byte ptr [erega+eregb+dword],reg │ 088HEX, 00reg100, 00rgBrgA, dword 
;
 emul_088:      push    05c6h          ; Идентификатор инструкции
                call    emul_xxx_byte
                jc      semul_err_ret
                jmp     spec_emul_ret