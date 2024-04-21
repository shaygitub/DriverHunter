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

# If InputOffset is not equal to any of those values (InputOffset has to be 0x22A051 or its not implemented):
This case takes care of the possibility where invalid InputOffset is provided. if the left over is not eight (InputOffset != 0x22A051),
the driver returns STATUS_NOT_IMPLEMENTED). when InputOffset = 0x22A051 firstly MasterIrp->Type is refrenced similarly to earlier operations,
but this time the IRP.Type+Size are interpreted as an int* (in 32bits it just means that this is a pointer to a 4byte value).

# InputOffset = 0x22A049:
if the lower 32 bits of the MasterIrp->MdlAddress is higher than the input length - returns STATUS_INVALID_PARAM
these lower 32 bits are then passed to MmMapIoSpace() as the NumberOfBytes parameter: size of the I/O space
makes sense as the I/O space does not need to be bigger than the input size, as can be seen in later parts of this section
the main operations occur on that I/O space / the descriptor module provided by caller, so its reasonable that operation size
refrences the size of the I/O space

MmMapIoSpace returns the base virtual address that maps the base physical address for the range
if MmMapIoSpace() fails, STATUS_INSUFFICIENT_RESOURCES is returned from the driver
again here, the cast of MasterIrp.Type to PHYSICAL_ADDRESS is passed as the physical address to map I/O space for (which
already proves that this parameter is somehow user controlled, in this case Type+Size describe a physical address)
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
![save7](https://github.com/shaygitub/VulnDrivers/assets/122000611/629a1206-5682-401d-8942-9ef7425ad16b)

This case seems to be a good attack vector as:
1) its refrenced on its own (the last ones were checked compared to 0x22A000 then subtracted from when higher than 0x22A000)
2) right at the start this case refrences Length, which is just the input size, provided by the caller as the nInBufferSize parameter in DeviceIoControl()

as can be seen, Length has to be equal/bigger than 16 (a good sign - usually when this shows up in IDA it means that there is a base requirement
of some sort of a struct, this time it needs to be 16 bytes long, and after it some input comes). this might be really helpful for the exploitation process
as we already have the size of the input we have to provide for the driver:
0-16: base struct/data of some kind
16+: input parameters
weirdly, if this requirement is not met (Length < 16), STATUS_INSUFFICIENT_RESOURCES is returned. probably not relevant for now

this case has some interesting operations. firstly, the same field we saw twice earlier is used to pass a parameter from the user to the driver (v36). secondly,
a local PHYSICAL_ADDRESS is initialized with 0xFFFFFFFF (highest possible 32 bit address that is acceptable). and lastly, the same user controlled
variable (v36) is shifted left 12 times. this weridly makes it so the value passed to MmAllocateContiguousMemory (number of bytes to allocate) is the value described here:

![save6](https://github.com/shaygitub/VulnDrivers/assets/122000611/0f1d5962-c5a5-47ed-ade7-c925aabd805f)
so basically:
1) this case allocates contigiuous memory, the size is provided and controlled by the caller
2) this allocated memory is parsed as a PMDL, meaning the allocated memory is the system space descriptor module that "the device or driver fills in." (msdn, refrenced earlier)
3) the pointer to this system space allocated descriptor module is returned in MasterIrp->MdlAddress, which can be probably parsed by the caller
4) the physical address of that contiguous memory is returned in that same area of Type+Size in the IRP structure
5) Irp->IoStatus.Information is set to 16 (the amount of input bytes operated on, here as i mentioned the minimum/needed amount is 16 so it makes sense)

* This might be relevant to InputOffset = 0x22A004, which frees contiguous memory that is passed by the caller
* This might also be relevant to InputOffset = 0x22A049, that operation receives a systemspace mapped descriptor module of some
  kind of memory (exactly part 3 here), this part should be called probably before as it checks if a systemspace module was provided, the self allocating else() statement is probably
  just used to make sure
