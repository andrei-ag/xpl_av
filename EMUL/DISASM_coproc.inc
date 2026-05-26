;
; Разбор инструкций сопроцессора
;

;══════════════════════
;  fcom   st(1)               │ 0D8HEX, D1
;  fcompp st(1)               │ 0D8HEX, D9
;
 @0D8_disasm:   lodsb
                cmp    al,0D1h                 ; 
                jz    _@two_copro              ; fcom st(1)
                cmp    al,0D9h                 ; 
                jz    _@two_copro              ; fcompp st(1)
                jmp    unknown_instr               
;══════════════════════
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
;  fucompp                  │ 0DAHEX, E9 
;
 @0DA_disasm:   lodsb
                cmp    al,0E9h
                jz    _@two_copro              ; fucompp
                jmp    unknown_instr               
;══════════════════════
;  fclex                    │ 0DBHEX, E2 
;  finit                    │ 0DBHEX, E3
;
 @0DB_disasm:   lodsb
                cmp    al,0E2h
                jz    _@two_copro              ; fclex
                cmp    al,0E3h                  
                jz    _@two_copro              ; finit
                jmp    unknown_instr               
;══════════════════════
;  ffree                    │ 0DDHEX, C1 
;  fst st(1)                │ 0DDHEX, D1 
;  fstp st(1)               │ 0DDHEX, D9 
;  fucom st(1)              │ 0DDHEX, E1 
;  fucomp st(1)             │ 0DDHEX, E9 
;
 @0DD_disasm:   lodsb
                cmp    al,0C1h
                jz    _@two_copro              ; ffree
                cmp    al,0D1h
                jz    _@two_copro              ; fst st(1)
                cmp    al,0D9h
                jz    _@two_copro              ; fstp st(1)
                cmp    al,0E1h
                jz    _@two_copro              ; fucom st(1)
                cmp    al,0E9h
                jz    _@two_copro              ; fucomp st(1)
                jmp    unknown_instr               
;══════════════════════
;  faddp st(1),st           │ 0DEHEX, C1
;  fmulp st(1),st           │ 0DEHEX, C9
;  fcompp                   │ 0DEHEX, D9
;  fsubrp st(1),st(0)       │ 0DEHEX, E1
;  fsubp st(1),st(0)        │ 0DEHEX, E9
;  fdivrp st(1),st          │ 0DEHEX, F1
;  fdivp st(1),st(0)        │ 0DEHEX, F9
;
 @0DE_disasm:   lodsb
                cmp    al,0C1h                  
                jz    _@two_copro              ; faddp st(1),st
                cmp    al,0C9h                  
                jz    _@two_copro              ; fmulp st(1),st
                cmp    al,0D9h                  
                jz    _@two_copro              ; fcompp
                cmp    al,0E1h                  
                jz    _@two_copro              ; fsubrp st(1),st(0)
                cmp    al,0E9h                  
                jz    _@two_copro              ; fsubp st(1),st(0)
                cmp    al,0F1h                  
                jz    _@two_copro              ; fdivrp st(1),st(0)
                cmp    al,0F9h                  
                jz    _@two_copro              ; fdivp st(1),st(0)
                jmp    unknown_instr 
;
; инструкции сопроцессора размером в 2 байта
;
 _@two_copro:   jz    _@two_bytes              ;              