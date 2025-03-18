
#include "KernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
	
	KernelInfo kernelInfo = InitializeKernel(bootInfo);

	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	
    BasicRenderer newRenderer = BasicRenderer(bootInfo, 127, 255, 212, 0, {500, 0}); 

	newRenderer.CursorPosition = {200, 200};
	newRenderer.Print("Kernel initialized succesfully");


	for(int t = 0; t < 2000; t++)
	{
		GlobalAllocator.RequestPage();
	}
	newRenderer.CursorPosition = {0, 200};
	newRenderer.Print("Done");
	while(true);

}
