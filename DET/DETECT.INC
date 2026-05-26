; Анализатор кода (c) 2004, beauty on the fire
;
; Часть анализатора кода, отвечающая за поиск сигнатур известных вирусов.
;
;──────────────────────────────────────────────────────────────────────────────
;
 entry_size            equ e_type+e_name+e_slen+e_dlen+e_sdata+e_curem
;
 e_type                equ 1                     ; тип записи для файлов:
  etype_dos_com        equ 0                     ;  - com (dos)
  etype_dos_exe        equ 1                     ;  - exe (dos)
  etype_pe             equ 2                     ;  - PE  (win32)
  etype_ole            equ 3                     ;  - OLE (msWord)
;
 e_name                equ 25            ; длина названия записи
 e_slen                equ 1             ; размер информации о длине сигнатуры
 e_dlen                equ 2             ; зарезервированно
 e_sdata               equ 30            ; длина сигнатуры
 e_curem               equ 4             ; метод лечения или смещение процедуры
   cmethod_unk         equ 0             ;  - вылечить немогу
   cmethod_del         equ 1             ;  - необходимо удалить
;───────────────────────
.data
;═══════════════════════────────────────────────── вирусная база
 vir_no                 dd 5                     ; количество записей в базе
@001_type               db etype_pe
@001_name               db 'W95/Marburg [8582]'
                        db 007 dup (0)
@001_sig_len            db 023
@001_reserved           dw ?
@001_sig_data           db 0E8h,000h,000h,000h,000h,05Dh,08Bh,0DDh,081h,0EDh
                        db  '?',004d,081h,0EBh, '?',004d,08Bh,0C3h,005h, '?'
                        db 010d,00Bh,0C0h,0FFh,034h,018h
                        db 04 dup (0)
@001_cure_loc           dd marburg_8582
;-----------------------
@002_type               db etype_pe
@002_name               db 'W32/Parite'          ; штамм B
                        db 015 dup (0)
@002_sig_len            db 020
@002_reserved           dw ?
@002_sig_data           db 055h,08Bh,0ECh,081h,0C4h,0C0h,0FEh,0FFh,0FFh,08Bh
                        db 0C5h,083h,0C0h,004h, '?',003h,056h,057h,033h,0DBh
                        db 010 dup (?)
@002_cure_loc           dd offset parite_b_pe
;-----------------------
@003_type               db etype_pe
@003_name               db 'Worm/Qaz'
                        db 017 dup (0)
@003_sig_len            db 013
@003_reserved           dw ?
@003_sig_data           db 055h,08Bh,0ECh,06Ah,0FFh,068h, '?',004d,068h,004h
                        db 0A5h,040h,000h
                        db 017 dup (?)
@003_cure_loc           dd cmethod_del
;-----------------------
@004_type               db etype_pe
@004_name               db 'W32/Krized [4029]'
                        db 008 dup (0)
@004_sig_len            db 030
@004_reserved           dw ?
@004_sig_data           db 0E8h, '?',004d,033h,0D2h,00Fh,085h, '?',004d,088h
                        db  '?',005d,08Bh,0C5h,02Dh, '?',004d,005h, '?',004d
                        db 089h,044h, '?',023d,'T-2000'
@004_cure_loc           dd offset krized_4029
;----------------------- 
@005_type               db etype_pe
@005_name               db 'W32/Funlove [4099]'
                        db 007 dup (0)
@005_sig_len            db 015
@005_reserved           dw ?
@005_sig_data           db 0E8h,0AAh,008h,000h,000h,08Dh, '?',005d,08Bh,03Ch
                        db 024h, '?',006d,0A5h,0A5h
                        db 15 dup (0)
@005_cure_loc           dd offset funlove
;----------------------- 
.code
;──────────────────────────────────────────────────────────────────────────────
; detect - сравнение сигнатур из базы "bloc" с участком кода из "cloc"
;
; on start :     ftype - тип файла
;                 bloc - расположение базы сигнатур
;                 cloc - смещение кода
; on exit  :  eax == 0 - ни одна из сигнатур не совпала
;             eax != 0 - содержит номер совпавшей сигнатуры в базе
;
 detect         proc   cloc:dword, bloc:dword, ftype:dword
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx esi
                sub     edx,edx                ; edx - регистр под номер записи
                inc     edx                    ; edx=1 - рассмотрим 1 запись
                mov     esi,bloc                 ; esi - расположение базы
                mov     ecx,dword ptr [esi]      ; ecx - кол-во записей в базе
                add     esi,4                    ; esi - данные первой записи
 det_main_loop: mov     eax,ftype           ;┐
                cmp     byte ptr [esi],al   ;│сравним тип файла и записи
                jnz     det_main_next       ;┘не совпадает, значит не проверяем
                sub     eax,eax
                mov     al,byte ptr [esi+e_type+e_name]
                push    eax
                add     esi,e_type+e_name+e_slen+e_dlen
                push    esi                    ; расположение сигнатуры в записи
                push    cloc                   ; расположение кода для сравнения
                call    detect_sig             ; совпадает сигнатура ?
                or      eax,eax
                jnz     det_sig_found          ; если сигнатура совпала
;───────────────
 det_main_next: inc     edx                    ; если сигнатура не совпала,
                add     esi,e_sdata+e_curem    ; проверим следующую запись
                loop    det_main_loop
                sub     eax,eax                ; если ни одна из сигнатур не
 det_main_ret:  pop     esi edx ecx            ; совпала - eax = 0
                ret
;───────────────
 det_sig_found: xchg    eax,edx            ; eax<>edx=номер совпавшей сигнатуры
                jmp     det_main_ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; проверка участка на совпадение с сигнатурой
;
; on start : slen - длина сигнатуры
;            sloc - расположение сигнатуры
;            cloc - расположение кода для сверки
; on exit  : eax = 0 - не совпадают, иначе сигнатура совпала
;
 detect_sig     proc   cloc:dword, sloc: dword, slen: dword
;──────────────────────────────────────────────────────────────────────────────
                push    ecx edx esi edi
                                         ; установим параметры сигнатуры:
                mov     ecx,slen         ; ecx - размер
                mov     esi,sloc         ; esi - расположение
                mov     edi,cloc         ; edi - код для сравнения с сигнатурой
 detect_loop:   lodsb
                cmp     al,'?'
                jnz     det_compare
;
; если встретился символ "?", значит необходимо пропустить количество
; (указанное в следующем dword'е сигнатуры) байт в исследуемом коде
;
                sub     eax,eax
                lodsb
                dec     ecx
                add     edi,eax
                jmp     detect_cont+1
 det_compare:   cmp     byte ptr [edi],al        ; сравним байты сигнатуры и кода
                jz      detect_cont
                sub     eax,eax
                jmp     detect_ret
 detect_cont:   inc     edi
                loop    detect_loop              ; если цикл окончен и сигнатура
                mov     al,1                     ; совпала полностью EAX = 1
 detect_ret:    pop     edi esi edx ecx
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
