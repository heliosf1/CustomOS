#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

//frame buffer info
typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine; //FrameBuffer might have more pixels than width, for specific graphics features
}FrameBuffer;

//magic bytes used to identify header
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

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


FrameBuffer framebuffer;
FrameBuffer* InitializeGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop; // pointer to EFI_GOP
	EFI_STATUS status; // fail check

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop); // fix calling convention for UEFI
	if(EFI_ERROR(status))
	{
		Print(L"Unable to locate GOP\n\r");
		return NULL;
	}
	else
	{
		Print(L"GOP located\n\r");
	}

	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
	return &framebuffer;
}

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) //Load File function from filesystem
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage); //efi loaded image protocol

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem); //file system that the OS is booted from

	if(Directory == NULL)
	{
		FileSystem->OpenVolume(FileSystem, &Directory); //if directory is null, directory is root of filesystem
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY); //open directory and save file to LoadedFile
	if(s != EFI_SUCCESS)
	{
		return NULL; //check if open directory exists
	}
	return LoadedFile;
}


PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* Font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if(Font == NULL) return NULL;

	PSF1_HEADER* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontHeader); //allocate memory for PSF1_HEADER
	UINTN size = sizeof(PSF1_HEADER); 
	Font->Read(Font, &size, fontHeader); //read header into memory

	if(fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1) //check if font file is valid
	{
		return NULL;
	}

	UINTN glyphBufferSize = fontHeader->charsize * 256; //buffer size to store glyphs

	if(fontHeader->mode == 1) //  512 glyph mode
	{
		glyphBufferSize = fontHeader->charsize*512;
	}

	void* glyphBuffer; //define glyphBuffer
	{
		Font->SetPosition(Font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);//allocate memory for glyph buffer
		Font->Read(Font, &glyphBufferSize, glyphBuffer); //read glyph data into allocated glyph buffer memory
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont); //allocate memory for font
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	
	return finishedFont;

}

int memcmp(const void* aptr, const void* bptr, size_t n) //checks if elf file is executable and header values are correct
{
	const unsigned char* a = aptr, *b = bptr;
	for(size_t i = 0; i <n; i++) //scan through two buffers of memory for size of n
	{
		if(a[i] < b[i])
		{
			return -1;
		} 
		else if(a[i] > b[i])
		{
			return 1;
		}
	}
		return 0;
}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable); // init uefi environment to recognize special commands
	Print(L" STRIIIING\n\r"); // new line and carriage return

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);

	if( Kernel == NULL) //check if LoadFile works
	{
		Print(L"Could not load kernel\n\r"); 
	}
	else
	{
		Print(L"Kernel Loaded Succesfully\n\r");
	}

	Elf64_Ehdr header;//elf64 header
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL); // Set FileInfoSize to the size required for this kernel
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo); // allocate a pool of memory the size of FileInfoSize
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header); // read no. of bytes in size into header
	}

	if(										// checks
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
	)
	{
		Print(L"Kernel format is bad\n\r");
	}
	else{
		Print(L"Kernel header succesfully verified\n\r");
	}
	Elf64_Phdr* phdrs; //load program info from elf file into memory (phdr = Program header)  [this is a struct]
	{
		Kernel->SetPosition(Kernel, header.e_phoff); //byte offset when reading file
		UINTN size = header.e_phnum * header.e_phentsize; // ph number ( ph entry size)
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);//memory allocation for program header
		Kernel->Read(Kernel, &size, phdrs); // read all bytes of ph and put them into struct
	}

	for(// loop through headers and load all binary info
	Elf64_Phdr* phdr = phdrs;
	(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
	phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)	
	)
	{
		switch(phdr->p_type)
		{
			case PT_LOAD: // allocate pages to load the program on memory
			{
				int pages = (phdr->p_memsz + 0x1000 -1) / 0x1000; //getting memory size and rounding up
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment); //allocate pages according to memory size

				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);
				break;
			}
		}
	}
//call entry of the kernel (kernel.c) 
//define void function pointer at address of header.e_Entry, and giving attribute for the compiler to use correct calling conventions, and returning to this point  	
	void(*KernelStart)(FrameBuffer*, PSF1_FONT*/*Insert info here z.B. memory map or graphics info */ ) = ((__attribute__((sysv_abi)) void (*)(FrameBuffer*, PSF1_FONT*) ) header.e_entry); //entry function pointer
	
	PSF1_FONT* newFont = LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable); //load font file
	if(newFont == NULL) //check if font file is valid
	{
		Print(L"Font is not valid or is not found\n\r");
	}
	else{
		Print(L"Font found, char size = %d\n\r", newFont->psf1_Header->charsize);
	}


	FrameBuffer* newBuffer = InitializeGOP(); // Graphics Output Protocol
	Print(
		L"Base: 0x%x\n\rSize: 0x%x\n\rWidth: %d\n\r	Height: %d\n\rPixelsPerScanLine: %d\n\r\n\r", 
		newBuffer->BaseAddress, 
		newBuffer->BufferSize, 
		newBuffer->Width, 
		newBuffer->Height, 
		newBuffer->PixelsPerScanLine);


    KernelStart(newBuffer, newFont); 

	Print(L"Kernel Loaded \n\r");

	return EFI_SUCCESS;
}