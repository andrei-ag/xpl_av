;
; База сигнатур
;──────────────────────────────────────────────────────────────────────────────
;
 entry_size            = e_type+e_name+e_slen+e_dlen+e_sdata+e_curem
;
 e_type                = 1                     ; тип записи для файлов:
  etype_dos_com        = 0                     ;  - com (dos)
  etype_dos_exe        = 1                     ;  - exe (dos)
  etype_pe             = 2                     ;  - PE  (win32)
  etype_ole            = 3                     ;  - OLE (msWord)
;
 e_name                = 25            ; длина названия записи
 e_slen                = 1             ; размер информации о длине сигнатуры
 e_dlen                = 2             ; зарезервированно
 e_sdata               = 30            ; длина сигнатуры
 e_curem               = 4             ; метод лечения или смещение процедуры
   cmethod_unk         = 0             ;  - вылечить немогу
   cmethod_del         = 1             ;  - необходимо удалить

;═══════════════════════────────────────────────── вирусная база
 vir_no                 dd 12                    ; количество записей в базе
@001_type               db etype_pe
@001_name               db 'W95/Marburg [8582]'
                        db 007 dup (0)
@001_sig_len            db 023
@001_reserved           dw ?
@001_sig_data           db 0E8h,000h,000h,000h,000h,05Dh,08Bh,0DDh,081h,0EDh
                        db  '?',004d,081h,0EBh, '?',004d,08Bh,0C3h,005h, '?'
                        db 010d,00Bh,0C0h,0FFh,034h,018h
                        db 04 dup (0)
@001_cure_loc           dd cmethod_unk ;marburg_8582
;-----------------------
@002_type               db etype_pe
@002_name               db 'W32/Parite.b'        ; штамм B
                        db 013 dup (0)
@002_sig_len            db 020
@002_reserved           dw ?
@002_sig_data           db 055h,08Bh,0ECh,081h,0C4h,0C0h,0FEh,0FFh,0FFh,08Bh
                        db 0C5h,083h,0C0h,004h, '?',003h,056h,057h,033h,0DBh
                        db 010 dup (?)
@002_cure_loc           dd cmethod_unk ; offset parite_b_pe
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
@004_cure_loc           dd cmethod_unk ; offset krized_4029
;----------------------- 
@005_type               db etype_pe
@005_name               db 'W32/Funlove [4099]'
                        db 007 dup (0)
@005_sig_len            db 015
@005_reserved           dw ?
@005_sig_data           db 0E8h,0AAh,008h,000h,000h,08Dh, '?',005d,08Bh,03Ch
                        db 024h, '?',006d,0A5h,0A5h
                        db 15 dup (0)
@005_cure_loc           dd cmethod_unk ; offset funlove
;----------------------- 
@006_type               db etype_pe
@006_name               db 'W32/Aztec [1344]' ; Symantec - Win32.Iced.1344 
                        db 009 dup (0)
@006_sig_len            db 018
@006_reserved           dw ?
@006_sig_data           db 060h,09Ch,0E8h, '?',006d,0C5h, '?',009d,02Dh, '?'
                        db 025d,0E8h, '?',095d,0FFh,095h,080h,015h
                        db 12 dup (0)
@006_cure_loc           dd cmethod_unk
;-----------------------
@007_type               db etype_pe
@007_name               db 'W95/HPS [5124]'
                        db 011 dup (0)
@007_sig_len            db 017
@007_reserved           dw ?
@007_sig_data           db 05Dh,08Bh,0C5h,081h, '?',015d,050h,060h,0B8h,001h
                        db  '?',098d,0BEh,021h,053h,050h,048h, '?',189d,0FAh
                        db 066h,0ADh, '?',159d,091h,040h,0e8h
                        db 3 dup (0)
@007_cure_loc           dd cmethod_unk
;-----------------------
@008_type               db etype_pe
@008_name               db 'W32/Thorin'
                        db 015 dup (0)
@008_sig_len            db 30	
@008_reserved           dw ?
@008_sig_data           db 0E8h, '?',010d,074h, '?',001d,0E8h, '?',007d,0C3h 
                        db 08Dh,0BDh, '?',003d,000h,0B9h, '?',003d,000h,030h
                        db  '?',002d,0E2h,0FBh,0C3h,0FCh, '?',010d,0ACh,032h
@008_cure_loc           dd cmethod_unk ; offset win32_thorin  
;-----------------------
@009_type               db etype_pe
@009_name               db 'W9x/Zombie II'
                        db 012 dup (0)
@009_sig_len            db 10	
@009_reserved           dw ?
@009_sig_data           db 0B8h, '?',027d,0FCh, '?',007d,000h,0FFh,0FFh,081h
                        db 20 dup (0)
@009_cure_loc           dd cmethod_unk
;-----------------------
@010_type               db etype_pe
@010_name               db 'W32/Driller'
                        db 014 dup (0)
@010_sig_len            db 18	
@010_reserved           dw ?
@010_sig_data           db 00Fh,031h, '?',089d,050h,064h, '?',156d,049h
                        db  '?',240d,0c3h, '?',222d,08dh,05eh,0fch,053h
                        db 12 dup (0)
@010_cure_loc           dd cmethod_unk
;-----------------------
@011_type               db etype_pe
@011_name               db 'W95/Prizzy'
                        db 015 dup (0)
@011_sig_len            db 20	
@011_reserved           dw ?
@011_sig_data           db 0e8h, '?',004d,05Dh, '?',016d,080h,07ch,024h,003h
                        db 0bfh, '?',042d,066h,0abh, '?',012d,0cch,0fah,00fh
                        db 10 dup (0)
@011_cure_loc           dd cmethod_unk
;-----------------------
@012_type               db etype_pe
@012_name               db 'W32/DeadCode'
                        db 013 dup (0)
@012_sig_len            db 20	
@012_reserved           dw ?
@012_sig_data           db 05fh,05eh,05dh, '?',015d,089h,085h, '?',034d,0e8h
                        db  '?',009d,08bh,040h,008h, '?',037d,022h,083h,0bdh
                        db 10 dup (0)
@012_cure_loc           dd cmethod_unk
