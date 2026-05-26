;────────────────────── данные для консоли
 text_buff             db 10000 dup (?)          ;┐
 console_in            dd ?                      ;│используются для работы
 console_out           dd ?                      ;│с консолью
 bytes_read            dd ?                      ;┘
 number                dd ?
;
 handle                dd ?                      ; файловый номер
 rep_handle            dd ?                      ; файловый номер отчета
