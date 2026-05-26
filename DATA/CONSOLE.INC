;
 DRIVE_UNKNOWN         = 0
 DRIVE_NO_ROOT_DIR     = 1
 DRIVE_REMOVABLE       = 2
 DRIVE_FIXED           = 3
 DRIVE_REMOTE          = 4
 DRIVE_CDROM           = 5
 DRIVE_RAMDISK         = 6
;──────────────────────
 STD_INPUT_HANDLE      = -10
 STD_OUTPUT_HANDLE     = -11
 STD_ERROR_HANDLE      = -12
;
 FILE_BEGIN            = 0
 FILE_CURRENT          = 1
 FILE_END              = 2
 CREATE_ALWAYS         = 2
 OPEN_EXISTING         = 3
 GENERIC_READ          = 80000000h
 GENERIC_WRITE         = 40000000h
 GENERIC_RNW           = GENERIC_READ + GENERIC_WRITE
;
; размер одной ячейки данных в массиве "find_buf"
;
 ENTRY_SIZE            = 1+260+4+318
;────────────────────── данные для консоли
 text_buff             db 3000 dup (?)           ;
 console_in            dd ?                      ;используются для работы
 console_out           dd ?                      ;с консолью
 hex_num_temp          dd ?
 number                dd ?
; 
 _argc                 dd ?                      ; данные ком. строки
 _argv                 dd ?
 _strbuf               dd ?
 _hheap                dd ?
;
 thread_handle         dd ?
;
 handle                dd ?                      ; файловый номер
 heap_handle           dd ?                      ; блок памяти
 file_map_address      dd ?                      ; местоположение файла в памяти
 rep_handle            dd ?                      ; файловый номер отчета
 bytes_read            dd ?                      ;
;
 fixed_drives          db 33+1 dup (?)
;
; find_buf - массив содержащий данные, используемые для рекурсивного обхода
;            дерева каталогов
;
 find_buf              db ENTRY_SIZE*50 dup (?)
 file_path             db 260 dup (?)
 start_directory_ptr   dd ?