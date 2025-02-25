#include "cstr.h"

char uintTo_StringOutput[128];
const char* toString(uint64_t value){
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++; //calculate number size
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10; //get LSD(Least Significant Digit)
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0'; //put LSD in array last position
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_StringOutput[size - index] = remainder + '0'; //insert number value by adding it with ASCII '0'
    uintTo_StringOutput[size + 1] = 0;  //put line termination ASCII character at endposition
    return uintTo_StringOutput;
}

char intTo_StringOutput[128];
const char* toString(int64_t value){
    uint8_t isNegative = 0; 

    if(value < 0) 
    {
        isNegative = 1; 
        value *= -1; //turn value positive
        intTo_StringOutput[0] = '-'; //if negative put '-' in the first position of the array
    }

    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10; 
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        intTo_StringOutput[isNegative + size - index] = remainder + '0'; //add isNegative to array index to calculate correct digit placement
        index++;
    }
    uint8_t remainder = value % 10;
    intTo_StringOutput[isNegative + size - index] = remainder + '0';
    intTo_StringOutput[isNegative + size + 1] = 0; 
    return intTo_StringOutput;
}

char doubleTo_StringOutput[128];
const char* toString(double value, uint8_t decimalPrecision){
    
    if(decimalPrecision > 20) 
    { 
        decimalPrecision = 20; // failsafe if double precision is surpassed
    }

    char* intPtr = (char*)toString((int64_t)value); //ptr for whole number
    char* doublePtr = doubleTo_StringOutput; //ptr for full value of number

    if(value < 0)   //check if number is negative
    {
        value *= -1; 
    }

    while(*intPtr != 0)
    {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;  
        //while intPtr ! 0, insert digits of whole number into doublePtr
        //in this case the value 0 is the ASCII line termination value
    }

    *doublePtr = '.'; 
    doublePtr++; 

    double newValue = value - (int)value; //get decimal places
    for(uint8_t i = 0; i < decimalPrecision; i++) 
    {
        newValue *= 10; 
        *doublePtr = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePtr++;
    }

    *doublePtr = 0;
    return doubleTo_StringOutput;
}

const char* toString(double value)
{
    return toString(value, 2);
}

char hexTo_StringOutput64[128];
const char* toHexString(uint64_t value)
{
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = sizeof(uint64_t) * 2 - 1; //max size for full hex representation; 32bit hex is 8 digits
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)valPtr + i); //ptr for every 2 hex digits (4bits/digit); increment after mapping every digit
        temp = ((*ptr & 0xF0) >> 4); //get left side of number, bitshift by 4
        hexTo_StringOutput64[size - (i * 2 + 1)/*add to first position of 8bit chunk*/] = temp + (temp > 9 ? 55 : '0'); // add to array, check if it is a digit or a character
        temp = ((*ptr & 0x0F)); //get right side of number
        hexTo_StringOutput64[size - (i * 2)/*add to last position of 8bit chunk*/] =  temp + (temp > 9 ? 55 : '0'); // add to array, check if it is a digit or a character
    }
    
    hexTo_StringOutput64[size + 1] = 0; //line termination ASCII character
    
    return hexTo_StringOutput64; 
}

char hexTo_StringOutput32[128];
const char* toHexString(uint32_t value)
{
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = sizeof(uint32_t) * 2 - 1; //max size for full hex representation; 32bit hex is 8 digits
    for(uint8_t i = 0; i < size / 2 + 1; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        temp = ((*ptr & 0xF0) >> 4); //get left side of number, bitshift by 4
        hexTo_StringOutput32[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0'); // add to array, check if it is a digit or a character
        temp = ((*ptr & 0x0F)); //get right side of number
        hexTo_StringOutput32[size - (i * 2)] =  temp + (temp > 9 ? 55 : '0');
    }
    
    hexTo_StringOutput32[size + 1] = 0; //line termination ASCII character
    
    return hexTo_StringOutput32; 
}

char hexTo_StringOutput16[128];
const char* toHexString(uint16_t value)
{
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = sizeof(uint16_t) * 2 - 1; //max size for full hex representation; 32bit hex is 8 digits
    for(uint8_t i = 0; i < size / 2 + 1; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        temp = ((*ptr & 0xF0) >> 4); //get left side of number, bitshift by 4
        hexTo_StringOutput16[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0'); // add to array, check if it is a digit or a character
        temp = ((*ptr & 0x0F)); //get right side of number
        hexTo_StringOutput16[size - (i * 2)] =  temp + (temp > 9 ? 55 : '0');
    }
    
    hexTo_StringOutput16[size + 1] = 0; //line termination ASCII character
    
    return hexTo_StringOutput16; 
}

char hexTo_StringOutput8[128];
const char* toHexString(uint8_t value)
{
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = sizeof(uint8_t) * 2 - 1; //max size for full hex representation; 32bit hex is 8 digits
    for(uint8_t i = 0; i < size / 2 + 1; i++)
    {
        ptr = ((uint8_t*)valPtr + i);
        temp = ((*ptr & 0xF0) >> 4); //get left side of number, bitshift by 4
        hexTo_StringOutput8[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : '0'); // add to array, check if it is a digit or a character
        temp = ((*ptr & 0x0F)); //get right side of number
        hexTo_StringOutput8[size - (i * 2)] =  temp + (temp > 9 ? 55 : '0');
    }
    
    hexTo_StringOutput8[size + 1] = 0; //line termination ASCII character
    
    return hexTo_StringOutput8; 
}