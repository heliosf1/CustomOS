#pragma once

#include "Framebuffer.h"
#include "SimpleFont.h"
#include "efiMemory.h"
#include <cstdint>
struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
} ;