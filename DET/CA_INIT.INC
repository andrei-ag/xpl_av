; Анализатор кода (c) 2004, beauty on the fire
;
; Инициализация кодоанализатора
;
 proc ca_init    
;──────────────────────────────────────────────────────────────────────────────
                push   eax ecx edi
                mov    word [ca_ip_percent],0
 reg_analyze:   sub    eax,eax
                sub    ecx,ecx
                mov    cl,8
                mov    edi,_regs
 ra_init_loop:  stosb
                loop   ra_init_loop
;
; тип значений виртуального регистра esp - 2 (stack)
;
                mov    byte [_regs+4],2
                pop    edi ecx eax
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────