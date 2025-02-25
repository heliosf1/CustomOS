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
    BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font, uint8_t r, uint8_t g, uint8_t b, uint8_t a, Point pos);
    BasicRenderer(BootInfo* bootInfo);
    BasicRenderer(BootInfo* bootInfo, uint8_t r, uint8_t g, uint8_t b, uint8_t a, Point pos);
    Point CursorPosition;
	FrameBuffer* targetFramebuffer;
	PSF1_FONT* psf1_font;
	unsigned int color;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
    uint32_t GetColor();
    void Print(const char* str);
    void putChar(char chr, Point offset);

};