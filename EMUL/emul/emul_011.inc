;───────────────────────
; adc dword ptr [EREG],ereg              │ 011HEX, 00regERG
; adc dword ptr [esp],ereg               │ 011HEX, 00reg100,   024HEX
; adc dword ptr [erega+eregb],ereg       │ 011HEX, 00reg100, 00rgBrgA 
; adc dword ptr [dword],ereg             │ 011HEX, 00reg101,    dword
;
; adc dword ptr [ereg*2],ereg            │ 011HEX, 00reg100, 01reg101, 00000000
; adc dword ptr [ereg*2+dword],ereg      │ 011HEX, 00reg100, 01reg101,    dword
; adc dword ptr [ereg*4],ereg            │ 011HEX, 00reg100, 10reg101, 00000000
; adc dword ptr [ereg*4+dword],ereg      │ 011HEX, 00reg100, 10reg101,    dword
; adc dword ptr [ereg*8],ereg            │ 011HEX, 00reg100, 11reg101, 00000000
; adc dword ptr [ereg*8+dword],ereg      │ 011HEX, 00reg100, 11reg101,    dword
;
; adc dword ptr [erega+eregb*2],ereg     │ 011HEX, 00reg100, 01rgBrgA
; adc dword ptr [erega+eregb*4],ereg     │ 011HEX, 00reg100, 10rgBrgA
; adc dword ptr [erega+eregb*8],ereg     │ 011HEX, 00reg100, 11rgBrgA
;
; adc dword ptr [ebp],ereg               │ 011HEX, 01reg101, 00000000 
; adc dword ptr [esp+byte],ereg          │ 011HEX, 01reg100,   024HEX, byte
; adc dword ptr [EREG+byte],ereg         │ 011HEX, 01regERG,     byte
; adc dword ptr [erega+eregb+byte],ereg  │ 011HEX, 01reg100, 00rgBrgA, byte 
;
; adc dword ptr [EREG+dword],ereg        │ 011HEX, 10regERG,    dword
; adc dword ptr [ESP+dword],ereg         │ 011HEX, 10reg100,   024HEX, dword
; adc dword ptr [erega+eregb+dword],ereg │ 011HEX, 10reg100, 00rgBrgA, dword 
;
 emul_011:      push    1581h          ; Идентификатор инструкции
                call    emul_xxx_dword
                jc      semul_err_ret
                jmp     spec_emul_ret