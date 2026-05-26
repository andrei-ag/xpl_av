section '.idata' import data readable writeable

    library kernel,'kernel32.dll',\
        user,'user32.dll'

    import kernel,\
        lstrlenA,'lstrlenA',\
        GetProcessHeap,'GetProcessHeap',\
        GetStdHandle,'GetStdHandle',\
        WriteConsoleA,'WriteConsoleA',\
        ExitProcess,'ExitProcess',\         
        GetCommandLineA,'GetCommandLineA',\  
        GetProcAddress,'GetProcAddress',\
        GetModuleHandleA,'GetModuleHandleA',\
        GetDriveTypeA,'GetDriveTypeA',\
        GetTickCount,'GetTickCount',\
        HeapFree,'HeapFree',\
        HeapAlloc,'HeapAlloc',\
        HeapCreate,'HeapCreate',\
        HeapDestroy,'HeapDestroy',\
        GetCurrentDirectoryA,'GetCurrentDirectoryA',\
        SetCurrentDirectoryA,'SetCurrentDirectoryA',\
        FindFirstFileA,'FindFirstFileA',\    
        FindNextFileA,'FindNextFileA',\      
        FindClose,'FindClose',\             
        CreateFileA,'CreateFileA',\          
        CloseHandle,'CloseHandle',\         
        ReadFile,'ReadFile',\                 
        WriteFile,'WriteFile',\          
        DeleteFileA,'DeleteFileA',\
        GetFileSize,'GetFileSize',\
        SetEndOfFile,'SetEndOfFile',\
        SetFilePointer,'SetFilePointer',\
        GetFileAttributesA,'GetFileAttributesA',\
        SetFileAttributesA,'SetFileAttributesA'
;
    import user,\
        wsprintf,'wsprintfA'