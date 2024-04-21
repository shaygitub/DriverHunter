# First thoughts/notes:
first thing that is checked is if IoIs32bitProcess returns true (else: STATUS_UNSUPPORTED)
"IoIs32bitProcess returns TRUE if the originator of the current I/O request is a 32-bit user-mode process.
 Otherwise, it returns FALSE." - msdn
so - to use the vulnurable driver you have to be a 32 bit process

as i see it here, the usual switch case() for dealing with request codes is highly obfuscated, the ByteOffset
attribute of the input to the driver (will be refrenced from now as InputOffset) is the actual request code thats being checked by
subtracting numbers

# InputOffset = 0x22A004:
MmFreeContigiuousMemory() is called with a PVOID cast of the value starting at MasterIrp.Type and returns STATUS_SUCCESS
which means the virtual address of the contiguous memory to be freed is -
CSHORT                    Type;  // 0 -> 2
USHORT                    Size;  // 2 -> 4
PMDL                      MdlAddress;  // 4 -> 8, takes the first half of the module pointer
 * THIS PART IS REPEATEDALLY USED FOR INPUT PARAMETERS, NEED TO CHECK HOW TO CONTROL THIS PART
![save1](https://github.com/shaygitub/VulnDrivers/assets/122000611/f59fcc6c-9887-4801-8cad-9f8f130bbf83)


# InputOffset = 0x22A00D:
# Large part, will not get into this for now as its not vulnurable for sure

# InputOffset = 0x22A030:
# Large part, will not get into this for now as its not vulnurable for sure

# InputOffset = 0x22A034:
# Large part, will not get into this for now as its not vulnurable for sure

# InputOffset = 0x22A038:
# Large part, will not get into this for now as its not vulnurable for sure

# InputOffset = 0x22A049:
# Large part, will not get into this for now as its not vulnurable for sure

# If InputOffset is not equal to any of those values:
if the lower 32 bits of the MasterIrp->MdlAddress is higher than the input length - returns STATUS_INVALID_PARAM
these lower 32 bits are then passed to MmMapIoSpace() as the NumberOfBytes parameter: size of the I/O space
makes sense as the I/O space does not need to be bigger than the input size, as can be seen in later parts of this section
the main operations occur on that I/O space / the descriptor module provided by caller, so its reasonable that operation size
refrences the size of the I/O space

MmMapIoSpace returns the base virtual address that maps the base physical address for the range
if MmMapIoSpace() fails, STATUS_INSUFFICIENT_RESOURCES is returned from the driver
again here, the cast of MasterIrp.Type to PHYSICAL_ADDRESS is passed as the physical address to map I/O space for (which
already proves that this parameter is somehow user controlled)
![save2](https://github.com/shaygitub/VulnDrivers/assets/122000611/44a45ef8-daee-4da4-bee5-40024f32df41)




then Irp->MdlAddress is refrenced:
Irp.MdlAddress - "Pointer to an MDL describing a user buffer, if the driver is using direct I/O.
the MDL describes an empty buffer that the device or driver fills in." (msdn)
so - this module is somehow also controlled by the caller

then UserBufferModule->MdlFlags is checked, these flags describe the attributes of the MDL
the if() else() only means that the if() is triggered when the MdlFlags is equal to MDL_MAPPED_TO_SYSTEM_VA/MDL_SOURCE_IS_NONPAGED_POOL,
otherwise the else() statement is triggered
MDL_MAPPED_TO_SYSTEM_VA/MDL_SOURCE_IS_NONPAGED_POOL:
saves UserBufferModule->MappedSystemVa in a variable - takes a mapped system virtual address as a parameter for later operations
if there is not a mapped system virtual address buffer - map the module to system virtual space using MmMapLockedPagesSpecifyCache()
![save3](https://github.com/shaygitub/VulnDrivers/assets/122000611/b1ffea48-b598-4fb9-ba65-f5c904dad334)




after this, if the virtual I/O space was mapped successfully, MmUnmapIoSpace() is called to unmap the I/O space
then another function is called with virtual address of the I/O space, mapped system module that is either provided by the user or
mapped to system space from the user descriptor module, and lower 32 bits of the MasterIrp->MdlAddress (size of I/O space)

sub_11D90:
when analyzing this function, it seems to perform some arithmetic operations on the user provided IoSpace (read and write) and eventually
in any case it returns a pointer to the manipulated I/O space, this does not seem to be anything that might be vulnurable or user controlled
![save4](https://github.com/shaygitub/VulnDrivers/assets/122000611/8cd9a471-98df-4ab4-981e-2a949b3d96a8)




this case finally goes to lines 175/176 and return to Irp->IoStatus.Information (usually holds the amount of bytes that were operated
on by the driver), makes sense as this case operated on the I/O space somehow and the amount returned is the size of the user provided
system space module that describes the request
![save5](https://github.com/shaygitub/VulnDrivers/assets/122000611/02b93f7d-231e-4ccb-9f10-daa5a620d6a7)


# InputOffset = 0x22A000:
