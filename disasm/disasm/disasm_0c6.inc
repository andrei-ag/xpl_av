;══════════════════════
; 1a) mov byte ptr [ereg],byte                │ 0C6HEX, 00000reg,     byte
; 1b) mov byte ptr [esp],byte                 │ 0C6HEX, 00000100,   024HEX,  byte
; 1c) mov byte ptr [erega+eregb],byte         │ 0C6HEX, 00000100, 00rgBrgA,  byte 
; 1d) mov byte ptr [dword],byte               │ 0C6HEX, 00000101,    dword,  byte
;
;     mov byte ptr [ereg*2],byte              │ 0C6HEX, 00000100, 01reg101, 00000000,  byte
;     mov byte ptr [ereg*2+dword],byte        │ 0C6HEX, 00000100, 01reg101,    dword,  byte
;     mov byte ptr [ereg*4],byte              │ 0C6HEX, 00000100, 10reg101, 00000000,  byte                             
;     mov byte ptr [ereg*4+dword],byte        │ 0C6HEX, 00000100, 10reg101,    dword,  byte
;     mov byte ptr [ereg*8],byte              │ 0C6HEX, 00000100, 11reg101, 00000000,  byte                                
;     mov byte ptr [ereg*8+dword],byte        │ 0C6HEX, 00000100, 11reg101,    dword,  byte
;
;     mov byte ptr [erega+eregb*2],byte       │ 0C6HEX, 00000100, 01rgBrgA,  byte
;     mov byte ptr [erega+eregb*4],byte       │ 0C6HEX, 00000100, 10rgBrgA,  byte
;     mov byte ptr [erega+eregb*8],byte       │ 0C6HEX, 00000100, 11rgBrgA,  byte
;                                             
; 1e) mov byte ptr [ebp],byte                 │ 0C6HEX, 01000101, 00000000,  byte 
; 1f) mov byte ptr [esp+byte],byte            │ 0C6HEX, 01000100,   024HEX,  byte,  byte
; 1g) mov byte ptr [ereg+byte],byte           │ 0C6HEX, 01000reg,     byte,  byte
; 1h) mov byte ptr [erega+eregb+byte],byte    │ 0C6HEX, 01000100, 00rgBrgA,  byte,  byte
;
; 1i) mov byte ptr [ereg+dword],byte          │ 0C6HEX, 10000reg,    dword,  byte
; 1j) mov byte ptr [ESP+dword],byte           │ 0C6HEX, 10000100,   024HEX, dword,  byte
; 1k) mov byte ptr [erega+eregb+dword],byte   │ 0C6HEX, 10000100, 00rgBrgA, dword,  byte
;
;  2) mov reg,byte                            │ 0C6HEX, 11000reg,     byte
;
;  По способу разбора совпадают с "0FFh", отличаются лишь
; размером (больше на 1 байт)
;
 @0C6_disasm:   lodsb
                sub    ecx,ecx
;
                push   eax
                push   edx
;
                sub    ah,ah
                cmp    al,00111111b
                jbe    @0C6_disasm_test
;                 
                mov    ah,01000000b
                cmp    al,01111111b                              
                jbe    @0C6_disasm_test
;
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @0C6_disasm_test
;
                mov    ah,11000000b
 @0C6_disasm_test:
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    al,000b
                pop    edx
                pop    eax
                jnz    unknown_instr
;
                inc    ecx                  ; увеличим размер
                cmp    al,11000000b
                jb     @0FF_disasm_test_x   ; 1 (i - k)
;
;  2) mov reg,byte                          │ 0C6HEX, 11000reg,     byte
;
                add    cl,2
                jmp    disasm_ret