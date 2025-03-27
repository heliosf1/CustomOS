#include "keyboard.h"

bool isLShiftPressed;
bool isRShiftPressed;



void HandleKeyboard(uint8_t scancode)
{
    switch(scancode){
        case LeftShift:
        isLShiftPressed = true;
        return;
        case LeftShift + 0x80:
        isLShiftPressed = false;
        return;
        case RightShift:
        isRShiftPressed = true;
        return;
        case RightShift + 0x80:
        isLShiftPressed = false;
        return;
        case Enter:
        GlobalRenderer->Next();
        return;
        case Spacebar:
        GlobalRenderer->putChar(' ');
        return;
        case Backspace:
        GlobalRenderer->ClearChar();
        return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLShiftPressed | isRShiftPressed);

    if(ascii != 0)
    {
        GlobalRenderer->putChar(ascii);
    }
}