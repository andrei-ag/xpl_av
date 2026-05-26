 extrn                  lstrlenA:near            ; список needful апи
 extrn                 _wsprintfA:near
 extrn                  GetStdHandle:near
 extrn                  WriteConsoleA:near
;─────
 extrn                  GetDriveTypeA:near
 extrn                  GetTickCount:near
 extrn                  GetCommandLineA:near
 extrn                  ExitProcess:near
;─────
 extrn                  ReadFile:near
 extrn                  WriteFile:near
 extrn                  DeleteFileA:near
 extrn                  CloseHandle:near
 extrn                  GetFileSize:near
 extrn                  CreateFileA:near
 extrn                  SetEndOfFile:near
 extrn                  SetFilePointer:near
 extrn                  GetFileAttributesA:near
 extrn                  SetFileAttributesA:near
;─────
 extrn                  FindFirstFileA:near    
 extrn                  FindNextFileA:near    
 extrn                  FindClose:near
 extrn                  GetCurrentDirectoryA:near
 extrn                  SetCurrentDirectoryA:near
;
 DRIVE_UNKNOWN         equ 0
 DRIVE_NO_ROOT_DIR     equ 1
 DRIVE_REMOVABLE       equ 2
 DRIVE_FIXED           equ 3
 DRIVE_REMOTE          equ 4
 DRIVE_CDROM           equ 5
 DRIVE_RAMDISK         equ 6
;──────────────────────
 STD_INPUT_HANDLE      equ -10
 STD_OUTPUT_HANDLE     equ -11
 STD_ERROR_HANDLE      equ -12
;
 FILE_BEGIN            equ 0
 FILE_CURRENT          equ 1
 FILE_END              equ 2
 CREATE_ALWAYS         equ 2
 OPEN_EXISTING         equ 3
 GENERIC_READ          equ 80000000h
 GENERIC_WRITE         equ 40000000h
 GENERIC_RNW           equ GENERIC_READ + GENERIC_WRITE
;
; размер одной ячейки данных в массиве "find_buf"
;
 ENTRY_SIZE            equ 1+260+4+318