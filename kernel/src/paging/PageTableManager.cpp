#include "PageTableManager.h"
#include "PageMapIndexer.h"
#include "PageFrameAllocator.h"
#include "../memory.h"
#include <cstdint>


PageTableManager::PageTableManager(PageTable* PML4Address){
    this->PML4 = PML4Address; 
}

void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory)
{
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    PDE = PML4->entries[indexer.PDP_i]; //index into L4 Page Map Address
    PageTable* PDP; 
    if(!PDE.Present){
        PDP = (PageTable*)GlobalAllocator.RequestPage();
        memoryset(PDP, 0, 0x1000);
        PDE.Address = (uint64_t)PDP >> 12;
        PDE.Present = true;
        PDE.ReadWrite = true;
        PML4->entries[indexer.PDP_i] = PDE;
    }
    else{
        PDP = (PageTable*)((uint64_t)PDE.Address << 12); //assign logical address to PDE
    }

    
    PDE = PDP->entries[indexer.PD_i]; //index into Page Directory Map Address
    PageTable* PD; 
    if(!PDE.Present){
        PD = (PageTable*)GlobalAllocator.RequestPage();
        memoryset(PD, 0, 0x1000);
        PDE.Address = (uint64_t)PD >> 12;
        PDE.Present = true;
        PDE.ReadWrite = true;
        PDP->entries[indexer.PD_i] = PDE;
    }
    else{
        PD = (PageTable*)((uint64_t)PDE.Address << 12); //assign logical address to PDE
    }

    
    PDE = PD->entries[indexer.PT_i]; //index into Page Table Address
    PageTable* PT; 
    if(!PDE.Present){
        PT = (PageTable*)GlobalAllocator.RequestPage();
        memoryset(PT, 0, 0x1000);
        PDE.Address = (uint64_t)PT >> 12;
        PDE.Present = true;
        PDE.ReadWrite = true;
        PD->entries[indexer.PT_i] = PDE;
    }
    else{
        PT = (PageTable*)((uint64_t)PDE.Address << 12); //assign logical address to PDE
    }
    
    PDE = PT->entries[indexer.P_i]; // index into Page
    PDE.Address = (uint64_t)physicalMemory >> 12;
    PDE.Present = true;
    PDE.ReadWrite = true;
    PT->entries[indexer.P_i] = PDE; //assign page values to page

}