#include "BasicRenderer.h"
#include "cstr.h"

extern "C" /*ext C for linker to find and link as entry function*/ void _start(FrameBuffer* framebuffer, PSF1_FONT* psf1_font)
{	
	BasicRenderer newRenderer = BasicRenderer(framebuffer, psf1_font); 
	newRenderer.Print(toString((uint64_t)2342));
	newRenderer.CursorPosition = {15, 70};
	newRenderer.Print(toString((int64_t)-242));
	newRenderer.CursorPosition = {15, 90};
	newRenderer.Print(toString((double)-42.26));
	newRenderer.CursorPosition = {15,110};
	newRenderer.Print(toHexString((uint64_t)0xF2));
	newRenderer.CursorPosition = {15, 130};
	newRenderer.Print(toHexString((uint32_t)0xFA));
	newRenderer.CursorPosition = {15, 150};
	newRenderer.Print(toHexString((uint16_t)0xF3));
	newRenderer.CursorPosition = {15, 170};
	newRenderer.Print(toHexString((uint8_t)0xF5));
	newRenderer.CursorPosition = {15, 190};
	return ;
}
