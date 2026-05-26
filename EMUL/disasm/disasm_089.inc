;══════════════════════
; 1a) mov dword ptr [erega],eregb            │ 089HEX, 00rgBrgA
; 1b) mov dword ptr [esp],ereg               │ 089HEX, 00reg100,   024HEX
; 1c) mov dword ptr [erega+eregb],ereg       │ 089HEX, 00reg100, 00rgBrgA 
; 1d) mov dword ptr [dword],ereg             │ 089HEX, 00reg101,    dword
;
;     mov dword ptr [ereg*2],ereg            │ 089HEX, 00reg100, 01reg101, 00000000
;     mov dword ptr [ereg*2+dword],ereg      │ 089HEX, 00reg100, 01reg101,    dword
;     mov dword ptr [ereg*4],ereg            │ 089HEX, 00reg100, 10reg101, 00000000
;     mov dword ptr [ereg*4+dword],ereg      │ 089HEX, 00reg100, 10reg101,    dword
;     mov dword ptr [ereg*8],ereg            │ 089HEX, 00reg100, 11reg101, 00000000
;     mov dword ptr [ereg*8+dword],ereg      │ 089HEX, 00reg100, 11reg101,    dword
;
;     mov dword ptr [erega+eregb*2],ereg     │ 089HEX, 00reg100, 01rgBrgA
;     mov dword ptr [erega+eregb*4],ereg     │ 089HEX, 00reg100, 10rgBrgA
;     mov dword ptr [erega+eregb*8],ereg     │ 089HEX, 00reg100, 11rgBrgA
;
; 2a) mov dword ptr [ebp],ereg               │ 089HEX, 01reg101, 00000000 
; 2b) mov dword ptr [esp+byte],ereg          │ 089HEX, 01reg100,   024HEX, byte
; 2c) mov dword ptr [erega+byte],eregb       │ 089HEX, 01rgBrgA,     byte
; 2d) mov dword ptr [erega+eregb+byte],ereg  │ 089HEX, 01reg100, 00rgBrgA, byte 
;
; 3a) mov dword ptr [erega+dword],eregb      │ 089HEX, 10rgBrgA,    dword
; 3b) mov dword ptr [ESP+dword],ereg         │ 089HEX, 10reg100,   024HEX, dword
; 3c) mov dword ptr [erega+eregb+dword],ereg │ 089HEX, 10reg100, 00rgBrgA, dword 
;
;  4) mov erega, eregb                       │ 089HEX, 11rgBrgA
;
; Инструкции 1a-3c разбираются так же как и те, что начинаются с "0FFh"
; Совпадает даже размер
;
 @089_disasm:   lodsb
                sub    ecx,ecx
                cmp    al,11000000b
                jc     @0FF_disasm_test_x   ; 1, 2, 3
;
;  4) mov erega, eregb                      │ 089HEX, 11rgBrgA
;
                add    cl,2
                jmp    disasm_ret