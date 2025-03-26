#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer()
{	
	CursorPosition = {15, 50};
	color = 0xffffffff;
}
BasicRenderer::BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font)
{
	targetFramebuffer = framebuffer;
	psf1_font = font;
	CursorPosition = {15, 50};
	color = 0xffffffff;
}
BasicRenderer::BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font, unsigned int clr, Point pos)
{
	targetFramebuffer = framebuffer;
	psf1_font = font;
	color = clr;
	CursorPosition = pos;
}
BasicRenderer::BasicRenderer(FrameBuffer* framebuffer, PSF1_FONT* font, uint8_t r, uint8_t g, uint8_t b, uint8_t a, Point pos)
{
	targetFramebuffer = framebuffer;
	psf1_font = font;
	CursorPosition = pos;
	red = r;
	green = g;
	blue = b;
	alpha = a;
	color = GetColor();
}
BasicRenderer::BasicRenderer(BootInfo* bootInfo)
{
	targetFramebuffer = bootInfo->framebuffer;
	psf1_font = bootInfo->psf1_font;
	color = 0xffffffff;
}
BasicRenderer::BasicRenderer(BootInfo* bootInfo, uint8_t r, uint8_t g, uint8_t b, uint8_t a, Point pos)
{
	targetFramebuffer = bootInfo->framebuffer;
	psf1_font = bootInfo->psf1_font;	
	CursorPosition = pos;
	red = r;
	green = g;
	blue = b;
	alpha = a;
	color = GetColor();
}

uint32_t BasicRenderer::GetColor()
{
	return (alpha << 24) + (red << 16) + (green << 8) + (blue); // bitshift for each pos; hex sequence is ARGB

}

void BasicRenderer::Clear(uint32_t color){
	uint64_t fbBase = (uint64_t)targetFramebuffer->BaseAddress;
	uint64_t bytesPerScanline = targetFramebuffer->PixelsPerScanLine * 4;
	uint64_t fbHeight = targetFramebuffer->Height;
	uint64_t fbSize = targetFramebuffer->BufferSize;

	for(int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++)
	{
		uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
		for(uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++){
			*pixPtr = color;
		}
	}
}
void BasicRenderer::Next(){
	CursorPosition.x = 0;
	CursorPosition.y +=16;
}

void BasicRenderer::SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	red = r;
	green = g;
	blue = b;	
	alpha = a;
	color = GetColor();
}
void BasicRenderer::Print( const char* str)
{
	char* chr = (char*)str;
	while(*chr != 0)
	{
		putChar(*chr, CursorPosition);
		CursorPosition.x += 8;
		if(CursorPosition.x + 8 > targetFramebuffer->Width) //check if next char would overflow out of screen
		{
			CursorPosition.x = 0; //reset x pos
			CursorPosition.y +=16; // go down 1 char length
		}
		chr++;
	}
}
void BasicRenderer::putChar(char chr, Point offset)
{
    unsigned int* pixPtr = (unsigned int*)targetFramebuffer->BaseAddress; //ptr to baseAddress
    char* fontPtr = (char*)psf1_font->glyphBuffer + (chr * psf1_font->psf1_Header->charsize); //ptr to glyph inserted by user 
	for(unsigned long y = offset.y; y < offset.y + 16; y++) //select y-axis bitmap coordinate (16bit wide)
	{
		for(unsigned long x = offset.x; x < offset.x + 8; x++) //select x-axis bitmap coordinate (8bit wide)
		{
			if((*fontPtr & (0b10000000 >> (x - offset.x))) > 0 ) // bitshift a single bit to the right by x - xOff, check if it is ON with bitwise AND
			{
				*(unsigned int*)(pixPtr + x + (y * targetFramebuffer->PixelsPerScanLine)) = color; //assign colour to bit
			}
		}
		fontPtr++;
	}
}