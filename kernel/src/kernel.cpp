
#include "KernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
	
	KernelInfo kernelInfo = InitializeKernel(bootInfo);

	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	
    BasicRenderer newRenderer = BasicRenderer(bootInfo, 127, 255, 212, 0, {500, 0}); 

	newRenderer.CursorPosition = {200, 200};
	newRenderer.Print("Kernel initialized succesfully");


	while(true);
	
}
