;══════════════════════                                                         
; 1a) xxx dword ptr [ereg]                │ 0FFHEX, 00XXXreg
; 1b) xxx dword ptr [esp]                 │ 0FFHEX, 00XXX100,   024HEX
; 1c) xxx dword ptr [erega+eregb]         │ 0FFHEX, 00XXX100, 00rgBrgA 
;
;     xxx dword ptr [ereg*2]              │ 0FFHEX, 00XXX100, 01reg101, 00000000                                
;     xxx dword ptr [ereg*2+dword]        │ 0FFHEX, 00XXX100, 01reg101,    dword
;     xxx dword ptr [ereg*4]              │ 0FFHEX, 00XXX100, 10reg101, 00000000                             
;     xxx dword ptr [ereg*4+dword]        │ 0FFHEX, 00XXX100, 10reg101,    dword
;     xxx dword ptr [ereg*8]              │ 0FFHEX, 00XXX100, 11reg101, 00000000                                
;     xxx dword ptr [ereg*8+dword]        │ 0FFHEX, 00XXX100, 11reg101,    dword
;
;     xxx dword ptr [erega+eregb*2]       │ 0FFHEX, 00XXX100, 01rgBrgA
;     xxx dword ptr [erega+eregb*4]       │ 0FFHEX, 00XXX100, 10rgBrgA
;     xxx dword ptr [erega+eregb*8]       │ 0FFHEX, 00XXX100, 11rgBrgA
;                                
; 1d) xxx dword ptr [dword]               │ 0FFHEX, 00XXX101,    dword
;
; 1e) xxx dword ptr [ebp]                 │ 0FFHEX, 01XXX101, 00000000 
; 1f) xxx dword ptr [esp+byte]            │ 0FFHEX, 01XXX100,   024HEX,  byte
; 1g) xxx dword ptr [ereg+byte]           │ 0FFHEX, 01XXXreg,     byte
; 1h) xxx dword ptr [erega+eregb+byte]    │ 0FFHEX, 01XXX100, 00rgBrgA,  byte 
;
; 1i) xxx dword ptr [ereg+dword]          │ 0FFHEX, 10XXXreg,    dword
; 1j) xxx dword ptr [ESP+dword]           │ 0FFHEX, 10XXX100,   024HEX, dword
; 1k) xxx dword ptr [erega+eregb+dword]   │ 0FFHEX, 10XXX100, 00rgBrgA, dword
;
;     xxx dword ptr [erega+eregb*2+dword] │ 0FFHEX, 10XXX100, 01rgBrgA, dword
;     xxx dword ptr [erega+eregb*4+dword] │ 0FFHEX, 10XXX100, 10rgBrgA, dword
;     xxx dword ptr [erega+eregb*8+dword] │ 0FFHEX, 10XXX100, 11rgBrgA, dword
;
;  2) xxx ereg                            │ 0FFHEX, 11XXXreg
;
; LEGEND: 000 - INC, 001 - DEC, 010 - CALL, 011 - CALL FAR,
;         100 - JMP, 101 - ???, 110 - PUSH, 111 - ??? 
;
;
 @0FF_disasm:   
                lodsb
                sub    ecx,ecx
                push   eax
;
                call   set_off_first_two_bits
                call   two_regs
;
                cmp    al,101b
                jz     @0FF_disasm_unknown
                cmp    al,111b
                jz     @0FF_disasm_unknown
                pop    eax
                cmp    al,11000000b
                jc     @0FF_disasm_test_x
;
;  2) xxx ereg                            │ 0FFHEX, 11XXXreg
;
		xor    al,11000000b
                call   two_regs
;
                cmp    al,010b
                jb     @0FF_disasm_xxx_ereg_not_spec ; < CALL
                cmp    al,100b
                ja     @0FF_disasm_xxx_ereg_not_spec ; > JMP
;
                xchg   eax,edx                       ; CALL, CALL FAR, JMP
                call   set_dreg_eax
                jmp    _@two_spec
;
; INC, DEC, PUSH
;
 @0FF_disasm_xxx_ereg_not_spec:
                sub    edx,edx
                jmp    _@two_bytes
;
 @0FF_disasm_unknown:
                pop    eax
                jmp    unknown_instr
;
; XXX instructions
;
 @0FF_disasm_test_x:
;
; 1a) xxx dword ptr [ereg]                │ 0FFHEX, 00XXXreg
; 1b) xxx dword ptr [esp]                 │ 0FFHEX, 00XXX100,   024HEX
; 1c) xxx dword ptr [erega+eregb]         │ 0FFHEX, 00XXX100, 00rgBrgA 
;     xxx dword ptr [ereg*2/4/8]          │ 0FFHEX, 00XXX100, ??reg101, 00000000                                
;     xxx dword ptr [ereg*2/4/8+dword]    │ 0FFHEX, 00XXX100, ??reg101,    dword
;     xxx dword ptr [erega+eregb*2/4/8]   │ 0FFHEX, 00XXX100, ??rgBrgA
; 1d) xxx dword ptr [dword]               │ 0FFHEX, 00XXX101,    dword
;
                cmp    al,00111111b
                jbe    @0FF_disasm_1        ;    xxx dword ptr [ereg]
                                            ; OR xxx dword ptr [erega+eregb]
                                            ; OR xxx dword ptr [dword]
;
; 1e) xxx dword ptr [ebp]                 │ 0FFHEX, 01XXX101, 00000000 
; 1f) xxx dword ptr [esp+byte]            │ 0FFHEX, 01XXX100,   024HEX,  byte
; 1g) xxx dword ptr [ereg+byte]           │ 0FFHEX, 01XXXreg,     byte
; 1h) xxx dword ptr [erega+eregb+byte]    │ 0FFHEX, 01XXX100, 00rgBrgA,  byte 
;
                mov    ah,01000000b
                cmp    al,01111111b
                jbe    @0FF_disasm_ereg_byte ;    xxx dword ptr [ereg+byte]
                                             ; OR xxx dword ptr [erega+eregb+byte]
;
; 1i) xxx dword ptr [ereg+dword]              │ 0FFHEX, 10XXXreg,    dword
; 1j) xxx dword ptr [ESP+dword]               │ 0FFHEX, 10XXX100,   024HEX, dword
; 1k) xxx dword ptr [erega+eregb+dword]       │ 0FFHEX, 10XXX100, 00rgBrgA, dword
;     xxx dword ptr [erega+eregb*2/4/8+dword] │ 0FFHEX, 10XXX100, ??rgBrgA, dword
;
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @0FF_disasm_ereg_dword ;    xxx dword ptr [ereg+dword]
                                              ; OR xxx dword ptr [erega+eregb+dword]
;══════════════════════
; 1a) xxx dword ptr [ereg]                │ 0FFHEX, 00XXXreg
; 1b) xxx dword ptr [esp]                 │ 0FFHEX, 00XXX100,   024HEX
; 1c) xxx dword ptr [erega+eregb]         │ 0FFHEX, 00XXX100, 00rgBrgA 
; 1d) xxx dword ptr [dword]               │ 0FFHEX, 00XXX101,    dword
;
 @0FF_disasm_1: call   two_regs
                cmp    dl,101b
                jz     @0FF_disasm_dword    ; = xxx dword ptr [dword]
                cmp    dl,100b
                jz     @0FF_disasm_esp_OR_2eregs
;
 @0FF_disasm_ereg:                          ; = xxx dword ptr [erega]
                xchg   eax,edx
                call   set_dreg_eax
                add    cl,2
                jmp    disasm_ret
;
 @0FF_disasm_esp_OR_2eregs:
                inc    ecx
                cmp    byte [esi],24h
                jz     @0FF_disasm_ereg     ; = xxx dword ptr [esp]
;
                sub    eax,eax              ;  = xxx dword ptr [erega+eregb]
                                            ; OR xxx dword ptr [ereg*2/4/8]
                                            ; OR xxx dword ptr [ereg*2/4/8+dword]
                                            ; OR xxx dword ptr [erega+eregb*2/4/8]
                lodsb
                cmp    al,00111111b
                jbe    @0FF_disasm_2eregs   ;  = xxx dword ptr [erega+eregb]
;
;     xxx dword ptr [ereg*2/4/8]          │ 0FFHEX, 00XXX100, ??reg101, 00000000                                
;     xxx dword ptr [ereg*2/4/8+dword]    │ 0FFHEX, 00XXX100, ??reg101,    dword
;     xxx dword ptr [erega+eregb*2/4/8]   │ 0FFHEX, 00XXX100, ??rgBrgA
;
 @0FF_disasm_mul:
                call   get_mul_arg
;
                mov    edi,dregs+004
                add    cl,4+2                  ; Instruction size
;
                push   edx
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    dl,101b
                jz     @0FF_disasm_mul_process_2
;
;     xxx dword ptr [erega+eregb*2/4/8]   │ 0FFHEX, 00XXX100, ??rgBrgA
;
                inc    dword [dregs]
                mov    dword [dregs+004],edx   ; Store "EREGA"
                add    edi,4
                sub    cl,4                    ; This instr. type is shorter 
 @0FF_disasm_mul_process_2:
                pop    edx
                add    dword [dregs],edx
                push   ecx
                xchg   ecx,edx
 @0FF_disasm_mul_loop:
                stosd                     ; Store "EREGB" or "EREG" ID
                loop   @0FF_disasm_mul_loop

                pop    ecx
                sub    edx,edx
                inc    edx                 ; Turn on full emulation flag
                jmp    disasm_ret
;
; 1c) xxx dword ptr [erega+eregb]         │ 0FFHEX, 00XXX100, 00rgBrgA 
;
 @0FF_disasm_2eregs:
                call   two_regs
                inc    dword [dregs]
                mov    dword [dregs+008],edx
                call   set_dreg_eax
                add    cl,2
                jmp    disasm_ret
;
 @0FF_disasm_dword:                         ; = xxx dword ptr [dword]
                inc    edx
                add    cl,2+4
                jmp    disasm_ret
;
; 1e) xxx dword ptr [ebp]                 │ 0FFHEX, 01XXX101, 00000000 
; 1f) xxx dword ptr [esp+byte]            │ 0FFHEX, 01XXX100,   024HEX,  byte
; 1g) xxx dword ptr [ereg+byte]           │ 0FFHEX, 01XXXreg,     byte
; 1h) xxx dword ptr [erega+eregb+byte]    │ 0FFHEX, 01XXX100, 00rgBrgA,  byte 
;
 @0FF_disasm_ereg_byte:
                add    cl,3
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    dl,100b
                jnz    @0FF_disasm_ereg_byte_2 ; НЕ является
                                               ; " xxx dword ptr [esp+byte] "
                inc    ecx
                cmp    byte [esi],024h
                jz     @0FF_disasm_ereg_byte_2 ; = xxx dword ptr [esp+byte]
;
; xxx dword ptr [erega+eregb + byte]
;
                lodsb
                cmp    al,00111111b
                ja     @0FF_disasm_mul_2       ; xxx dword ptr [erega+eregb*2/4/8+dword]
;
                sub    cl,2
                jmp    @0FF_disasm_2eregs
;
;     xxx dword ptr [erega+eregb*2/4/8+dword] │ 0FFHEX, 10XXX100, ??rgBrgA, dword
;
 @0FF_disasm_mul_2:
                call   get_mul_arg
;
                push   edx                ; MUL arg
                inc    edx
                mov    dword [dregs],edx  ; Set number of registers
;
                xor    al,ah
                sub    ah,ah
                call   two_regs
;
                mov    edi,dregs+004
                xchg   eax,edx            
                stosd                     ; Store "EREGA"
                xchg   edx,eax
;
                pop    ecx
 @0FF_disasm_mul_2_loop:
                stosd                     ; Store "EREGB"
                loop   @0FF_disasm_mul_2_loop
;
                sub    ecx,ecx
                mov    cl,7
;
                sub    edx,edx
                inc    edx                 ; Turn on full emulation flag
                jmp    disasm_ret
;
; " xxx dword ptr [ereg+byte] " or " xxx dword ptr [esp+byte] "
;
 @0FF_disasm_ereg_byte_2:
                sub    cl,2
                jmp    @0FF_disasm_ereg
;
; 1i) xxx dword ptr [ereg+dword]              │ 0FFHEX, 10XXXreg,    dword
; 1j) xxx dword ptr [ESP+dword]               │ 0FFHEX, 10XXX100,   024HEX, dword
; 1k) xxx dword ptr [erega+eregb+dword]       │ 0FFHEX, 10XXX100, 00rgBrgA, dword
;     xxx dword ptr [erega+eregb*2/4/8+dword] │ 0FFHEX, 10XXX100, ??rgBrgA, dword
;
 @0FF_disasm_ereg_dword:
                add    cl,3
                jmp    @0FF_disasm_ereg_byte
;══════════════════════                                                         
;
; Get MUL arg for:
;
; xxx dword ptr [ereg*2/4/8]              │ 0FFHEX, 00XXX100, ??reg101, 00000000                                
; xxx dword ptr [ereg*2/4/8+dword]        │ 0FFHEX, 00XXX100, ??reg101,    dword
; xxx dword ptr [erega+eregb*2/4/8]       │ 0FFHEX, 00XXX100, ??rgBrgA
; xxx dword ptr [erega+eregb*2/4/8+dword] │ 0FFHEX, 10XXX100, ??rgBrgA, dword
;
; LEGEND: 01XXXXXX = * 2
;         10XXXXXX = * 4
;         11XXXXXX = * 8
;
; on start:  AL = "???reg101" or "???rgBrgA"
; on exit : EDX = ARG
;
 proc get_mul_arg
                sub    edx,edx
                add    dl,2
;
                mov    ah,01000000b
                cmp    al,01111111b
                jbe    get_mul_arg_ret ; ereg*2
;
                add    dl,2
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    get_mul_arg_ret ; ereg*6
;
                add    dl,4
                mov    ah,11000000b    ; ereg*8
;
 get_mul_arg_ret:
                ret
 endp