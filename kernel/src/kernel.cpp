#include <stdint.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include "bitmap.h"



extern "C" void _start(BootInfo* bootInfo){
    

    BasicRenderer newRenderer = BasicRenderer(bootInfo, 127, 255, 212, 0, {500, 0}); 
	// newRenderer.Print(toHexString(newRenderer.color));
	// newRenderer.CursorPosition = {500, 16};
	// newRenderer.Print(toHexString((uint8_t)newRenderer.red));
	// newRenderer.CursorPosition = {500, 32};
	// newRenderer.Print(toHexString((uint8_t)newRenderer.green));
	// newRenderer.CursorPosition = {500, 48};
	// newRenderer.Print(toHexString((uint8_t)newRenderer.blue));
	// newRenderer.CursorPosition = {500, 64};
	// newRenderer.Print(toString((uint64_t)2342));
	// newRenderer.CursorPosition = {500, 80};
	// newRenderer.Print(toString((int64_t)-242));
	// newRenderer.CursorPosition = {500, 96};
	// newRenderer.Print(toString((double)-42.26));
	// newRenderer.CursorPosition = {500,112};
	// newRenderer.Print(toHexString((uint64_t)0xF2));
	// newRenderer.CursorPosition = {500, 130};
	// newRenderer.Print(toHexString((uint32_t)0xFA));
	// newRenderer.CursorPosition = {500, 150};
	// newRenderer.Print(toHexString((uint16_t)0xF3));
	// newRenderer.CursorPosition = {500, 170};
	// newRenderer.Print(toHexString((uint8_t)0xF5));
	// newRenderer.CursorPosition = {500, 190};

	// newRenderer.CursorPosition = {500, 210};

	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	uint8_t testBuffer[20];

	Bitmap testBitmap;
	testBitmap.Buffer = &testBuffer[0];
	testBitmap.Set(0, false);
	testBitmap.Set(1, true);
	testBitmap.Set(2, false);
	testBitmap.Set(3, true);
	testBitmap.Set(4, true);

	for(int i = 0; i < 20; i++)
	{
		newRenderer.CursorPosition = {500, newRenderer.CursorPosition.y + 16};
		newRenderer.Print(testBitmap[i] ? "true" : "false");
	}
	newRenderer.CursorPosition = {500, newRenderer.CursorPosition.y + 16};

	newRenderer.Print(toString(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize)));

	//EFI_MEMORY_DESCRIPTOR size is dynamic

	// for(int i = 0; i < mMapEntries; i++)
	// {
	// 	EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescSize)); //iterate through each memory entry 
	// 	newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 20};
	// 	newRenderer.Print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
	// 	newRenderer.color = 0x007fffd4;
	// 	newRenderer.Print(" ");
	// 	newRenderer.Print(toString(desc->numPages * 4096 / 1024 /*get amount of KB, each page is 4KB */));
	// 	newRenderer.Print(" KB");
	// 	newRenderer.color = 0xffffffff;
	// }


	return ;
}
