#include "paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled)
{
    uint64_t bitSelector = (uint64_t)1 << flag; //bitshift to appropriate place;
    Value &= ~bitSelector;
    if(enabled){
        Value |= bitSelector;
    }
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag)
{
    uint64_t bitSelector = (uint64_t)1 << flag;
    return Value & bitSelector > 0 ? true : false;
}

uint64_t PageDirectoryEntry::GetAddress(){
    return(Value & 0x000ffffffffff000) >> 12; //each f is 4 bits, 40 bits which defines the address, bitshift to the right to get actual address
}

void PageDirectoryEntry::SetAddress(uint64_t address)
{
    address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff; //clear address
    Value |= (address << 12);
}