.code
;──────────────────────────────────────────────────────────────────────────────
; " make_str_various "
;
;   Генерируем строчное представление для следующих инструкций:
;
;  1) add  regA,regB   (8 bit)             │ 000HEX, 11rgArgB
;                      (32 bit)            │ 001HEX, 11rgArgB
;                      (8 bit)             │ 002HEX, 11rgBrgA 
;                      (32 bit)            │ 003HEX, 11rgBrgA 
;  2) or   regA,regB   (8 bit)             │ 008HEX, 11rgArgB
;                      (32 bit)            │ 009HEX, 11rgArgB
;                      (8 bit)             │ 00AHEX, 11rgArgB
;                      (32 bit)            │ 00BHEX, 11rgArgB
;  3) adc  regA,regB   (8 bit)             │ 010HEX, 11rgBrgA
;                      (32 bit)            │ 011HEX, 11rgArgB
;                      (8 bit)             │ 012HEX, 11rgBrgA
;                      (32 bit)            │ 013HEX, 11rbArbB
;  4) sbb  regA,regB   (8 bit)             │ 018HEX, 11rgArgB
;                      (32 bit)            │ 019HEX, 11rgArgB
;                      (8 bit)             │ 01AHEX, 11rgArgB
;                      (32 bit)            │ 01BHEX, 11rgBrgA
;  5) and  regA,regB   (8 bit)             │ 020HEX, 11rgBrgA
;                      (32 bit)            │ 021HEX, 11rgBrgA
;                      (8 bit)             │ 022HEX, 11rgBrgA
;                      (32 bit)            │ 023HEX, 11rgBrgA
;  6) sub  regA,regB   (8 bit)             │ 028HEX, 11rgBrgA
;                      (32 bit)            │ 029HEX, 11rgBrgA
;                      (8 bit)             │ 02AHEX, 11rgBrgA
;                      (32 bit)            │ 02BHEX, 11rgBrgA
;  7) xor  regA,regB   (8 bit)             │ 030HEX, 11rgBrgA
;                      (32 bit)            │ 031HEX, 11rgBrgA
;                      (8 bit)             │ 032HEX, 11rgBrgA
;                      (32 bit)            │ 033HEX, 11rgBrgA
;  8) cmp  regA,regB   (8 bit)             │ 038HEX, 11rgBrgA
;                      (32 bit)            │ 039HEX, 11rgBrgA
;                      (8 bit)             │ 03AHEX, 11rgBrgA
;                      (32 bit)            │ 03BHEX, 11rgBrgA
;  9) test regA,regB   (8 bit)             │ 084HEX, 11rgBrgA
;                      (32 bit)            │ 085HEX, 11rgBrgA
; 10) xchg regA,regB   (8 bit)             │ 086HEX, 11rgBrgA
;                      (32 bit)            │ 087HEX, 11rgBrgA
;
 make_str_various proc   instr_offset: dword
;──────────────────────────────────────────────────────────────────────────────
                
                push   eax ecx ebp esi edi
                sub    ecx,ecx
                mov    esi,instr_offset
                lodsw                      ; Тип инструкции
                sub    ebp,ebp
                cmp    al,000h
                jz     make_str_various_add_b
                cmp    al,001h
                jz     make_str_various_add_d
                inc    ebp
                cmp    al,002h
                jz     make_str_various_add_b
                cmp    al,003h
                jz     make_str_various_add_d
;
                sub    ebp,ebp
                cmp    al,008h
                jz     make_str_various_or_b
                cmp    al,009h
                jz     make_str_various_or_d
                inc    ebp
                cmp    al,00ah
                jz     make_str_various_or_b
                cmp    al,00bh
                jz     make_str_various_or_d
;
                sub    ebp,ebp
                cmp    al,010h
                jz     make_str_various_adc_b
                cmp    al,011h
                jz     make_str_various_adc_d
                inc    ebp
                cmp    al,012h
                jz     make_str_various_adc_b
                cmp    al,013h
                jz     make_str_various_adc_d
;
                sub    ebp,ebp
                cmp    al,018h
                jz     make_str_various_sbb_b
                cmp    al,019h
                jz     make_str_various_sbb_d
                inc    ebp
                cmp    al,01Ah
                jz     make_str_various_sbb_b
                cmp    al,01Bh
                jz     make_str_various_sbb_d
;
                sub    ebp,ebp
                cmp    al,020h
                jz     make_str_various_and_b
                cmp    al,021h
                jz     make_str_various_and_d
                inc    ebp
                cmp    al,022h
                jz     make_str_various_and_b
                cmp    al,023h
                jz     make_str_various_and_d
;
                sub    ebp,ebp
                cmp    al,028h
                jz     make_str_various_sub_b
                cmp    al,029h
                jz     make_str_various_sub_d
                inc    ebp
                cmp    al,02ah
                jz     make_str_various_sub_b
                cmp    al,02bh
                jz     make_str_various_sub_d
;
                sub    ebp,ebp
                cmp    al,030h
                jz     make_str_various_xor_b
                cmp    al,031h
                jz     make_str_various_xor_d
                inc    ebp
                cmp    al,032h
                jz     make_str_various_xor_b
                cmp    al,033h
                jz     make_str_various_xor_d
;
                sub    ebp,ebp
                cmp    al,038h
                jz     make_str_various_cmp_b
                cmp    al,039h
                jz     make_str_various_cmp_d
                inc    ebp
                cmp    al,03ah
                jz     make_str_various_cmp_b
                cmp    al,03bh
                jz     make_str_various_cmp_d
;
                sub    ebp,ebp
                cmp    al,084h
                jz     make_str_various_test_b
                cmp    al,085h
                jz     make_str_various_test_d
;
                cmp    al,086h
                jz     make_str_various_xchg_b
                cmp    al,087h
                jz     make_str_various_xchg_d
;
                jmp    make_str_various_ret
;
 make_str_various_add_d:
                inc    ecx
 make_str_various_add_b:
                inc    ecx
                mov    esi,offset _add
                jmp    make_str_various_registers 
 make_str_various_or_d:
                inc    ecx
 make_str_various_or_b:
                inc    ecx
                mov    esi,offset _or
                jmp    make_str_various_registers 
 make_str_various_adc_d:
                inc    ecx
 make_str_various_adc_b:
                inc    ecx
                mov    esi,offset _adc
                jmp    make_str_various_registers 
 make_str_various_sbb_d:
                inc    ecx
 make_str_various_sbb_b:
                inc    ecx
                mov    esi,offset _sbb
                jmp    make_str_various_registers 
 make_str_various_and_d:
                inc    ecx
 make_str_various_and_b:
                inc    ecx
                mov    esi,offset _and
                jmp    make_str_various_registers 
 make_str_various_sub_d:
                inc    ecx
 make_str_various_sub_b:
                inc    ecx
                mov    esi,offset _sub
                jmp    make_str_various_registers 
 make_str_various_xor_d:
                inc    ecx
 make_str_various_xor_b:
                inc    ecx
                mov    esi,offset _xor
                jmp    make_str_various_registers 
 make_str_various_cmp_d:
                inc    ecx
 make_str_various_cmp_b:
                inc    ecx
                mov    esi,offset _cmp
                jmp    make_str_various_registers 
 make_str_various_test_d:
                inc    ecx
 make_str_various_test_b:
                inc    ecx
                mov    esi,offset _test
                jmp    make_str_various_registers 
 make_str_various_xchg_d:
                inc    ecx
 make_str_various_xchg_b:
                inc    ecx
                mov    esi,offset _xchg

; соберем данные по регистрам инструкции
 make_str_various_registers:
                mov    edi,offset disasm_str
                call   copy_str
                call   copy_space          ; разделитель

                xor    ah,11000000b        ; уберем мусор
                mov    al,0
                xchg   ah,al               ; данные регистров в EAX
                call   two_regs            ; EDX = REGA; EAX = REGB;
                or     ebp,ebp
                jz     make_str_various_not_swap_regs
                xchg   eax,edx

 make_str_various_not_swap_regs:
                mov    esi,offset _regs_8bit
                cmp    cl,1
                jz     make_str_various_get_regs
                mov    esi,offset _regs_32bit
 make_str_various_get_regs:
                add    cl,2
                push   eax esi             ; определим первый регистр (REG B)
                sub    eax,eax
                mov    al,cl
                mul    edx
                add    esi,eax
                call   copy_str
                call   copy_cont           ; проставим запятую
                pop    esi eax
;

                sub    edx,edx             ; второй регистр (REG A)
                mov    dl,cl
                mul    edx
                add    esi,eax
                call   copy_str

                mov    al,0
                stosb                      ; поставим точку
;
                sub    ebx,ebx             ; строчное представление подготовлено
;
 make_str_various_ret:
                pop    edi esi ebp ecx eax
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; Копируем строку из ESI в EDI (должна заканчиваться нулем)
;
 copy_str       proc   
;──────────────────────────────────────────────────────────────────────────────
 copy_str_loop: lodsb
                cmp    al,0
                jz     copy_str_ret
                stosb
                jmp    copy_str_loop
 copy_str_ret:  ret
 endp
;
; Копируем пробел
;
 copy_space     proc
                push   eax
                mov    al,20h
                stosb
                pop    eax
                ret
 endp
;
; Копируем плюс
;
 copy_plus      proc
                push   eax
                mov    al,'+'
                stosb
                pop    eax
                ret
 endp
;
; Копируем минус
;
 copy_minus     proc
                push   eax
                mov    al,'-'
                stosb
                pop    eax
                ret
 endp
;
; Копируем запятую
;
 copy_cont      proc
                push   eax
                mov    al,','
                stosb
                pop    eax
                ret
 endp
;
; Копируем [
;
 copy_start     proc
                push   eax
                mov    al,'['
                stosb
                pop    eax
                ret
 endp
;
; Копируем ]
;
 copy_close     proc
                push   eax
                mov    al,']'
                stosb
                pop    eax
                ret
 endp
;══════════════════════════════════════════════════════════════════════════════
; работа с файлом "листингом"
;
; Создание листинга
;
 init_lst               proc fname:dword
;──────────────────────────────────────────────────────────────────────────────
 init_lst_start:
                push    0 0
                push    CREATE_ALWAYS
                push    0 0
                push    GENERIC_RNW fname 
                call    CreateFileA              ; откроем файл для чтения
                cmp     eax,-1                   ; выйдем в случае ошибки
                jz      init_lst_ret
;───────────────
                inc     byte ptr [debug_lst]
;───────────────
 save_lst_hndl: mov     dword ptr [lst_handle],eax
;
                push    FILE_END 0 0 eax         ; установим указатель в
                call    SetFilePointer           ; конец файла
 init_lst_ret:  ret
;
 init_lst               endp
;──────────────────────────────────────────────────────────────────────────────
; Запись в файл-листинг произвольной строки
;
 write_lst              proc lbuf:dword, dlen: dword
;──────────────────────────────────────────────────────────────────────────────
                cmp     byte ptr [debug_lst],1
                jnz     write_lst_ret
;───────────────
                push    eax ecx edx 
                push    0
                push    offset number
                push    dlen
                push    lbuf
                push    lst_handle
                call    WriteFile
                pop     edx ecx eax
 write_lst_ret: ret
;
 write_lst              endp
;──────────────────────────────────────────────────────────────────────────────
; Получаем строку состояния регистров и записываем ее в буфер sbuf
;
 get_regs_nfo           proc sbuf:dword

                push    eax ecx edx esi edi
                sub     ecx,ecx
                mov     edx,offset _regs_32bit
                mov     esi,offset regs 
                mov     edi,sbuf

 get_regs_nfo_lp:
                push    esi
                xchg    esi,edx             ; ESI - Текстовое представление 
                lodsw                       ;       регистра
                stosw
                lodsb
                stosb
                xchg    esi,edx
                inc     edx
                pop     esi

                mov     al,'='              ; 
                stosb

                lodsd                       ; EAX - Данные регистра
;
                push    eax                 ; Переведем значение регистра в текст
                push    edi
                call    dword_to_str
                add     edi,8               ; Размер текстового представления DWORD        
;
                mov     ax,';h'
                stosw                       ; 

                inc     ecx     
                cmp     cl,8
                jnz     get_regs_nfo_lp
                  
                pop     edi esi edx ecx eax
                ret
;
 get_regs_nfo           endp
;──────────────────────────────────────────────────────────────────────────────
; Запись в файл-листинг дизассемблированной строки в виде
;
; " смещение: команда  состояние регистров "
;
; На входе: instr_offset - смещение инструкции
;           regs_info    - добавлять (0/1 - да/нет) 
;
; представление команды берется из "disasm_str"
;
 write_disasm_str       proc instr_offset:dword, add_regs_nfo: dword
;──────────────────────────────────────────────────────────────────────────────
                push    eax esi edi
                cmp     byte ptr [debug_lst],1
                jz      write_disasm_str_write
 
                push    offset lst_fname
                call    init_lst

 write_disasm_str_write:
                mov     edi, offset text_buff
                mov     al,' '
                stosb  

                push    dword ptr [instr_offset]
                push    edi
                call    dword_to_str
                add     edi,8

                mov     ax,' :'
                stosw
                sub     eax,eax
                stosb

                push    offset text_buff
                call    lstrlenA
                push    eax
                push    offset text_buff
                call    write_lst                ; Запишем смещение

                mov     esi,offset unknown_instruction 
                cmp     byte ptr [disasm_str],0
                jz      write_disasm_str_instr

                mov     esi,offset disasm_str
 write_disasm_str_instr:

                push    esi
                call    lstrlenA
                push    eax
                push    esi
                call    write_lst

                mov     eax,add_regs_nfo ; Записываем состояние регистров?
                or      eax,eax
                jnz     write_disasm_str_ret

                mov     edi,offset text_buff
                mov     al,' '
                stosb

                push    offset text_buff+1
                call    get_regs_nfo

                push    offset text_buff
                call    lstrlenA
                push    eax
                push    offset text_buff
                call    write_lst

 write_disasm_str_ret:          

                mov     edi,offset text_buff
                mov     ax,0a0dh
                stosw

                sub     eax,eax
                mov     al,2
                push    eax
                push    offset text_buff
                call    write_lst
                    
                pop     edi esi eax
                ret
;
 write_disasm_str       endp