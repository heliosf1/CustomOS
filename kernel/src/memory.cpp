#include "memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize)
{
    static uint64_t memorySizeBytes = 0; //check if memory size has already been calculated 
    if(memorySizeBytes > 0) return memorySizeBytes;

    for(int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize)); //iterate through each memory entry     }
        memorySizeBytes += desc->numPages * 4096;
    }

    return memorySizeBytes;

}