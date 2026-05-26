;══════════════════════
; mov al,byte ptr [dword]               │ 0A0HEX,    dword
; mov eax,dword ptr [dword]             │ 0A1HEX,    dword
; mov byte ptr [dword],al               │ 0A2HEX,    dword
; mov dword ptr [dword],eax             │ 0A3HEX,    dword
;
 @0AX_disasm:
                inc    edx
                add    cl,1+4
                jmp    disasm_ret
