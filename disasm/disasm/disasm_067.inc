;══════════════════════
; Prefix 67h - Change default ADDRESS size.
;
; mov al,byte ptr [word]                │ 067HEX, 0A0HEX,    word
; mov eax,dword ptr [word]              │ 067HEX, 0A1HEX,    word
; mov byte ptr [word],al                │ 067HEX, 0A2HEX,    word
; mov dword ptr [word],eax              │ 067HEX, 0A3HEX,    word
;
; 08B:
;      mov ereg,dword ptr [word]        │ 067HEX, 08BHEX, 00reg101,    word
;
 @067_disasm:   lodsb
                cmp    al,08bh
                jz     @067_disasm_08B
                cmp    al,0a0h
                jb     unknown_instr
                cmp    al,0a3h
                ja     unknown_instr
;
                inc    edx
                add    cl,1+1+2
                jmp    disasm_ret
;
 @067_disasm_08B:
                lodsb
                cmp    al,00000101b
                jb     unknown_instr    ; < mov eax,dword [word]
                cmp    al,00111101b
                ja     unknown_instr    ; > mov edi,dword [word]
;
                call   two_regs
                xchg   edx,eax
                call   set_dreg_eax
                mov    cl,3+2
                jmp    disasm_ret