;──────────────────────────────────────────────────────────────────────────────
; real_off - Calculate "real offset" (in antivirus buffer) from
;            the originally virtual adress
;
; on start: voff - virtual adress
;
; on exit:   EAX - calculated "real offset"
;
 proc real_off          voff
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx ebx esi
                mov     eax,[voff]               ; смещение инструкции в памяти
;
                mov     esi,dword [file_map_address]
                add     esi,dword [ph_loc]       ; ESI = Начало заголовка PE
;
                sub     eax,dword [esi+034h]     ; смещение без ImageBase
                sub     ecx,ecx
                mov     cx,word [esi+06h]
                inc     ecx

                sub     ebx,ebx
                mov     bx,word [esi+014h]       ; Size of optional header
                add     ebx,018h
                add     esi,ebx                  ; ESI = Данные первой секции
;
 real_off_lp:   mov     ebx,dword [esi+012]      ; ebx = VirtRVA
                cmp     eax,ebx
                jc      real_off_next            ; jump if OFFSET < VirtRVA
                add     ebx,dword [esi+008]      ; ebx = VirtRVA + VirtSize
                cmp     eax,ebx
                jb      found_ot                 ; jump ONLY if OFFSET < VirtRVA+VirtSize
                add     esi,ot_size              ; esi - данные следующей секции
 real_off_next: loop    real_off_lp
;───────────────
 found_ot:      ; EAX - оригинальное смещение в памяти
                ;       далее, внимательно наблюдаем за манипуляциями
                sub     eax,dword [esi+012d] ; EAX - Разница между началом секции и смещением
                add     eax,dword [esi+020d] ; EAX - Положение смещения в файле
                add     eax,dword [file_map_address]
                                                 ; EAX - Смещение в памяти антивируса

                pop     esi ebx edx ecx
                ret
;
 endp