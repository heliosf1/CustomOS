#include "mouse.h"

void MouseWait(){
    uint64_t timeout = 100000;
    while(timeout--){
        if((inb(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while(timeout--){
        if(inb(0x64) & 0b1){
            return;
        }
    }
}

void MouseWrite(uint8_t value){
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return inb(0x60);
}

uint8_t mouseCycle = 0;
uint8_t mousePacket[4];
bool mousePacketReady = false;
Point mousePosition;
void HandlePS2Mouse(uint8_t data){
    switch(mouseCycle)
    {
        case 0:
            if(mousePacketReady) break;
            if(data & 0b00001000 == 0) break;
            mousePacket[0] = data;
            mouseCycle++;
            break;
        case 1:
            if(mousePacketReady) break;
            mousePacket[1] = data;
            mouseCycle++;
            break;
        case 2:
            if(mousePacketReady) break;
            mousePacket[2] = data;
            mousePacketReady = true;
            mouseCycle = 0;
            break;
        // case 3:
        //     if(mousePacketReady) break;
        //     mousePacket[3] = data;
        //     mousePacketReady = true;
        //     mouseCycle = 0;
        //     break;
    }
}

void ProcessMousePacket(){
    if(!mousePacketReady) return;

        bool xNegative, yNegative, xOverflow, yOverflow;

        if(mousePacket[0] & PS2XSign){
            xNegative = true;
        } else xNegative = false;

        if(mousePacket[0] & PS2YSign){
            yNegative = true;
        } else yNegative = false;

        if(mousePacket[0] & PS2XOverflow){
            xOverflow = true;
        } else xOverflow = false;

        if(mousePacket[0] & PS2YOverflow){
            yOverflow = true;
        } else yOverflow = false;

        if(!xNegative){
            mousePosition.x += mousePacket[1];
            if(xOverflow){
                mousePosition.x += 255;
            }
        }
        else{
            mousePacket[1] = 256 - mousePacket[1];
            mousePosition.x -= mousePacket[1];
            if(xOverflow){
                mousePosition.x -= 255;
            } 
        }

        if(!yNegative){
            mousePosition.y -= mousePacket[2];
            if(yOverflow){
                mousePosition.y -= 255;
            }
        }
        else{
            mousePacket[2] = 256 - mousePacket[2];
            mousePosition.y += mousePacket[2];
            if(yOverflow){
                mousePosition.y += 255;
            } 
        }

        if(mousePosition.x < 0) mousePosition.x = 0;
        if(mousePosition.x  > GlobalRenderer->targetFramebuffer->Width - 8) mousePosition.x = GlobalRenderer->targetFramebuffer->Width - 8;
        
        if(mousePosition.y < 0) mousePosition.y = 0;
        if(mousePosition.y  > GlobalRenderer->targetFramebuffer->Height - 16) mousePosition.y = GlobalRenderer->targetFramebuffer->Height - 16;

        GlobalRenderer->putChar('a', mousePosition);

        mousePacketReady = false;

    }

void InitPS2Mouse(){

    outb(0x64, 0xA8); //enable aux device - mouse 
    MouseWait();
    outb(0x64, 0x20); //tells kb controller that we want to send a command to mouse
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10; //check if 2nd bit is set
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status); //setting the correct bit is the compaq status byte 

    MouseWrite(0xF6); //default setting for mouse
    MouseRead();

    MouseWrite(0xF4);

}
