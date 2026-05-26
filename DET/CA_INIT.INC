; Анализатор кода (c) 2004, beauty on the fire
;
; Инициализация кодоанализатора
;
 ca_init        proc
;──────────────────────────────────────────────────────────────────────────────
                push   eax ecx edi
                mov    word ptr [ca_ip_percent],0
 reg_analyze:   sub    eax,eax
                sub    ecx,ecx
                mov    cl,8
                mov    edi,offset _regs
 ra_init_loop:  stosb
                loop   ra_init_loop
;
; тип значений виртуального регистра esp - 2 (stack)
;
                mov    byte ptr [_regs+4],2
                pop    edi ecx eax
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
