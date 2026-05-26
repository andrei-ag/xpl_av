;══════════════════════
; 1a) xor dword ptr [EREG],ereg              │ 031HEX, 00regERG
; 1b) xor dword ptr [esp],ereg               │ 031HEX, 00reg100,   024HEX
; 1c) xor dword ptr [erega+eregb],ereg       │ 031HEX, 00reg100, 00rgBrgA 
; 1d) xor dword ptr [dword],ereg             │ 031HEX, 00reg101,    dword
;                                         
;     xor dword ptr [ereg*2],ereg            │ 031HEX, 00reg100, 01reg101, 00000000
;     xor dword ptr [ereg*2+dword],ereg      │ 031HEX, 00reg100, 01reg101,    dword
;     xor dword ptr [ereg*4],ereg            │ 031HEX, 00reg100, 10reg101, 00000000
;     xor dword ptr [ereg*4+dword],ereg      │ 031HEX, 00reg100, 10reg101,    dword
;     xor dword ptr [ereg*8],ereg            │ 031HEX, 00reg100, 11reg101, 00000000
;     xor dword ptr [ereg*8+dword],ereg      │ 031HEX, 00reg100, 11reg101,    dword
;
;     xor dword ptr [erega+eregb*2],ereg     │ 031HEX, 00reg100, 01rgBrgA
;     xor dword ptr [erega+eregb*4],ereg     │ 031HEX, 00reg100, 10rgBrgA
;     xor dword ptr [erega+eregb*8],ereg     │ 031HEX, 00reg100, 11rgBrgA
;
; 2a) xor dword ptr [ebp],ereg               │ 031HEX, 01reg101, 00000000 
; 2b) xor dword ptr [esp+byte],ereg          │ 031HEX, 01reg100,   024HEX, byte
; 2c) xor dword ptr [EREG+byte],ereg         │ 031HEX, 01regERG,     byte
; 2d) xor dword ptr [erega+eregb+byte],ereg  │ 031HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) xor dword ptr [EREG+dword],ereg        │ 031HEX, 10regERG,    dword
; 3b) xor dword ptr [ESP+dword],ereg         │ 031HEX, 10reg100,   024HEX, dword
; 3c) xor dword ptr [erega+eregb+dword],ereg │ 031HEX, 10reg100, 00rgBrgA, dword 
;
;  4) xor erega, eregb                       │ 031HEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @031_disasm:   
                jmp    @089_disasm