;═══════════════
; console routines (c) 1998 by h-    [hayras@hotmail.com]
;                  (c) 2003 by _andy [jpanic@mail.ru]
;
.code
;──────────────────────────────────────────────────────────────────────────────
 init_console   proc
;──────────────────────────────────────────────────────────────────────────────
                push    STD_INPUT_HANDLE
                call    GetStdHandle
                or      eax,eax
                je      init_error
                mov     [console_in],eax
                push    STD_OUTPUT_HANDLE
                call    GetStdHandle
                or      eax,eax
                je      init_error
                mov     [console_out],eax
                ret
 init_error:    push    0
                call    ExitProcess
 endp
;──────────────────────────────────────────────────────────────────────────────
 write_console  proc    text_out:dword, text_len:dword
;──────────────────────────────────────────────────────────────────────────────
                pusha
                push    0
                push    offset bytes_read             ; number of bytes read
                push    text_len                      ; text length
                push    text_out                      ; offset of text
                push    console_out                   ; handle
                call    WriteConsoleA
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 write_string   proc    text_out:dword                ; (c) _andy [mnt]
;──────────────────────────────────────────────────────────────────────────────               
                pusha
                push    text_out
                call    lstrlenA
                push    eax
                push    text_out
                call    write_rep
;
                push    eax                
                push    text_out
                call    write_console
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 write_hex      proc    hex_in:dword, value1:dword, value2:dword, value3:dword
;──────────────────────────────────────────────────────────────────────────────
                pusha
                push    value3
                push    value2
                push    value1
                push    hex_in
                push    offset text_buff
                call    _wsprintfA
                add     esp, 4*5
                push    eax offset text_buff
                call    write_console
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
  write_stat    proc    str_disp:dword, val1:dword, val2:dword, \
                            val3:dword, val4:dword, val5:dword
;──────────────────────────────────────────────────────────────────────────────
                pusha
                push    val1 val2 val3 val4 val5
                push    str_disp
                push    offset text_buff
                call    _wsprintfA
                add     esp, 4*7
                push    eax offset text_buff
                call    write_console
                popa
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
; перевод числа "snum" в десятичный вид и сохранение в буффере "sbuf" 
; (c) _andy [mnt]
;
 hex_to_dec     proc   sbuf: dword, snum: dword
;──────────────────────────────────────────────────────────────────────────────
                push   eax ecx ebx ebp esi edi   ; сохраним регистры
                mov    edx,snum                  ; edx - число для перевода
                sub    ecx,ecx                   ; инициализация счетчика
                mov    edi,sbuf
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
; десятичное число лежит задом на перед в "sbuf+10" перенесем его в "sbuf"
; и переведем в нормальный вид
;
                mov    esi,sbuf
 htd_back_loop: dec    edi
                mov    al,[edi]
                mov    [esi],al
                inc    esi
                loop   htd_back_loop
                mov    byte ptr [esi],0          ; установим конец числа
                pop    edi esi ebp ebx ecx eax   ; востановим сохраненые регистры
                ret
 endp
;──────────────────────────────────────────────────────────────────────────────
 show_hex       proc    no:dword                 ; (c) _andy [mnt]
;──────────────────────────────────────────────────────────────────────────────
                push    no
                push    offset text_buff
                call    hex_to_dec
                push    offset text_buff
                call    write_string
                ret
 endp