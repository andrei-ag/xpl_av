;═══════════════
; console routines (c) 1998 by h-    [hayras@hotmail.com]
;                  (c) 2003 by _andy [jpanic@mail.ru]
;
;──────────────────────────────────────────────────────────────────────────────
 proc init_console
;──────────────────────────────────────────────────────────────────────────────
                push    STD_INPUT_HANDLE
                invoke  GetStdHandle

                or      eax,eax
                je      init_error
                mov     [console_in],eax
                push    STD_OUTPUT_HANDLE
                invoke  GetStdHandle

                or      eax,eax
                je      init_error
                mov     [console_out],eax
                ret

 init_error:    invoke  ExitProcess,0
;
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc write_console     text_out, text_len
;──────────────────────────────────────────────────────────────────────────────
                pusha
                push    0
                push    bytes_read                    ; number of bytes read
                push    [text_len]                    ; text length
                push    [text_out]                    ; offset of text
                push    [console_out]                 ; handle
                invoke  WriteConsoleA
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc write_string      text_out                ; (c) _andy [mnt]
;──────────────────────────────────────────────────────────────────────────────               
                pusha
                push    [text_out]
                invoke  lstrlenA
                push    eax
                push    [text_out]
                call    write_rep
;
                push    eax                
                push    [text_out]
                call    write_console
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc write_hex         hex_in, value1, value2, value3
;──────────────────────────────────────────────────────────────────────────────
                pusha
                push    [value3]
                push    [value2]
                push    [value1]
                push    [hex_in]
                push    text_buff
                invoke  wsprintfA
;
                add     esp, 4*5
                push    eax text_buff
                call    write_console
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc write_stat        str_disp, val1, val2, val3, val4, val5
;──────────────────────────────────────────────────────────────────────────────
                pusha
                push    [val1] [val2] [val3] [val4] [val5] [str_disp]
                push    text_buff
                invoke  wsprintfA

                add     esp, 4*7
                push    eax text_buff
                call    write_console
                popa
                ret
  endp
;──────────────────────────────────────────────────────────────────────────────
; перевод числа "snum" в десятичный вид и сохранение в буфере "sbuf" 
; (c) _andy [mnt]
;
 proc hex_to_dec       sbuf, snum
;──────────────────────────────────────────────────────────────────────────────
                push   eax ecx ebx ebp esi edi   ; сохраним регистры
                mov    edx,[snum]                ; edx - число для перевода
                sub    ecx,ecx                   ; инициализация счетчика
                mov    edi,[sbuf]
                add    edi,10                    ; указатель для буффера
;───────────────
 htd_main_loop: push   ecx                       ; ecx - счетчик размера числа
                mov    eax,edx                   ; eax - число для перевода
                sub    edx,edx                   ; edx = 0
                mov    ecx,10
                div    ecx                       ; делим на 10
                xchg   eax,edx                   ; eax = edx │ edx = eax
                add    al,'0'                    ; получим десятичное число
                mov    [edi],al                  ; положим в буффер
                inc    edi                       ; следующий байт
                pop    ecx                       ; "вспомним" счетчик
                inc    ecx                       ; увеличим размер десятичного
                or     edx,edx                   ; все?
                jnz    htd_main_loop
;───────────────
; число лежит в обратном порядке, перенесем его в "sbuf" и приведем "в чувство"
;
                mov    esi,[sbuf]
 htd_back_loop: dec    edi
                mov    al,[edi]
                mov    [esi],al
                inc    esi
                loop   htd_back_loop
                mov    byte [esi],0              ; установим конец числа
                pop    edi esi ebp ebx ecx eax   ; востановим сохраненые регистры
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; перевод двойного "snum" в шестнадцатеричный вид и сохранение в буфере "sbuf" 
;
; by Mikl__ (http://www.wasm.ru/forum/viewtopic.php?id=20503)
;
 proc dword_to_str     sbuf, snum
;──────────────────────────────────────────────────────────────────────────────
                push   eax ecx ebx esi edi       ; сохраним регистры
                mov    eax,[snum]                ; eax - число для перевода
                mov    esi,[sbuf]
                sub    ecx,ecx
                mov    cl,8  
 dword_to_str_lp:
             	push   eax
	        and    al,0Fh
	        cmp    al,0Ah
	        sbb    al,69h
	        das
	        mov    byte [esi+ecx-1],al
   	        pop    eax
	        shr    eax,4
	        loop   dword_to_str_lp 
                pop    edi esi ebx ecx eax       ; востановим сохраненые регистры
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 proc show_hex          no                 ; (c) _andy [mnt]
;──────────────────────────────────────────────────────────────────────────────
                push    [no]
                push    text_buff
                call    hex_to_dec
                push    text_buff
                call    write_string
                ret
 endp