#include <stdint.h>
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include "bitmap.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageTableManager.h"
#include "paging/paging.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;


extern "C" void _start(BootInfo* bootInfo){

	
	GlobalAllocator = PageFrameAllocator();
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;
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
	
	GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	newRenderer.CursorPosition = {0, 16};
	newRenderer.Print("Free RAM: ");
	newRenderer.Print(toString(GlobalAllocator.GetFreeRAM() / 1024));
	newRenderer.Print(" KB.");	
	
	newRenderer.CursorPosition = {0, 32};
	newRenderer.Print("Used RAM: ");
	newRenderer.Print(toString(GlobalAllocator.GetUsedRAM() / 1024));
	newRenderer.Print(" KB.");

	newRenderer.CursorPosition = {0, 48};
	newRenderer.Print("Reserved RAM: ");
	newRenderer.Print(toString(GlobalAllocator.GetReservedRAM() / 1024));
	newRenderer.Print(" KB.");
	newRenderer.CursorPosition = {0, 64};

	
	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart; //size of kernel in memory
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1; //number of pages needed for kernel

	
	GlobalAllocator.LockPages(&_KernelStart, kernelPages);

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memoryset(PML4, 0, 0x1000);
	
	PageTableManager pageTableManager = PageTableManager(PML4);

	for(uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000){
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;

	for(uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
		pageTableManager.MapMemory((void*)t, (void*)t);
	}

	asm("mov %0, %%cr3" : : "r" (PML4)); //put PML4 into register0; move value into cr3 register

	pageTableManager.MapMemory((void*)0x600000000, (void*)0x80000);
	uint64_t* test = (uint64_t*)0x600000000;
	*test = 26;
	
	newRenderer.CursorPosition = {200, 200};
	newRenderer.Print(toString(*test));
	newRenderer.Print(" - ");
	newRenderer.Print("New Map!");

	
	newRenderer.CursorPosition = {0, 80};
	PageMapIndexer pageIndexer = PageMapIndexer(0x2000);
	newRenderer.Print(toString(pageIndexer.P_i));
	newRenderer.Print(" - ");
	newRenderer.Print(toString(pageIndexer.PT_i));
	newRenderer.Print(" - ");
	newRenderer.Print(toString(pageIndexer.PD_i));
	newRenderer.Print(" - ");
	newRenderer.Print(toString(pageIndexer.PDP_i));
	newRenderer.Print(" - ");
	newRenderer.Print(toString(pageIndexer.PDP_i));
	newRenderer.Print(" - ");


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
