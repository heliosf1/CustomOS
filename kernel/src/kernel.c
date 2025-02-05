typedef unsigned long long size_t;

//frame buffer info
typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine; //FrameBuffer might have more pixels than width, for specific graphics features
}FrameBuffer;

typedef struct 
{
	unsigned char magic[2]; //bytes that header stores, to recognize/identify a psf file
	unsigned char mode; // mode of psf font
	unsigned char charsize;
} PSF1_HEADER;

typedef struct 
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer; //pointer to array of glyphs/bitmaps inside font file

}PSF1_FONT;

typedef struct{
	unsigned int x;
	unsigned int y;
}Point;

void putChar(FrameBuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int colour, char chr, Point offset)
{
    unsigned int* pixPtr = (unsigned int*)framebuffer->BaseAddress; //ptr to baseAddress
    char* fontPtr = psf1_font->glyphBuffer + (chr * psf1_font->psf1_Header->charsize); //ptr to glyph inserted by user 
	for(unsigned long y = offset.y; y < offset.y + 16; y++) //select y-axis bitmap coordinate (16bit wide)
	{
		for(unsigned long x = offset.x; x < offset.x + 8; x++) //select x-axis bitmap coordinate (8bit wide)
		{
			if((*fontPtr & (0b10000000 >> (x - offset.x))) > 0 ) // bitshift a single bit to the right by x - xOff, check if it is ON with bitwise AND
			{
				*(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = colour; //assign colour to bit
			}
		}
		fontPtr++;
	}
}
Point CursorPosition;
void Print(FrameBuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int colour, char* str)
{
	char* chr = str;
	while(*chr != 0)
	{
		putChar(framebuffer, psf1_font, colour, *chr, CursorPosition);
		CursorPosition.x += 8;
		if(CursorPosition.x + 8 > framebuffer->Width) //check if next char would overflow out of screen
		{
			CursorPosition.x = 0; //reset x pos
			CursorPosition.y +=16; // go down 1 char length
		}
		chr++;
	}
}

void _start(FrameBuffer* framebuffer, PSF1_FONT* psf1_font)
{	
	CursorPosition.x = 50;
	CursorPosition.y = 120;
	for(int t = 0; t < 50; t+=1){
    	Print(framebuffer, psf1_font, 0xffffffff, "LALLALALALLAAA I CANT HEAR YOUUUU");
	}
	return ;
}
