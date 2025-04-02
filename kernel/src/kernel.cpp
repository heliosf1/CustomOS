
#include "kernelUtil.h"

extern "C" void _start(BootInfo* bootInfo){
	
	KernelInfo kernelInfo = InitializeKernel(bootInfo);

	PageTableManager* pageTableManager = kernelInfo.pageTableManager;
	
	GlobalRenderer->CursorPosition = {200, 200};
	GlobalRenderer->Print("Kernel initialized succesfully");
	
	while(true){
		ProcessMousePacket();
	}

	while(true);

}
