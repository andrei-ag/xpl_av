;──────────────────────────────────────────────────────────────────────────────
; Формат участка памяти, который динамически выделяется для загрузки PE:
;
; 1) PE файл
; 2) Блок под виртуальный стек (размера "virtual_stack_size")
; 3) 1000 байт резерв
; 4) 1000 байт резерв под структуру PEB
; 5) Блок для базы данных кодо-анализатора (размера "virtual_stack_size-2000")
;
 proc load_pe   
;
                push    eax ecx ebx ebp edi     
;
                mov     eax,dword [temp+024]     ; Размер файла
                add     eax,virtual_stack_size*2 ; + виртуальный стек
                                                 ; + вспомогательный стек
                                                 ;   для хранения смещений
                push    eax
                push    4h+8h                    ; HEAP_GENERATE_EXCEPTIONS
                                                 ; HEAP_ZERO_MEMORY
                push    [heap_handle]            ; Выделим участок памяти
                invoke  HeapAlloc                ; под разбираемый файл
                mov     dword [file_map_address],eax
                or      eax,eax
                jnz     load_pe_read_file
                stc
                jmp     load_pe_ret
                 		
 load_pe_read_file:
                add     eax,dword [temp+024]     ; Указатель на конец файла в памяти
                                                 ; он же, начало вирт. стека
                mov     dword [virtual_stack_pointer],eax
;
; Устанавливаем указатель на базу данных кодо-анализатора
;
                mov     dword [ca_base_pointer],eax
                add     dword [ca_base_pointer],virtual_stack_size+2000
;                
                push    FILE_BEGIN 0 0 [handle]  ; установим указатель на
                invoke  SetFilePointer           ; самое начало файла
;
                mov     edi,header_temp_buffer   ; EDI = Начало заголовка PE
                mov     ebx,dword [edi+34h]      
                mov     dword [image_base_of_pe],ebx
;
                sub     ebx,ebx
                mov     bx,word [edi+014h]       ; Size of optional header
                add     ebx,018h
                add     ebx,dword [ph_loc]       ; EDX = смещение данных 1ой
                                                 ;       секции, в файле
;
                sub     edx,edx
                mov     dx,word [edi+006h]       ; Количество секций файла
                push    edx
                mov     eax,028h
                mul     edx
;
                add     eax,ebx
                push    eax
                push    dword [file_map_address] ; Прочитаем весь заголовок 
                call    read_file                ; файла, включая данные о секциях
                                                 ; и первую секцию
;
                add     ebx,dword [file_map_address]; Смещение данных первой
                                                    ; секции в памяти
                pop     ecx                      ; ECX = Количество секций
;
 load_pe_loop:  push    ecx
;
                call    count_new_vsize          ; Посчитаем новый вирт. размер
                mov     dword [ebx+008d],eax     ; Новый виртуальный размер
                mov     dword [ebx+016d],eax     ; Новый физический размер
;
                push    FILE_BEGIN
                push    0
                push    dword [ebx+020d]
                push    [handle]                 ; установим указатель на
                invoke  SetFilePointer           ; начало текущей секции в файле
;
                mov     eax,dword [ebx+012d]     ; Положение секции в памяти
                mov     dword [ebx+020d],eax     ; Установим новое положение
                                                 ; секции в файле
                add     eax,dword [file_map_address]
                                                 ; Положение секции в памяти
;
                push    dword [ebx+016d]         ; прочитаем секцию в 
                push    eax                      ; ее месторасположение
                call    read_file                
;
                pop     ecx
                add     ebx,ot_size              ; След. секция
                loop    load_pe_loop
;
; Установим структуру PEB в область сразу за виртуальным стеком + 1000
;
                mov     eax,dword [virtual_stack_pointer]
                add     eax,virtual_stack_size+1000
                push    dword [ph_loc]
                call    set_PEB
;
                clc
 load_pe_ret:   pop     edi ebp ebx ecx eax
                ret
;
 endp
;──────────────────────────────────────────────────────────────────────────────
; count_new_vsize - Расчет нового виртуального размера секции
;                   (вирт. размер секции+выравнивание)
;
; on start: ecx=1 - последняя секция
;           ebx   - данные текущей секции
; on exit:  новый виртуальный и физический размеры секции
;
 proc count_new_vsize
                ;int 3
	        cmp     ecx,1
                jbe     last_section

                mov     eax,dword [ebx+ot_size+12d] ; Смещение след. секции от начала образа в памяти
                sub     eax,dword [ebx+12d]      ; вычтем смещение тек. секции
                                                 ; получим размер без смещений

                mov     dword [ebx+008d],eax     ; Установим новый вирт. размер
                ret
               
 last_section:  mov     eax,dword [ebx+008d]     ; Виртуальный размер секции
                ret
 endp
;
;──────────────────────────────────────────────────────────────────────────────
; set_PEB - Устанавливаем структуру PEB в области памяти,
;           сразу за виртуальным стеком
;
; on start: pe_location - Начало заголовка PE
;                   EAX - Смещение для структуры PEB
;
 proc set_PEB           pe_location
                push    ecx
                mov     ecx,dword [virtual_stack_pointer]
                mov     dword [ecx+030h],eax        ; Установим указатель на PEB
;
                mov     ecx,dword [image_base_of_pe] ; file_map_address]
                mov     dword [eax+8],ecx           ; Set image base address
                pop     ecx
                ret
 endp
;
; Структура PEB:
; 
;typedef struct _PEB {
;  BOOLEAN                 InheritedAddressSpace;
;  BOOLEAN                 ReadImageFileExecOptions;
;  BOOLEAN                 BeingDebugged;
;  BOOLEAN                 Spare;
;  HANDLE                  Mutant;
;  PVOID                   ImageBaseAddress;
;  PPEB_LDR_DATA           LoaderData;
;  PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
;  PVOID                   SubSystemData;
;  PVOID                   ProcessHeap;
;  PVOID                   FastPebLock;
;  PPEBLOCKROUTINE         FastPebLockRoutine;
;  PPEBLOCKROUTINE         FastPebUnlockRoutine;
;  ULONG                   EnvironmentUpdateCount;
;  PPVOID                  KernelCallbackTable;
;  PVOID                   EventLogSection;
;  PVOID                   EventLog;
;  PPEB_FREE_BLOCK         FreeList;
;  ULONG                   TlsExpansionCounter;
;  PVOID                   TlsBitmap;
;  ULONG                   TlsBitmapBits[0x2];
;  PVOID                   ReadOnlySharedMemoryBase;
;  PVOID                   ReadOnlySharedMemoryHeap;
;  PPVOID                  ReadOnlyStaticServerData;
;  PVOID                   AnsiCodePageData;
;  PVOID                   OemCodePageData;
;  PVOID                   UnicodeCaseTableData;
;  ULONG                   NumberOfProcessors;
;  ULONG                   NtGlobalFlag;
;  BYTE                    Spare2[0x4];
;  LARGE_INTEGER           CriticalSectionTimeout;
;  ULONG                   HeapSegmentReserve;
;  ULONG                   HeapSegmentCommit;
;  ULONG                   HeapDeCommitTotalFreeThreshold;
;  ULONG                   HeapDeCommitFreeBlockThreshold;
;  ULONG                   NumberOfHeaps;
;  ULONG                   MaximumNumberOfHeaps;
;  PPVOID                  *ProcessHeaps;
;  PVOID                   GdiSharedHandleTable;
;  PVOID                   ProcessStarterHelper;
;  PVOID                   GdiDCAttributeList;
;  PVOID                   LoaderLock;
;  ULONG                   OSMajorVersion;
;  ULONG                   OSMinorVersion;
;  ULONG                   OSBuildNumber;
;  ULONG                   OSPlatformId;
;  ULONG                   ImageSubSystem;
;  ULONG                   ImageSubSystemMajorVersion;
;  ULONG                   ImageSubSystemMinorVersion;
;  ULONG                   GdiHandleBuffer[0x22];
;  ULONG                   PostProcessInitRoutine;
;  ULONG                   TlsExpansionBitmap;
;  BYTE                    TlsExpansionBitmapBits[0x80];
;  ULONG                   SessionId;
;
;} PEB, *PPEB; 