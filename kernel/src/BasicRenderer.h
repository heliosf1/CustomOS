#pragma once

#include "math.h"
#include "Framebuffer.h"
#include "SimpleFont.h"
#include <stdint.h>
#include "BootInfo.h"

class BasicRenderer{
    public:
    BasicRenderer();
    BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font);
    BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font, unsigned int clr, Point pos);
    BasicRenderer(BootInfo* bootInfo);
    Point CursorPosition;
	FrameBuffer* targetFramebuffer;
	PSF1_FONT* psf1_font;
	unsigned int color;
    void Print(const char* str);
    void putChar(char chr, Point offset);

};