#include <stdint.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"

extern "C" void _start(BootInfo* bootInfo){
    

    BasicRenderer newRenderer = BasicRenderer(bootInfo); 
	newRenderer.CursorPosition = {15, 50};
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

	newRenderer.CursorPosition = {0, 210};

	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;
	//EFI_MEMORY_DESCRIPTOR size is dynamic
	
	for(int i = 0; i < mMapEntries; i++)
	{
		EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescSize));
		newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 20};
		newRenderer.Print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
	}


	return ;
}
