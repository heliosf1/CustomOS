#pragma once

#include <stddef.h>

//frame buffer info
struct FrameBuffer{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine; //FrameBuffer might have more pixels than width, for specific graphics features
};