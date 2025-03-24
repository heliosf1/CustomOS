
#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
	
	KernelInfo kernelInfo = InitializeKernel(bootInfo);

	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	
	GlobalRenderer->CursorPosition = {200, 200};
	GlobalRenderer->Print("Kernel initialized succesfully");


	for(int t = 0; t < 2000; t++)
	{
		GlobalAllocator.RequestPage();
	}
	GlobalRenderer->CursorPosition = {0, 200};
	GlobalRenderer->Print("Done");
	while(true);

}
