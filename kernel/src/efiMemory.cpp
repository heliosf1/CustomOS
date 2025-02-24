#include "efiMemory.h"

const char* EFI_MEMORY_TYPE_STRINGS[]
{
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory", //Memory not used by UEFI
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory", //Reclaimable memory after gathering info from ACPI tables
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode"
};