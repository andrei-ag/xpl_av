;──────────────────────────────────────────────────────────────────────────────
; get_pe_ep - определяет точку входа в PE файл
;
; on start :
; on exit  : eax = 0 - точка входа не найдена
;                      ИНАЧЕ
;            eax     - entry point
;
 proc get_pe_ep         
;──────────────────────────────────────────────────────────────────────────────
                push    edi esi ebp ebx edx ecx
;
                mov     esi,dword [file_map_address]
                add     esi,dword [ph_loc]       ; ESI = Начало заголовка PE
;───────────────
; рассчитаем расположение данных о секциях в файле
; 018hex + смещение PE-заголовка + [Size of optional header] = XXX
;
 sec_data_loc:  sub     edx,edx
                mov     dx,word [esi+014h]       ; Size of optional header
                add     edx,018H
                add     edx,esi                  ; edx = XXX (см. выше)
                xchg    edi,edx

 set_to_nfo:    sub     ecx,ecx
                mov     cx,word [esi+006h]       ; eax - число секций в PE
;───────────────
; проверим все секции на наличие в них точки входа
;
 read_sec_loop: push    ecx
 check_sec_nfo: push    dword [esi+028h]         ; Entry point
                push    edi
                call    check_sec_ep             ; проверим, точка входа в этой
                or      edx,edx                  ; секции ?
                jz      next_sec                 ; если нет, проверим следующую
;═══════════════
; расчитаем точку входа относительно начала файла
; [_ph+028h] (EntryPoint) - [_ot+012d] (VirtRVA) + [_ot+020d] (PhOffset) = AAA
;
                mov     eax,dword [esi+028h]
                sub     eax,dword [edi+012d]
                add     eax,dword [edi+020d]     ; eax = AAA
                pop     ecx
                jmp     get_pe_ep_ret            ; выйдем
;───────────────
 next_sec:      pop     ecx
                add     edi,ot_size
                loop    read_sec_loop
                sub     eax,eax                  ; eax = 0 - не нашли entry point
;═══════════════════════
 get_pe_ep_ret: pop     ecx edx ebx ebp esi edi
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; находится ли точка входа в секции, данные о которой содержатся в "ot"
;
; если значение EntryPoint находится в промежутке от
; VirtRVA и до VirtRVA+VirtSize, то точка входа находится именно в этой секции
;
; on exit: edx == 0 - точка входа не в секции
;          edx != 0 - точка входа именно в этой секции
;
 proc check_sec_ep      _ot, off
;──────────────────────────────────────────────────────────────────────────────
                mov     eax,[off]
                mov     edi,[_ot]
                mov     edx,dword [edi+012d]
                push    edx
                add     edx,dword [edi+008d]
                cmp     eax,edx
                pop     edx
                jge     no_ip_in_sec     ; if EntryPoint < (VirtRVA + VirtSize)
                cmp     eax,edx
                jge     check_ip_ret     ; if EntryPoint >= VirtRVA
 no_ip_in_sec:  sub     edx,edx
 check_ip_ret:  ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────