;
; Разбор инструкций сопроцессора
;

;══════════════════════
; fld  st(?)              │ 0D9HEX, 11000???
; fxch st(?)              │ 0D9HEX, 11001???
; fnop                    │ 0D9HEX, D0
; fchs                    │ 0D9HEX, E0
; fabs                    │ 0D9HEX, E1
; ftst                    │ 0D9HEX, E4
; fxam                    │ 0D9HEX, E5
; fld1                    │ 0D9HEX, E8 
; fldl2t                  │ 0D9HEX, E9
; fldl2e                  │ 0D9HEX, EA
; fldpi                   │ 0D9HEX, EB
; fldln2                  │ 0D9HEX, ED
; fldz                    │ 0D9HEX, EE
; f2xmi                   │ 0D9HEX, F0
; fyl2x                   │ 0D9HEX, F1
; fptan                   │ 0D9HEX, F2
; fpatan                  │ 0D9HEX, F3
; fxtract                 │ 0D9HEX, F4
; fprem1                  │ 0D9HEX, F5
; fdecstp                 │ 0D9HEX, F6
; fincstp                 │ 0D9HEX, F7
; fprem                   │ 0D9HEX, F8
; fyl2xp1                 │ 0D9HEX, F9
; fsqrt                   │ 0D9HEX, FA
; fsincos                 │ 0D9HEX, FB
; frndint                 │ 0D9HEX, FC
; fscale                  │ 0D9HEX, FD
; fsin                    │ 0D9HEX, FE
; fcos                    │ 0D9HEX, FF
;
 @0D9_disasm:   lodsb
                cmp    al,11001111b
                jbe   _@two_copro              ; < fxch st(7)
                cmp    al,0D0h                  
                jz    _@two_copro              ; fnop
                cmp    al,0E0h                  
                jz    _@two_copro              ; fchs
                cmp    al,0E1h                  
                jz    _@two_copro              ; fabs
                cmp    al,0E4h                  
                jz    _@two_copro              ; ftst
                cmp    al,0E5h                  
                jz    _@two_copro              ; fxam
                cmp    al,0E8h                  
                jz    _@two_copro              ; fld1
                cmp    al,0E9h                  
                jz    _@two_copro              ; fldl2t
                cmp    al,0EAh                  
                jz    _@two_copro              ; fldl2e
                cmp    al,0EBh                  
                jz    _@two_copro              ; fldpi
                cmp    al,0EDh                  
                jz    _@two_copro              ; fldln2
                cmp    al,0EEh                  
                jz    _@two_copro              ; fldz
                cmp    al,0F0h                  
                jz    _@two_copro              ; f2xmi
                cmp    al,0F1h                  
                jz    _@two_copro              ; fyl2x
                cmp    al,0F2h                  
                jz    _@two_copro              ; fptan
                cmp    al,0F3h                  
                jz    _@two_copro              ; fpatan
                cmp    al,0F4h                  
                jz    _@two_copro              ; fxtract
                cmp    al,0F5h                  
                jz    _@two_copro              ; fprem1
                cmp    al,0F6h                  
                jz    _@two_copro              ; fdecstp
                cmp    al,0F7h                  
                jz    _@two_copro              ; fincstp
                cmp    al,0F8h                  
                jz    _@two_copro              ; fprem
                cmp    al,0F9h                  
                jz    _@two_copro              ; fyl2xp1
                cmp    al,0FAh                  
                jz    _@two_copro              ; fsqrt
                cmp    al,0FBh                  
                jz    _@two_copro              ; fsincos
                cmp    al,0FCh                  
                jz    _@two_copro              ; frndint
                cmp    al,0FDh                  
                jz    _@two_copro              ; fscale
                cmp    al,0FEh                  
                jz    _@two_copro              ; fsin
                cmp    al,0FFh                  
                jz    _@two_copro              ; fcos
                jmp    unknown_instr               
;══════════════════════
;  fild dword ptr [ereg]    │ 0DBHEX, 00000reg
;  fist dword ptr [ereg]    │ 0DBHEX, 00010reg
;  fclex                    │ 0DBHEX, E2 
;  finit                    │ 0DBHEX, E3
;
 @0DB_disasm:   lodsb
                cmp    al,00000111b
                jbe    @0DB_disasm_fild_fist   ; <= fild dword ptr [edi]
                cmp    al,00010000b
                jc     unknown_instr           ; < fist dword ptr [eax]
                cmp    al,00010111b
                jbe    @0DB_disasm_fild_fist   ; <= fist dword ptr [edi]
;
                cmp    al,0E2h
                jz    _@two_copro              ; fclex
                cmp    al,0E3h                  
                jz    _@two_copro              ; finit
                jmp    unknown_instr
;
 @0DB_disasm_fild_fist: 
                call   two_regs
                cmp    dl,100b
                jz     unknown_instr           ; EREG = ESP, пока не разбираем
                cmp    dl,101b
                jz     unknown_instr           ; EREG = EBP, пока не разбираем
                xchg   eax,edx
                call   set_dreg_eax
                add    cl,2
                jmp    disasm_ret
;══════════════════════
;  ffree st(?)              │ 0DDHEX, 11000???b
;  ???                      │ 0DDHEX, 11001???b
;  fst st(?)                │ 0DDHEX, 11010???b 
;  fstp st(?)               │ 0DDHEX, 11011???b 
;  fucom st(?)              │ 0DDHEX, 11100???b 
;  fucomp st(?)             │ 0DDHEX, 11101???b
;  fsave ereg               │ 0DDHEX, 11110REGb
;  fstsw ereg               │ 0DDHEX, 11111REGb
;
 @0DD_disasm:   lodsb
                cmp    al,11000000b
                jc     unknown_instr           ; < ffree st(0)
                cmp    al,11000111b
                jbe    _@two_copro             ; <= ffree st(7)
;
                cmp    al,11010000b
                jc     unknown_instr           ; < fst st(0)
                ;jmp    _@two_copro               
;
; инструкции сопроцессора размером в 2 байта
;
 _@two_copro:   jmp   _@two_bytes              ;              