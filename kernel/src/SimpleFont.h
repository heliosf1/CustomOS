#pragma once

struct PSF1_HEADER
{
	unsigned char magic[2]; //bytes that header stores, to recognize/identify a psf file
	unsigned char mode; // mode of psf font
	unsigned char charsize;
};

struct PSF1_FONT
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer; //pointer to array of glyphs/bitmaps inside font file

};