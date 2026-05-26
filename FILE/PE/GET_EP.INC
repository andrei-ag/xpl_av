.code
;──────────────────────────────────────────────────────────────────────────────
; get_pe_ep - определяет точку входа в PE файл
;
; on start :     _ph - даные о заголовке PE
;              fhndl - номер открытого файла
; on exit  : eax = 0 - точка входа не найдена
;                      ИНАЧЕ
;            eax     - entry point
;             ot_loc - смещение данных о секции, в которой
;                      расположена точка входа в буфере "all_ot"
;             ot_1st - смещение данных о первой секции
;                      относительно файла
;
 get_pe_ep      proc   fhndl:dword, _ph:dword
;──────────────────────────────────────────────────────────────────────────────
                push    edi esi ebp ebx edx ecx
                mov     esi,_ph
                mov     edi,offset all_ot
;───────────────
; рассчитаем расположение данных о секциях в файле
; 018hex + смещение PE-заголовка PE + [_ph+014h] = XXX
;
 sec_data_loc:  sub     edx,edx
                mov     dx,word ptr [esi+014h]
                add     edx,018H
                add     edx,dword ptr [ph_loc]   ; edx = XXX (см. выше)

 set_to_nfo:    mov     dword ptr [ot_1st],edx   ; смещение данных о 1ой секции
                mov     dword ptr [ot_loc],edi
                push    FILE_BEGIN 0 edx fhndl   ; установим указатель на
                call    SetFilePointer           ; данные о секциях PE

                sub     eax,eax
                mov     ax,word ptr [esi+006h]   ; eax - число секций в PE
                push    eax
                mov     edx,ot_size              ; edx - размер данных о 1 секции
                mul     edx

                push    eax
                push    offset all_ot            ; прочтем информацию о всех
                call    read_file                ; секциях в "all_ot"
;───────────────
; проверим все секции на наличие в них точки входа
;
                pop     ecx                      ; ecx - число секций в PE
 read_sec_loop: push    ecx
 check_sec_nfo: push    dword ptr [ph.EntryPoint]
                push    edi
                call    check_sec_ep             ; проверим, точка входа в этой
                or      edx,edx                  ; секции ?
                jz      next_sec                 ; если нет, проверим следующую
;═══════════════
; расчитаем точку входа относительно начала файла
; [_ph+028h] (EntryPoint) - [_ot+012d] (VirtRVA) + [_ot+020d] (PhOffset) = AAA
;
                mov     eax,dword ptr [esi+028h]
                sub     eax,dword ptr [edi+012d]
                add     eax,dword ptr [edi+020d] ; eax = AAA
                pop     ecx
                jmp     get_pe_ep_ret            ; выйдем
;───────────────
 next_sec:      pop     ecx
                add     dword ptr [ot_loc],ot_size
                add     edi,ot_size
                loop    read_sec_loop
                sub     eax,eax                  ; eax = 0 - не нашли entry point
;═══════════════════════
 get_pe_ep_ret: pop     ecx edx ebx ebp esi edi
                ret
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
 check_sec_ep   proc    _ot:dword, off:dword
;──────────────────────────────────────────────────────────────────────────────
                mov     eax,off
                mov     edi,offset _ot
                mov     edx,dword ptr [edi+012d]
                push    edx
                add     edx,dword ptr [edi+008d]
                cmp     eax,edx
                pop     edx
                jge     no_ip_in_sec     ; if EntryPoint < (VirtRVA + VirtSize)
                cmp     eax,edx
                jge     check_ip_ret     ; if EntryPoint >= VirtRVA
 no_ip_in_sec:  sub     edx,edx
 check_ip_ret:  ret
 endp
;──────────────────────────────────────────────────────────────────────────────