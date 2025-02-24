#pragma once

#include "Framebuffer.h"
#include "SimpleFont.h"
#include <cstdint>

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_font;
	void* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
} ;