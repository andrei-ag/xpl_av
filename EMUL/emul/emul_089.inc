;───────────────────────
; mov dword ptr [erega],eregb           │ 089HEX, 00rgBrgA
; mov dword ptr [esp],ereg              │ 089HEX, 00reg100,   024HEX
; mov dword ptr [erega+eregb],reg       │ 089HEX, 00reg100, 00rgBrgA 
; mov dword ptr [dword],ereg            │ 089HEX, 00reg101,    dword
;
; mov dword ptr [ereg*2],ereg           │ 089HEX, 00reg100, 01reg101, 00000000
; mov dword ptr [ereg*2+dword],ereg     │ 089HEX, 00reg100, 01reg101,    dword
; mov dword ptr [ereg*4],ereg           │ 089HEX, 00reg100, 10reg101, 00000000
; mov dword ptr [ereg*4+dword],ereg     │ 089HEX, 00reg100, 10reg101,    dword
; mov dword ptr [ereg*8],ereg           │ 089HEX, 00reg100, 11reg101, 00000000
; mov dword ptr [ereg*8+dword],ereg     │ 089HEX, 00reg100, 11reg101,    dword
;
; mov dword ptr [erega+eregb*2],ereg    │ 089HEX, 00reg100, 01rgBrgA
; mov dword ptr [erega+eregb*4],ereg    │ 089HEX, 00reg100, 10rgBrgA
; mov dword ptr [erega+eregb*8],ereg    │ 089HEX, 00reg100, 11rgBrgA
;
; mov dword ptr [ebp],ereg              │ 089HEX, 01reg101, 00000000 
; mov dword ptr [esp+byte],ereg         │ 089HEX, 01reg100,   024HEX, byte
; mov dword ptr [erega+byte],eregb      │ 089HEX, 01rgBrgA,     byte
; mov dword ptr [erega+eregb+byte],reg  │ 089HEX, 00reg100, 00rgBrgA, byte 
;
; mov dword ptr [erega+dword],eregb     │ 089HEX, 10rgBrgA,    dword
; mov dword ptr [ESP+dword],ereg        │ 089HEX, 10reg100,   024HEX, dword
; mov dword ptr [erega+eregb+dword],reg │ 089HEX, 00reg100, 00rgBrgA, dword 
;
 emul_089:      push    05c7h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret