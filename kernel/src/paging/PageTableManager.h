#pragma once

#include "paging.h"

class PageTableManager{
    public:
    PageTableManager(PageTable* PML4Address);
    PageTable* PML4; // Page Map Level 4 Address
    void MapMemory(void* virtualMemory, void* physicalMemory);
};