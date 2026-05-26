.data
;──────────────────────────────────────────────────────────────────────────────
; данные о формате исполняемых файлов
;
; взяты из программы DULLPRO (c) Constantin E. Climentieff aka DrMad
;                                drmad@mail.ru * http://www.chat.ru/~drmad
; переделанны для использования в Explosion Antivirus
;──────────────────────────────────────────────────────────────────────────────
EXEhdr                struct
;
 MZ                    dw ?         ; +00  'MZ'
 PartPag               dw ?         ; +02  длина неполной последней страницы
 PageCnt               dw ?         ; +04  длина образа (+заголовок) в 512-байтниках
 ReloCnt               dw ?         ; +06  число элементов в Relocation Table
 HdrSize               dw ?         ; +08  длина заголовка в 16-байтниках
 MinMem                dw ?         ; +0a  минимум требуемой памяти
 MaxMem                dw ?         ; +0c  максимум требуемой памяти
 ReloSS                dw ?         ; +0e  сегмент стека (относительно RootS)
 ExeSP                 dw ?         ; +10  указатель стека
 ChkSum                dw ?         ; +12  контрольная сумма
 ExeIP                 dw ?         ; +14  счетчик команд
 ReloCS                dw ?         ; +16  сегмент кода (относительно RootS)
 TablOff               dw ?         ; +18  позиция в файле 1-го элемента Relocation Table
 Overlay               dw ?         ; +1a  номер оверлея
 reserved              dd ?         ; +1c  зарезервировано
 oem_id                db 28 dup (?); +20  информация о программе
 PE_hdr_loc            dd ?         ; +3c  смещение реального PE заголовка в файле
;
EXEhdr                ends
;──────────────────────────────────────────────────────────────────────────────
 ph_size              equ 60h
 ot_size              equ 40
;
PEhdr                 struct
 pe                    dd ?         ; +00  Сигнатура
 MachType              dw ?         ; +04  Тип процессора
 NOfSections           dw ?         ; +06  Количество секций
 TimDat                dd ?         ; +08  Время/дата создания
 PSymTable             dd ?         ; +0СH Адрес таблицы символов
 NOfSymbols            dd ?         ; +10H К-во строк в таблице символов
 SzOfOptHdr            dw ?         ; +14H Размер переменной части
 Flags                 dw ?         ; +16H Флаги
 R1                    dw ?         ; +18H ???
 MajorLnkV             db ?         ; +1AH Старшая версия линкера
 MinorLnkV             db ?         ; +1BH Младшая версия линкера
 SizeOfCode            dd ?         ; +1CH Размер исполняемого кода
 SizeOfInD             dd ?         ; +20H Размер иниц-ных данных
 SizeOfUnInD           dd ?         ; +24H Размер неиниц-ных данных
 EntryPoint            dd ?         ; +28H Адрес точки входа
 BaseOfCode            dd ?         ; +2CH Смещ. кода в памяти
 BaseOfData            dd ?         ; +30H Смещ. иниц-ных данных в памяти
 ImageBase             dd ?         ; +34H Image Base
 SectAlign             dd ?         ; +38H Фактор выравнивания объектов в ОЗУ
 FileAlign             dd ?         ; +3CH Фактор выравнивания объектов в файле
 MajorOSV              dw ?         ; +40H |
 MinorOSV              dw ?         ; +42H |
 MajorImV              dw ?         ; +44H |
 MinorImV              dw ?         ; +46H +> Версии и субверсии компонентов
 MajorSSV              dw ?         ; +48H |
 MinorSSV              dw ?         ; +4AH |
 Win32Vers             dd ?         ; +4CH |
 SizeOfIm              dd ?         ; +50H Размер образа программы в ОЗУ
 SizeOfHd              dd ?         ; +54H Размеp заголовка и таблицы объектов
 CSum                  dd ?         ; +58H Контpольная сумма
 SubSystem             dw ?         ; +5CH Операционная подсистема
 DLL_Flags             dw ?         ; +5EH Указывает на специальные потребости при загрузке
PEhdr                 ends

PEObjTbl              struct
 ObjName               db 8 dup (?) ; +00d Символьное имя объекта
 VirtSize              dd ?         ; +08d Размер секции объекта в памяти
 VirtRVA               dd ?         ; +12d Смещение секции от начала образа в памяти
 PhSize                dd ?         ; +16d Размер секции объекта на диске
 PhOffset              dd ?         ; +20d Смещение секции от начала файла
 R_                    dd 3 dup (?) ; +32d ???
 ObjFlags              dd ?         ; +36d Флаги свойств секции
PEObjTbl              ends
