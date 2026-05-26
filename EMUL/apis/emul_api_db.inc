;
;                    * * *  LIST OF EMULATED APIS * * *
;

;
; Emulator can run these list of API
; (of course with some tests to avoid errors)
;
 API_number              = 31
 API_names:

 __GetCommandLineA       db 'GetCommandLineA',0       ; DWORD GetCommandLineaA(void)
 __GetStartupInfoA       db 'GetStartupInfoA',0       ; void GetStartupInfoA(Pointer)
 __GetEnvironmentStrings db 'GetEnvironmentStrings',0 ; DWORD GetEnvironmentStrings(void)
 __GetVersion            db 'GetVersion',0            ; DWORD GetVersion(void)
; db 'GetModuleHandleA,0               ; DWORD GetModuleHandleA(Pointer) ; DISABLED
 __GetModuleFileNameA    db 'GetModuleFileNameA',0    ; DWORD GetModuleFileNameA(Handle,Buffer,Buffer_Size)
 __MulDiv                db 'MulDiv',0                ; DWORD MulDiv(Random,Random,Random)
 __GetACP                db 'GetACP',0                ; DWORD GetACP(void)
 __GetOEMCP              db 'GetOEMCP',0              ; DWORD GetOEMCP(void)
 __GetCPInfo             db 'GetCPInfo',0             ; DWORD GetCPInfo(CodePage,Buffer)
 __GetStdHandle          db 'GetStdHandle',0          ; DWORD GetStdHandle(Flags)
 __GetLastError          db 'GetLastError',0          ; DWORD GetLastError(void)
 __GetLocalTime          db 'GetLocalTime',0          ; void GetLocalTime(Buffer)
 __GetSystemInfo         db 'GetSystemInfo',0         ; void GetSystemInfo(Buffer)
 __GetCurrentProcess     db 'GetCurrentProcess',0     ; DWORD GetCurrentProcess(void)
 __GetCurrentProcessID   db 'GetCurrentProcessID',0   ; DWORD GetCurrentProcessID(void)
 __GetCurrentThread      db 'GetCurrentThread',0      ; DWORD GetCurrentThread(void)
 __GetCurrentThreadID    db 'GetCurrentThreadID',0    ; DWORD GetCurrentThreadID(void)
 __GetConsoleCP          db 'GetConsoleCP',0          ; DWORD GetConsoleCP(void)
 __GetCurrentDirectoryA  db 'GetCurrentDirectoryA',0  ; DWORD GetCurrentDirectoryA(Buffer_Size,Buffer)
 __GetWindowsDirectoryA  db 'GetWindowsDirectoryA',0  ; DWORD GetWindowsDirectoryA(Buffer,Buffer_Size)
 __GetSystemDirectoryA   db 'GetSystemDirectoryA',0   ; DWORD GetSystemDirectoryA(Buffer,Buffer_Size)
 __GetDriveTypeA         db 'GetDriveTypeA',0         ; DWORD GetDriveTypeA(NULL)
 __GetComputerNameA      db 'GetComputerNameA',0      ; BOOL GetComputerNameA(Buffer,Buffer_Size)
 __IsBadWritePtr         db 'IsBadWritePtr',0         ; BOOL IsBadWritePtr(Buffer,Buffer_Size)
 __GetTickCount          db 'GetTickCount',0          ; DWORD GetTickCount(void)
 __IsBadReadPtr          db 'IsBadReadPtr',0          ; BOOL IsBadReadPtr(Buffer,Buffer_Size)
 __IsBadCodePtr          db 'IsBadCodePtr',0          ; BOOL IsBadCodePtr(Buffer)
 __GetCurrentFiber       db 'GetCurrentFiber',0       ; DWORD GetCurrentFiber(void)
 __LocalHandle           db 'LocalHandle',0           ; DWORD LocalHandle(Address)
 __LocalSize             db 'LocalSize',0             ; DWORD LocalSize(Address)
 __LocalFlags            db 'LocalFlags',0            ; DWORD LocalFlags(Handle)
;
  db 0BBh                                    ; THE END

 LIB_names:
 __KERNEL32_DLL          db 'KERNEL32.DLL',0
                         db 0BBh

;
;                     * * *   APIs database    * * *
;
; FORMAT: ASCII name pointer, LIB ASCII name pointer, number of parameters
;         buffer parameter (number), buffer size parameter (number) 
;
 API_database:
 	dd __GetCommandLineA  ; *1
 	dd __KERNEL32_DLL
        db 0, 0, 0
 	dd __GetStartupInfoA  ; *2
 	dd __KERNEL32_DLL
 	db 1, 0, 0
        dd __GetEnvironmentStrings   ; *3
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetVersion              ; *4
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetModuleFileNameA      ; *5
 	dd __KERNEL32_DLL
        db 3, 2, 3      ;  DWORD GetModuleFileNameA(Handle,Buffer,Buffer_Size)
        dd __MulDiv                  ; *6
 	dd __KERNEL32_DLL
        db 3, 0, 0
        dd __GetACP                  ; *7
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetOEMCP                ; *8
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetCPInfo               ; *9
 	dd __KERNEL32_DLL
        db 2, 2, 0                   ; DWORD GetCPInfo(CodePage,Buffer)
        dd __GetStdHandle            ; *10
 	dd __KERNEL32_DLL
        db 1, 0, 0
        dd __GetLastError            ; *11
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetLocalTime            ; *12
 	dd __KERNEL32_DLL
        db 1, 1, 0                   ; void GetLocalTime(Buffer)
        dd __GetSystemInfo           ; *13
 	dd __KERNEL32_DLL
        db 1, 1, 0                   ; void GetSystemInfo(Buffer)
        dd __GetCurrentProcess       ; * 14
 	dd __KERNEL32_DLL
        db 0, 0, 0                        
        dd __GetCurrentProcessID     ; * 15
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetCurrentThread        ; * 16
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetCurrentThreadID      ; * 17
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetConsoleCP            ; * 18
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __GetCurrentDirectoryA    ; * 19
 	dd __KERNEL32_DLL
        db 2, 2, 1                   ; DWORD GetCurrentDirectoryA(Buffer_Size,Buffer)
        dd __GetWindowsDirectoryA    ; * 20
 	dd __KERNEL32_DLL
        db 2, 1, 2                   ; DWORD  GetWindowsDirectoryA(Buffer,Buffer_Size)
        dd __GetSystemDirectoryA     ; * 21
 	dd __KERNEL32_DLL
        db 2, 1, 2                   ; DWORD GetSystemDirectoryA(Buffer,Buffer_Size)
        dd __GetDriveTypeA	     ; * 22
 	dd __KERNEL32_DLL
        db 1, 0, 0
        dd __GetComputerNameA	     ; * 23
 	dd __KERNEL32_DLL
        db 2, 1, 2                   ; BOOL GetComputerNameA(Buffer,Buffer_Size)
        dd __IsBadWritePtr	     ; * 24
 	dd __KERNEL32_DLL
        db 2, 1, 2                   ; BOOL IsBadWritePtr(Buffer,Buffer_Size)
        dd __GetTickCount	     ; * 25
 	dd __KERNEL32_DLL
        db 0, 0, 0
        dd __IsBadReadPtr	     ; * 26
 	dd __KERNEL32_DLL
        db 2, 1, 2                   ; BOOL IsBadReadPtr(Buffer,Buffer_Size)
        dd __IsBadCodePtr	     ; * 27
 	dd __KERNEL32_DLL
        db 1, 1, 0                   ; BOOL IsBadCodePtr(Buffer)
        dd __GetCurrentFiber	     ; * 28
 	dd __KERNEL32_DLL
        db 0, 0, 0                   
        dd __LocalHandle	     ; * 29 !!!!!!!
 	dd __KERNEL32_DLL
        db 1, 0, 0                   ; DWORD LocalHandle(Address)
        dd __LocalSize		     ; * 30 !!!!!!!
 	dd __KERNEL32_DLL
        db 1, 0, 0                   ; DWORD LocalSize(Address)
        dd __LocalFlags		     ; * 31 !!!!!!!
 	dd __KERNEL32_DLL
        db 1, 0, 0
 API_database_end:
;
;               * * *  FOR APIS IMPORTED BY EMULATED PE FILE * * *
;
 MAX_APIS                 = 3000
;                        
 import_data_va           dd ?  ; Virtual address of import table (relative)
 import_data_raw          dd ?  ; Physical address of import table (relative)
;
 temp_module_handle       dd ?  ; TEMP buffer
;
; BUFFER FORMAT: API NAME (DWORD), API OFFSET (DWORD), LIB NAME (DWORD)
;                (You can set MAX_APIS number)
;
 imported_api_number      dd ?
 imported_api_buffer      db ( (3*4) * MAX_APIS ) dup (?)