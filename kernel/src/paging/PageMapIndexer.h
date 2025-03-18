#pragma once

#include <cstdint>

class PageMapIndexer
{
    public:
    PageMapIndexer(uint64_t virtualAddress);
    uint64_t PDP_i; //L4 PageMap index (Page Directory Pointer)
    uint64_t PD_i; //Page Directory index
    uint64_t PT_i; //Page Table index
    uint64_t P_i; //Page index
};