;══════════════════════                                                         
; 1a) xxx byte ptr [ereg]                │ 0F6HEX, 00XXXreg
; 1b) xxx byte ptr [esp]                 │ 0F6HEX, 00XXX100,   024HEX
; 1c) xxx byte ptr [erega+eregb]         │ 0F6HEX, 00XXX100, 00rgBrgA 
; 1d) xxx byte ptr [dword]               │ 0F6HEX, 00XXX101,    dword
;
;     xxx byte ptr [ereg*2]              │ 0F6HEX, 00XXX100, 01reg101, 00000000                                
;     xxx byte ptr [ereg*2+dword]        │ 0F6HEX, 00XXX100, 01reg101,    dword
;     xxx byte ptr [ereg*4]              │ 0F6HEX, 00XXX100, 10reg101, 00000000                             
;     xxx byte ptr [ereg*4+dword]        │ 0F6HEX, 00XXX100, 10reg101,    dword
;     xxx byte ptr [ereg*8]              │ 0F6HEX, 00XXX100, 11reg101, 00000000                                
;     xxx byte ptr [ereg*8+dword]        │ 0F6HEX, 00XXX100, 11reg101,    dword
;
;     xxx byte ptr [erega+eregb*2]       │ 0F6HEX, 00XXX100, 01rgBrgA
;     xxx byte ptr [erega+eregb*4]       │ 0F6HEX, 00XXX100, 10rgBrgA
;     xxx byte ptr [erega+eregb*8]       │ 0F6HEX, 00XXX100, 11rgBrgA
;                                
; 1e) xxx byte ptr [ebp]                 │ 0F6HEX, 01XXX101, 00000000 
; 1f) xxx byte ptr [esp+byte]            │ 0F6HEX, 01XXX100,   024HEX,  byte
; 1g) xxx byte ptr [ereg+byte]           │ 0F6HEX, 01XXXreg,     byte
; 1h) xxx byte ptr [erega+eregb+byte]    │ 0F6HEX, 01XXX100, 00rgBrgA,  byte 
;
; 1i) xxx byte ptr [ereg+dword]          │ 0F6HEX, 10XXXreg,    dword
; 1j) xxx byte ptr [ESP+dword]           │ 0F6HEX, 10XXX100,   024HEX, dword
; 1k) xxx byte ptr [erega+eregb+dword]   │ 0F6HEX, 10XXX100, 00rgBrgA, dword
;
;  2) xxx ereg                           │ 0F6HEX, 11XXXreg
;
; LEGEND: 000 - TEST, 001 - ???,  010 - NOT, 011 - NEG,
;         100 - MUL,  101 - IMUL, 110 - DIV, 111 - IDIV 
;
;  Инструкции NOT/NEG/MUL/IMUL/DIV/IDIV по алгоритму разбора,
; составу и размеру, полностью соответствуют аналогам из "0FFh"
;
; Инструкции TEST, DIV, IDIV пока не разбираются
;
 @0F6_disasm:   lodsb
                sub    ecx,ecx
;
                push   eax
                push   edx
;
                sub    ah,ah
                cmp    al,00111111b
                jbe    @0F6_disasm_test
;                 
                mov    ah,01000000b
                cmp    al,01111111b
                jbe    @0F6_disasm_test
;
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @0F6_disasm_test
;
                mov    ah,11000000b
 @0F6_disasm_test:
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    al,001b
                ja     @0F6_disasm_test_2      ; >= NOT
                pop    edx
                pop    eax                 
                jmp    unknown_instr           ; < NOT
;
 @0F6_disasm_test_2:
                cmp    al,101b
                pop    edx
                pop    eax
                ja     unknown_instr          ; = DIV, IDIV
;
                cmp    al,11000000b
                jc     @0FF_disasm_test_x   ; 1 (i - k)
;
;  2) xxx ereg                           │ 0F6HEX, 11XXXreg
;
                add    cl,2
                jmp    disasm_ret