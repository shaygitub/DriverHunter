#pragma once
#include <iostream>
#include <Windows.h>
#define VULNURABLE_IOCTL1 0x80002000
#define UNMAP_VIEWSECTION_IOCTL 0x80002004
#define READBYTE_HARDWAREPORT_IOCTL 0x80002008
#define READWORD_HARDWAREPORT_IOCTL 0x8000200C
#define READDWORD_HARDWAREPORT_IOCTL 0x80002010
#define WRITEBYTE_HARDWAREPORT_IOCTL 0x80002014
#define WRITEWORD_HARDWAREPORT_IOCTL 0x80002018
#define WRITEDWORD_HARDWAREPORT_IOCTL 0x8000201C
#define VULNURABLE_IOCTL9 0x80002024
#define USELESS_IOCTL10 0x80002028
#define USELESS_IOCTL11 0x8000202C
#define READMSR_IOCTL 0x80002030
#define WRITEMSR_IOCTL 0x80002034
#define VULNURABLE_IOCTL14 0x80002038
#define VULNURABLE_IOCTL15 0x8000203C
typedef LARGE_INTEGER PHYSICAL_ADDRESS, * PPHYSICAL_ADDRESS;


typedef struct _UNMAP_VIEWSECTION_BUFFER {
    PVOID ViewBaseAddress;
}UNMAP_VIEWSECTION_BUFFER, * PUNMAP_VIEWSECTION_BUFFER;


typedef struct _READBYTE_HARDWAREPORT_BUFFER {
    USHORT PortNumber;
    BYTE BufferPadding1;
    DWORD ValueFromPort;  // Value will actually be one byte
    BYTE BufferPadding2;
} READBYTE_HARDWAREPORT_BUFFER, * PREADBYTE_HARDWAREPORT_BUFFER;


typedef struct _READWORD_HARDWAREPORT_BUFFER {
    USHORT PortNumber;
    BYTE BufferPadding1;
    DWORD ValueFromPort;  // Value will actually be two bytes
    BYTE BufferPadding2;
} READWORD_HARDWAREPORT_BUFFER, * PREADWORD_HARDWAREPORT_BUFFER;


typedef struct _READDWORD_HARDWAREPORT_BUFFER {
    USHORT PortNumber;
    BYTE BufferPadding1;
    DWORD ValueFromPort;  // Value will actually be one byte
    BYTE BufferPadding2;
} READDWORD_HARDWAREPORT_BUFFER, * PREADDWORD_HARDWAREPORT_BUFFER;


typedef struct _WRITEBYTE_HARDWAREPORT_BUFFER {
    USHORT PortNumber;
    USHORT BufferPadding1;
    BYTE ValueToPort;
    BYTE BufferPadding2[3];
} WRITEBYTE_HARDWAREPORT_BUFFER, * PWRITEBYTE_HARDWAREPORT_BUFFER;


typedef struct _WRITEWORD_HARDWAREPORT_BUFFER {
    USHORT PortNumber;
    BYTE BufferPadding1;
    DWORD ValueToPort;
    BYTE BufferPadding2;
} WRITEWORD_HARDWAREPORT_BUFFER, * PWRITEWORD_HARDWAREPORT_BUFFER;


typedef struct _WRITEDWORD_HARDWAREPORT_BUFFER {
    USHORT PortNumber;
    BYTE BufferPadding1;
    DWORD ValueToPort;
    BYTE BufferPadding2;
} WRITEDWORD_HARDWAREPORT_BUFFER, * PWRITEDWORD_HARDWAREPORT_BUFFER;


typedef struct _READMSR_BUFFER {
    USHORT MsrToReadFrom1;
    USHORT MsrToReadFrom2;
    PVOID ValueFromMsr;  // Will contain the full value at the end
}READMSR_BUFFER, * PREADMSR_BUFFER;


typedef struct _WRITEMSR_BUFFER {
    DWORD TargetMsr;
    PVOID ValueToMsr;
}WRITEMSR_BUFFER, * PWRITEMSR_BUFFER;


namespace IoctlTriggers {
    BOOL UnmapViewOfSectionTrigger(HANDLE DriverHandle, PVOID ViewBaseAddress);
    BOOL ReadByteHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD* ValueFromPort);
    BOOL ReadWordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD* ValueFromPort);
    BOOL ReadDwordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD* ValueFromPort);
    BOOL WriteByteHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, BYTE ValueToPort);
    BOOL WriteWordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, USHORT ValueToPort);
    BOOL WriteDwordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD ValueToPort);
    BOOL ReadFromMsrTrigger(HANDLE DriverHandle, DWORD MsrToReadFrom, PVOID* ValueFromMsr);
    BOOL WriteToMsrTrigger(HANDLE DriverHandle, DWORD MsrToWriteTo, ULONG64 ValueToWrite);
}