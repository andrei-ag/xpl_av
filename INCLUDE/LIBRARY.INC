;══════════════════════════════════════════════════════════════════════════════
 proc get_command       
;──────────────────────────────────────────────────────────────────────────────
 local path:DWORD
                push    ecx edx ebx edi esi
;
                mov     dword [path],0
;
	        invoke  GetProcessHeap
        	mov     [_hheap],eax
      	  	invoke  HeapAlloc,[_hheap],HEAP_ZERO_MEMORY,1000h
        	mov     [_strbuf],eax

        	call    GetMainArgs
;
                mov     ecx,[_argc]
                cmp     ecx,1
                jbe     get_command_ret
;
                mov     esi,[_argv]
                lodsd
                dec     ecx                ; Skip first param
 process_cmndline_param:
                lodsd
                push    esi
                xchg    esi,eax
                call    str_to_low
;
                cmp     byte [esi],'/'
                jz      process_param
;
                mov     dword [path],esi
;
 next_param:    pop     esi
                loop    process_cmndline_param
;
 get_command_ret:
                lea     edi,[find_buf+1]
;
; IF "/*" typed
;
                cmp     byte [fixed_drives],0
                jz      load_path
                mov     dword [path],edi
                mov     al,byte [fixed_drives+1]
                add     al,'a'
                stosb
                mov     ax,':\'
                stosw
                jmp     load_path_end
;
 load_path:     mov     esi,dword [path]
                push    esi
                invoke  lstrlenA
                xchg    eax,ecx
                rep     movsb
;───────────────
 load_path_end: sub     eax,eax
                stosb

                pop     esi edi ebx edx ecx
                mov     eax,dword [path]
                ret
;───────────────
; Проверка параметров
;
 process_param: lodsb
                lodsw
                cmp     ax,'ra'
                jz      set_report_append
                cmp     ax,'rc'
                jz      set_report_create
                cmp     ax,'t-'
                jz      set_turn_off_emul_timer
                cmp     ax,'di'
                jz      set_show_ca_base         
                cmp     al,'d'
                jz      set_debug_mode
                cmp     al,'*'
                jz      set_scan_all_hdd
                cmp     al,'c'
                jz      set_cure_infected_file
;
                pop     esi
                jmp     get_command_ret          ; unknown param
;───────────────
 set_debug_mode:
                inc     byte [debug_mode]        ; Создание дампов
                jmp     next_param               
;───────────────
 set_show_ca_base:
                inc     byte [debug_show_loops]  ; Показываем БД кодо-анализатора по обнаруженным циклам
                jmp     next_param               
;───────────────
 set_turn_off_emul_timer:
                dec     byte [emul_timer]        ; отключаем таймер эмулятора
                jmp     next_param               
;───────────────
 set_scan_all_hdd:
                call    getfixed_drive
                jmp     next_param
;───────────────
 set_report_create:
                mov     byte [report],1
                jmp     next_param
;───────────────
 set_report_append:
                mov     byte [report],0
                jmp     next_param
;───────────────
 set_cure_infected_file:       
                mov     byte [try_cure],1    ; лечить файлы
                jmp     next_param
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc getfixed_drive
;──────────────────────────────────────────────────────────────────────────────
                pushad
                lea     edi,[fixed_drives+1]
;
 drive_loop:    cmp     byte [drive_path],'z'
                ja      gfix_drv_ret             ; "z" последний диск
                push    drive_path
                invoke  GetDriveTypeA            ; получим тип диска
                cmp     al,DRIVE_FIXED
                jnz     next_drive               ; если не жесткий, пропустим
;───────────────
                inc     byte [fixed_drives]      ;
                mov     al,byte [drive_path]
                sub     al,'a'
                stosb                            ;
 next_drive:    inc     byte [drive_path]
                jmp     drive_loop
;─ ─ ─ ─ ─ ─ ─ ─
 gfix_drv_ret:  popad
                mov     byte [drive_path],'a'
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; eax - value to align
; ecx - align to
;
 proc align_value            
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx
                sub     edx,edx
                div     ecx
;
                or      edx,edx
                jz      align_val_ret
;
                inc     eax
 align_val_ret: mul     ecx
                pop     edx ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc str_to_low
;──────────────────────────────────────────────────────────────────────────────
                push    esi
 str2low_lp:    cmp     byte [esi],0
                jz      str2low_ret
                cmp     byte [esi],'A'
                jc      str2low_next
                cmp     byte [esi],'Z'
                ja      str2low_next
                add     byte [esi],20h
 str2low_next:  inc     esi
                jmp     str2low_lp
 str2low_ret:   pop     esi
                ret
;
 endp    
;──────────────────────────────────────────────────────────────────────────────
; cmp_str - сравнение строк заканчивающихся нулем 
;
; on start:  ESI - str 1
;            EDI - str 2
;
; on end  : CF=0 - strings equail
;           CF=1 - strings no equail
;
 proc cmp_str
;──────────────────────────────────────────────────────────────────────────────
                push    edi esi
;
 cmp_str_loop:  cmpsb
                jnz     cmp_str_not_match
                cmp     byte [edi-1],0
                jnz     cmp_str_loop
                pop     esi edi
                clc
                ret
;
 cmp_str_not_match:
                pop     esi edi
                stc
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; cmp_str - сравнение строк (без учета регистра) заканчивающихся нулем 
;
; on start:  ESI - str 1
;            EDI - str 2
;
; on end  : CF=0 - strings equail
;           CF=1 - strings no equail
;
 proc cmpi_str
;──────────────────────────────────────────────────────────────────────────────
                push    edi esi
;
 cmpi_str_loop: lodsb
                cmp     al,byte [edi]
                jz      cmpi_str_next_byte
;
; Попробуем увеличить или уменьшить регистр буквы
; 
 cmpi_str_test_HIGH:
                cmp     al,'A'
                jb      cmpi_str_not_match
                cmp     al,'Z'
                ja      cmpi_str_test_LOW
                add     al,20h ; Switch FROM HIGH to LOW
                jmp     cmpi_str_test_2
;
 cmpi_str_test_LOW:
                cmp     al,'a'
                jb      cmpi_str_not_match
                cmp     al,'z'
                ja      cmpi_str_not_match
;
                sub     al,20h ; Switch FROM LOW to HIGH
 cmpi_str_test_2:
                cmp     al,byte [edi]
                jnz     cmpi_str_not_match
;
 cmpi_str_next_byte:
                cmp     byte [edi],0
                jz      cmpi_str_match
                inc     edi
                jmp     cmpi_str_loop
;
 cmpi_str_match:
                pop     esi edi
                clc
                ret

 cmpi_str_not_match:
                pop     esi edi
                stc
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc read_file         lbuf, dlen
;──────────────────────────────────────────────────────────────────────────────
                push    0
                push    number
                push    [dlen]
                push    [lbuf]
                push    [handle]
                invoke  ReadFile
                ret     
;                  
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc write_file        lbuf, dlen
;──────────────────────────────────────────────────────────────────────────────
                push    0
                push    offset number
                push    [dlen]
                push    [lbuf]
                push    [handle]
                invoke  WriteFile
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────