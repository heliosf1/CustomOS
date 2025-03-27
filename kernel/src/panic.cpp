#include "panic.h"

void Panic(const char* panicMessage)
{

    GlobalRenderer->clearColor = 0xffffffff;
    GlobalRenderer->Clear();

    GlobalRenderer->CursorPosition = {0, 0};
    GlobalRenderer->SetRGBA(127, 255, 212, 0);

    GlobalRenderer->Print("Kernel Panic");

    GlobalRenderer->Next();
    GlobalRenderer->Next();

    GlobalRenderer->Print(panicMessage);

}