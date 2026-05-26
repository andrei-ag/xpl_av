;──────────────────────────────────────────────────────────────────────────────
; get_pe_ep - определяет точку входа в PE файл
;
; on start :
; on exit  : eax     - entry point
;
 proc get_pe_ep         
;──────────────────────────────────────────────────────────────────────────────
                mov     eax,dword [file_map_address]
                add     eax,dword [ph_loc]       ; ESI = Начало заголовка PE
;
                mov     eax,dword [eax+028h]     ; EIP
                add     eax,dword [file_map_address]
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────