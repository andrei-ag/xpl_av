.code
;──────────────────────────────────────────────────────────────────────────────
;
 real_off       proc    voff: dword
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx ebx esi
                mov     eax,voff                 ; смещение инструкции в памяти
                sub     eax,4 ptr [ph.ImageBase] ; смещение без ImageBase
;
                sub     ecx,ecx
                mov     cx,word ptr [ph.NOfSections]
                inc     ecx
                mov     esi,offset all_ot        ; esi - данные о секциях
 real_off_lp:   mov     ebx,dword ptr [esi+012]  ; ebx = VirtRVA
                cmp     eax,ebx
                jc      real_off_next            ; jump if OFFSET < VirtRVA
                add     ebx,dword ptr [esi+008]  ; ebx = VirtRVA + VirtSize
                cmp     eax,ebx
                jbe     found_ot                 ; jump if OFFSET <= VirtRVA+VirtSize
                add     esi,ot_size              ; esi - данные следующей секции
 real_off_next: loop    real_off_lp
;───────────────
 found_ot:      sub     eax,dword ptr [esi+012d]
                add     eax,dword ptr [esi+020d]
                add     eax,offset buffer
                pop     esi ebx edx ecx
                ret
 endp
