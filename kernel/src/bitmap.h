#pragma once

#include <cstdint>
#include <stddef.h>

class Bitmap{
    public:
    size_t Size; //bufferSize that bitmap uses
    uint8_t* Buffer; //ptr to start of buffer for bitmap
    bool operator[](uint64_t index);
    void Set(uint64_t index, bool value);

};

