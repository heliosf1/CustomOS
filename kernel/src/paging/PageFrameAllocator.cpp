#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool initialized = false;
PageFrameAllocator GlobalAllocator;


void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescriptorSize)
{
    if(initialized) return; //check if mMap is already initialized

    initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescriptorSize;

    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    for(int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap +(i * mMapDescriptorSize));
        if(desc->type == 7) //type = EfiConventionalMemory
        {
            if(desc->numPages * 4096 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = desc->physicalAddress;
                largestFreeMemSegSize = desc->numPages * 4096;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescriptorSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;
    
    //initialize bitmap
    InitBitmap(bitmapSize, largestFreeMemSeg);

    //lock pages of bitmap
    LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);
    //reserve pages of unusable/reserved memory
    for(int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap +(i * mMapDescriptorSize));
        if(desc->type != 7 ) // check if memory is not efiConventionalMemory
        {
            ReservePages(desc->physicalAddress, desc->numPages); //reserve pages
        }
    }

}


void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*)bufferAddress;
    for(int i = 0; i < bitmapSize; i++)
    {
        *(uint8_t*)(PageBitmap.Buffer + i) = 0;
    }

}

void* PageFrameAllocator::RequestPage()
{
    for(uint64_t index = 0; index < PageBitmap.Size * 8; index++)
    {
        if(PageBitmap[index] == true) continue; //if page is allocated continue to next page
        LockPage((void*)(index * 4096));
        return (void*)(index * 4096);
    }

    return NULL; //Page Frame Swap to File
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if(PageBitmap[index] == false) return;
    if(PageBitmap.Set(index, false)){
        freeMemory += 4096;
        usedMemory -= 4096;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount)
{
    for(int i = 0; i < pageCount; i++)
    {
        FreePage((void*)((uint64_t)address + (i * 4096)));
    }
}


void PageFrameAllocator::LockPage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if(PageBitmap[index] == true) return;
    if(PageBitmap.Set(index, true)){
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount)
{
    for(int i = 0; i < pageCount; i++)
    {
        LockPage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if(PageBitmap[index] == false) return;
    if(PageBitmap.Set(index, false)){
        freeMemory += 4096;
        reservedMemory -= 4096;
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount)
{
    for(int i = 0; i < pageCount; i++)
    {
        UnreservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;

    if(PageBitmap[index] == true) return;
    if(PageBitmap.Set(index, true)){
        freeMemory -= 4096;
        reservedMemory += 4096;
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount)
{
    for(int i = 0; i < pageCount; i++)
    {
        ReservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

uint64_t PageFrameAllocator::GetFreeRAM()
{
    return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM()
{
    return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM()
{
    return reservedMemory;
}