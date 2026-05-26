;══════════════════════
; 1a) sbb dword ptr [EREG],ereg              │ 019HEX, 00regERG
; 1b) sbb dword ptr [esp],ereg               │ 019HEX, 00reg100,   024HEX
; 1c) sbb dword ptr [erega+eregb],ereg       │ 019HEX, 00reg100, 00rgBrgA 
; 1d) sbb dword ptr [dword],ereg             │ 019HEX, 00reg101,    dword
;                                         
;     sbb dword ptr [ereg*2],ereg            │ 019HEX, 00reg100, 01reg101, 00000000
;     sbb dword ptr [ereg*2+dword],ereg      │ 019HEX, 00reg100, 01reg101,    dword
;     sbb dword ptr [ereg*4],ereg            │ 019HEX, 00reg100, 10reg101, 00000000
;     sbb dword ptr [ereg*4+dword],ereg      │ 019HEX, 00reg100, 10reg101,    dword
;     sbb dword ptr [ereg*8],ereg            │ 019HEX, 00reg100, 11reg101, 00000000
;     sbb dword ptr [ereg*8+dword],ereg      │ 019HEX, 00reg100, 11reg101,    dword
;
;     sbb dword ptr [erega+eregb*2],ereg     │ 019HEX, 00reg100, 01rgBrgA
;     sbb dword ptr [erega+eregb*4],ereg     │ 019HEX, 00reg100, 10rgBrgA
;     sbb dword ptr [erega+eregb*8],ereg     │ 019HEX, 00reg100, 11rgBrgA
;
; 2a) sbb dword ptr [ebp],ereg               │ 019HEX, 01reg101, 00000000 
; 2b) sbb dword ptr [esp+byte],ereg          │ 019HEX, 01reg100,   024HEX, byte
; 2c) sbb dword ptr [EREG+byte],ereg         │ 019HEX, 01regERG,     byte
; 2d) sbb dword ptr [erega+eregb+byte],ereg  │ 019HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) sbb dword ptr [EREG+dword],ereg        │ 019HEX, 10regERG,    dword
; 3b) sbb dword ptr [ESP+dword],ereg         │ 019HEX, 10reg100,   024HEX, dword
; 3c) sbb dword ptr [erega+eregb+dword],ereg │ 019HEX, 10reg100, 00rgBrgA, dword 
;
;  4) sbb erega, eregb                       │ 019HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @019_disasm:   
                jmp    @089_disasm