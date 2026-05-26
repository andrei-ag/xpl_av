;══════════════════════
; 1a) lea eregb,dword ptr [erega]            │ 08DHEX, 00rgBrgA
; 1b) lea ereg,dword ptr [esp]               │ 08DHEX, 00reg100,   024HEX
; 1c) lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
; 1d) lea ereg,dword ptr [dword]             │ 08DHEX, 00reg101,    dword
;
;     lea ereg,dword ptr [ereg*2]            │ 08DHEX, 00reg100, 01reg101, 00000000
;     lea ereg,dword ptr [ereg*2+dword]      │ 08DHEX, 00reg100, 01reg101,    dword
;     lea ereg,dword ptr [ereg*4]            │ 08DHEX, 00reg100, 10reg101, 00000000
;     lea ereg,dword ptr [ereg*4+dword]      │ 08DHEX, 00reg100, 10reg101,    dword
;     lea ereg,dword ptr [ereg*8]            │ 08DHEX, 00reg100, 11reg101, 00000000
;     lea ereg,dword ptr [ereg*8+dword]      │ 08DHEX, 00reg100, 11reg101,    dword
;
;     lea ereg,dword ptr [erega+eregb*2]     │ 08DHEX, 00reg100, 01rgBrgA
;     lea ereg,dword ptr [erega+eregb*4]     │ 08DHEX, 00reg100, 10rgBrgA
;     lea ereg,dword ptr [erega+eregb*8]     │ 08DHEX, 00reg100, 11rgBrgA
;
; 2a) lea ereg,dword ptr [ebp]               │ 08DHEX, 01reg101, 00000000 
; 2b) lea ereg,dword ptr [esp+byte]          │ 08DHEX, 01reg100,   024HEX, byte
; 2c) lea eregb,dword ptr [erega+byte]       │ 08DHEX, 01rgBrgA,     byte
; 2d) lea ereg,dword ptr [erega+eregb+byte]  │ 08DHEX, 01reg100, 00rgBrgA, byte 
;
; 3a) lea eregb,dword ptr [erega+dword]      │ 08DHEX, 10rgBrgA,    dword
; 3b) lea ereg,dword ptr [ESP+dword]         │ 08DHEX, 10reg100,   024HEX, dword
; 3c) lea ereg,dword ptr [erega+eregb+dword] │ 08DHEX, 10reg100, 00rgBrgA, dword 
;
;  4) lea erega, eregb                       │ 08DHEX, 11rgBrgA
;
; По размеру и строению аналогичны "089H"
;
 @08D_disasm:     
                sub    ecx,ecx
                sub    edx,edx
;
                lodsb
                cmp    al,11000000b
                jae    _@two_bytes           ; = lea erega, eregb
;
; 1a) lea eregb,dword ptr [erega]            │ 08DHEX, 00rgBrgA
; 1b) lea ereg,dword ptr [esp]               │ 08DHEX, 00reg100,   024HEX
; 1c) lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
;     lea ereg,dword ptr [ereg*?]            │ 08DHEX, 00reg100, ??reg101, 00000000
;     lea ereg,dword ptr [ereg*?+dword]      │ 08DHEX, 00reg100, ??reg101,    dword
;     lea ereg,dword ptr [erega+eregb*?]     │ 08DHEX, 00reg100, ??rgBrgA
; 1d) lea ereg,dword ptr [dword]             │ 08DHEX, 00reg101,    dword
;
                cmp    al,00111111b
                jbe    @08D_disasm_1         ; one of listed instructions
;
; 2a) lea ereg,dword ptr [ebp]               │ 08DHEX, 01reg101, 00000000 
; 2b) lea ereg,dword ptr [esp+byte]          │ 08DHEX, 01reg100,   024HEX, byte
; 2c) lea eregb,dword ptr [erega+byte]       │ 08DHEX, 01rgBrgA,     byte
; 2d) lea ereg,dword ptr [erega+eregb+byte]  │ 08DHEX, 01reg100, 00rgBrgA, byte 
;
                mov    ah,01000000b
                cmp    al,01111111b
                jbe    @08D_disasm_ereg_byte ; one of listed instructions
;
; 3a) lea eregb,dword ptr [erega+dword]      │ 08DHEX, 10rgBrgA,    dword
; 3b) lea ereg,dword ptr [ESP+dword]         │ 08DHEX, 10reg100,   024HEX, dword
; 3c) lea ereg,dword ptr [erega+eregb+dword] │ 08DHEX, 10reg100, 00rgBrgA, dword 
;
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @08D_disasm_ereg_dword ; one of listed instructions
;══════════════════════
; 1a) lea eregb,dword ptr [erega]            │ 08DHEX, 00rgBrgA
; 1b) lea ereg,dword ptr [esp]               │ 08DHEX, 00reg100,   024HEX
; 1c) lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
;     lea ereg,dword ptr [ereg*?]            │ 08DHEX, 00reg100, ??reg101, 00000000
;     lea ereg,dword ptr [ereg*?+dword]      │ 08DHEX, 00reg100, ??reg101,    dword
;     lea ereg,dword ptr [erega+eregb*?]     │ 08DHEX, 00reg100, ??rgBrgA
; 1d) lea ereg,dword ptr [dword]             │ 08DHEX, 00reg101,    dword
;
 @08D_disasm_1: call   two_regs
                mov    edi,eax               ; EDI = EREG
; 
                cmp    dl,101b
                jz     @08D_disasm_dword     ; = lea ereg, dword ptr [dword]
                cmp    dl,100b
                jz     @08D_disasm_esp_OR_2eregs
;══════════════════════
; 1a) lea eregb,dword ptr [erega]            │ 08DHEX, 00rgBrgA
;
 @08D_disasm_ereg:                          
                xchg   eax,edx
                call   set_dreg_eax
;
; Если в регистре EREGA (инструкция вида "lea eregb,dword ptr [EREGA]")
; содержится delta value или он указывает на виртуальный стек, то после
; выполнения инструкции, регистр EREGB так же будет содержать
; delta value или указывать на стек
;
; По этому мы переносим тип значения, содержащегося в регистре EREGB, в
; регистр EREGB (тип значения хранится в массиве "_regs") 
; 
                push   ecx
                mov    cl,byte [eax+_regs]   ; получим тип значения EREGB
                mov    byte [edi+_regs],cl   ; установим тип значения EREGA                        
                pop    ecx
                add    cl,2
                jmp    disasm_ret
;══════════════════════
; 1b) lea ereg,dword ptr [esp]               │ 08DHEX, 00reg100,   024HEX
; 1c) lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
;     lea ereg,dword ptr [ereg*?]            │ 08DHEX, 00reg100, ??reg101, 00000000
;     lea ereg,dword ptr [ereg*?+dword]      │ 08DHEX, 00reg100, ??reg101,    dword
;     lea ereg,dword ptr [erega+eregb*?]     │ 08DHEX, 00reg100, ??rgBrgA
;
 @08D_disasm_esp_OR_2eregs:
                inc    ecx
                cmp    byte [esi],24h
                jz     @08D_disasm_ereg     ; = lea ereg,dword ptr [esp]
;
                sub    eax,eax
                lodsb
                cmp    al,00111111b
                jbe    @08D_disasm_2eregs   ;  = lea ereg,dword ptr [erega+eregb]
;══════════════════════
;     lea ereg,dword ptr [ereg*?]            │ 08DHEX, 00reg100, ??reg101, 00000000
;     lea ereg,dword ptr [ereg*?+dword]      │ 08DHEX, 00reg100, ??reg101,    dword
;     lea ereg,dword ptr [erega+eregb*?]     │ 08DHEX, 00reg100, ??rgBrgA
;
 @08D_disasm_mul:          
                sub    edx,edx               ; EDX - множитель для регистра
                add    dl,2
;
                mov    ah,01000000b
                cmp    al,01111111b
                jbe    @08D_disasm_mul_process ; ereg*2
;
                add    dl,2
                mov    ah,10000000b
                cmp    al,10111111b
                jbe    @08D_disasm_mul_process ; ereg*6
;
                add    dl,4
                mov    ah,11000000b            ; ereg*8
 @08D_disasm_mul_process:  
                push   edx
                xor    al,ah
                sub    ah,ah
                call   two_regs
                cmp    dl,101b
                jnz    @08D_disasm_mul_two_eregs
;══════════════════════
;     lea ereg,dword ptr [erega*?]           │ 08DHEX, 00reg100, ??reg101, 00000000
;     lea ereg,dword ptr [erega*?+dword]     │ 08DHEX, 00reg100, ??reg101,    dword
;
 @08D_disasm_mul_one_ereg:
; ***
;
; Если в регистре EREGA или EREGB (инструкция вида
; "lea ereg,dword ptr [erega+eregb*?...]") содержится delta value или
; он указывает на виртуальный стек, то после выполнения инструкции,
; регистр EREGB так же будет содержать delta value или указывать на стек
;
; По этому мы переносим тип значения, содержащегося в регистре EREGB, в
; регистр EREGB (тип значения хранится в массиве "_regs") 
; 
                push   ecx
                mov    cl,byte [eax+_regs]   ; получим тип значения EREGB
                mov    byte [edi+_regs],cl   ; установим тип значения EREGA                        
                pop    ecx
; ***
                add    cl,4+2                ; Instruction size

                mov    edi,dregs+004
                jmp    @08D_disasm_mul_move_ereg
;══════════════════════
;     lea ereg,dword ptr [erega+eregb*?]     │ 08DHEX, 00reg100, ??rgBrgA
;
 @08D_disasm_mul_two_eregs:
; ***
;
; Если в регистре EREGA (инструкция вида "lea eregb,dword ptr [erega*?...]")
; содержится delta value или он указывает на виртуальный стек, то после
; выполнения инструкции, регистр EREGB так же будет содержать
; delta value или указывать на стек
; 
                push   ecx
                mov    cl,byte [eax+_regs]   ; получим тип значения EREGB
                cmp    cl,0
                jnz    @08D_disasm_mul_two_eregs_set_reg_type
                mov    cl,byte [edx+_regs]
;
 @08D_disasm_mul_two_eregs_set_reg_type:
                mov    byte [edi+_regs],cl   ; установим тип значения EREGA                        
                pop    ecx
; ***
                add    cl,2                  ; + to instr. size
                inc    dword [dregs]
                mov    dword [dregs+004],edx ; Store "EREGA"
;
                mov    edi,dregs+008
; 
 @08D_disasm_mul_move_ereg:
                pop    edx                   ; EDI - множитель
                add    dword [dregs],edx
                push   ecx
                xchg   ecx,edx
 @08D_disasm_mul_two_eregs_loop:
                stosd                     ; Store "EREGB" or "EREG" ID
                loop   @08D_disasm_mul_two_eregs_loop

                pop    ecx
                sub    edx,edx
                inc    edx                 ; Turn on full emulation flag
                jmp    disasm_ret
;══════════════════════
; 1c) lea ereg,dword ptr [erega+eregb]       │ 08DHEX, 00reg100, 00rgBrgA 
;
 @08D_disasm_2eregs:
                call   two_regs
; ***
;
; Если в регистре EREGA (инструкция вида "lea ereg,dword ptr [erega+eregb]")
; содержится delta value или он указывает на виртуальный стек, то после
; выполнения инструкции, регистр EREG так же будет содержать
; delta value или указывать на стек
; 
                push   ecx
                mov    cl,byte [eax+_regs]   ; получим тип значения EREGB
                cmp    cl,0
                jnz    @08D_disasm_2eregs_set_reg_type
                mov    cl,byte [edx+_regs]
;
 @08D_disasm_2eregs_set_reg_type:
                mov    byte [edi+_regs],cl   ; установим тип значения EREGA                        
                pop    ecx
; ***
                inc    dword [dregs]
                mov    dword [dregs+008],edx
                call   set_dreg_eax
                add    cl,2
                jmp    disasm_ret
;══════════════════════
; 1d) lea ereg,dword ptr [dword]             │ 08DHEX, 00reg101,    dword
;
 @08D_disasm_dword:
                inc    edx
                add    cl,2+4
                jmp    disasm_ret
;══════════════════════
; 2a) lea ereg,dword ptr [ebp]               │ 08DHEX, 01reg101, 00000000 
; 2b) lea ereg,dword ptr [esp+byte]          │ 08DHEX, 01reg100,   024HEX, byte
; 2c) lea eregb,dword ptr [erega+byte]       │ 08DHEX, 01rgBrgA,     byte
; 2d) lea ereg,dword ptr [erega+eregb+byte]  │ 08DHEX, 01reg100, 00rgBrgA, byte 
;
 @08D_disasm_ereg_byte:
                add    cl,3
                xor    al,ah
                sub    ah,ah
                call   two_regs
                mov    edi,eax                 ; EDI = EREG
;
                cmp    dl,100b
                jnz    @08D_disasm_ereg_byte_2 ; НЕ является
                                               ; " lea ereg,dword ptr [esp+byte] "
                inc    ecx
                cmp    byte [esi],024h
                jz     @08D_disasm_ereg_byte_2 ; = " lea dword ptr [esp+byte] "
;══════════════════════
; 2d) lea ereg,dword ptr [erega+eregb+byte]  │ 08DHEX, 01reg100, 00rgBrgA, byte 
;
                lodsb
                cmp    al,00111111b
                ja     unknown_instr       ; ???

                sub    cl,2
                jmp    @08D_disasm_2eregs
;══════════════════════
; 2a) lea ereg,dword ptr [ebp]               │ 08DHEX, 01reg101, 00000000 
; 2b) lea ereg,dword ptr [esp+byte]          │ 08DHEX, 01reg100,   024HEX, byte
; 2c) lea eregb,dword ptr [erega+byte]       │ 08DHEX, 01rgBrgA,     byte
;
 @08D_disasm_ereg_byte_2:
                sub    cl,2
                jmp    @08D_disasm_ereg                          
;══════════════════════
; 3a) lea eregb,dword ptr [erega+dword]      │ 08DHEX, 10rgBrgA,    dword
; 3b) lea ereg,dword ptr [ESP+dword]         │ 08DHEX, 10reg100,   024HEX, dword
; 3c) lea ereg,dword ptr [erega+eregb+dword] │ 08DHEX, 10reg100, 00rgBrgA, dword 
;
 @08D_disasm_ereg_dword:
                add    cl,3
                jmp    @08D_disasm_ereg_byte