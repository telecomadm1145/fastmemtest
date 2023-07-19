#pragma once
#include "Defines.h"
#include "Array.h"

namespace nt::native
{
	using RVA = uint;
	struct DosImageHeader {				   // DOS .EXE header
		char   DosSig[2] = { 'M','Z' };             // Magic number
		short   Bytes;                     // Bytes on last page of file
		short   Pages;                     // Pages in file
		short   Relocation;                // Relocations
		short   HeaderSize;                // Size of header in paragraphs
		short   MinAlloc;                  // Minimum extra paragraphs needed
		short   MaxAlloc;                  // Maximum extra paragraphs needed
		short   Ss;                        // Initial (relative) SS value
		short   Sp;                        // Initial SP value
		short   Checksum;                  // Checksum
		short   IpReg;                     // Initial IP value
		short   CsReg;                     // Initial (relative) CS value
		short   RelocationTable;           // File address of relocation table
		short   Overlay;                   // Overlay number
		short   Reserved[4];               // Reserved words
		short   OemID;                     // OEM identifier (for e_oeminfo)
		short   OemInfo;                   // OEM information; e_oemid specific
		short   Reserved2[10];             // Reserved words
		long    PeAddress;                 // File address of new exe header
		struct SectionHeader
		{
			char    Name[8];
			uint   PhysicalAddress;
			uint   VirtualAddress;
			uint   SizeOfRawData;
			uint   PointerToRawData;
			uint   PointerToRelocations;
			uint   PointerToLinenumbers;
			byte    NumberOfRelocations;
			byte    NumberOfLinenumbers;
			uint   Characteristics;
		};
		struct PeHeader
		{
			struct ImageHeader {
				enum class MachineType : ushort
				{
					Unknown = 0,
					TargetHost = 1,
					I386 = 0x14c,
					IA64 = 0x200,
					Arm = 0x1c0,
					Thumb = 0x1c2,
					AMD64 = 0x8664,
					Arm64 = 0xAA64,
				} Machine;						// Define which Cpu the pe runs.
				ushort    NumberOfSections;		// The Number Of Sections in Pe file.
				uint      TimeDateStamp;			// The timestamp when compiler created this pe file(Unix time)
				RVA      PointerToSymbolTable;	// For obj file.
				uint      NumberOfSymbols;		// For obj file.
				ushort    SizeOfOptionalHeader;	// The size of optional header.
				enum class ImageAttributes : short
				{
					NoRelocs = 0x1,
					Executable = 0x2,
					NoLineNums = 0x4,
					NoLocalSyms = 0x8,
					AggresiveWsTrim = 0x10,
					LargeAddressAware = 0x20,
					X86Machine = 0x100,
					CannotDebug = 0x400,
					RunFromRemovableAsSwap = 0x400,
					RunFromNetworkAsSwap = 0x800,
					SystemFile = 0x1000,
					DllFile = 0x2000,
					UpSystemOnly = 0x4000,
				} Attributes;

				Uncopyable(ImageHeader);
			};
			struct ImageDataDirectory {
				RVA   VirtualAddress;
				uint   Size;
			};
			struct ImageExportDirectory {
				uint   Characteristics;
				uint   TimeDateStamp;
				ushort    MajorVersion;
				ushort    MinorVersion;
				RVA   Name;
				uint   Base;				  // Begin of export table
				uint   NumberOfFunctions;
				uint   NumberOfNames;
				RVA   AddressOfFunctions;     // RVA from base of image
				RVA   AddressOfNames;         // RVA from base of image
				RVA   AddressOfNameOrdinals;  // RVA from base of image
				auto Functions(void* ImageBase)
				{
					auto a = (RVA*)((char*)ImageBase + AddressOfFunctions);
					return nt::utility::EnumProxy(a,a+NumberOfFunctions);
				}
				auto Names(void* ImageBase)
				{
					auto a = (RVA*)((char*)ImageBase + AddressOfNames);
					return nt::utility::EnumProxy(a, a + NumberOfNames);
				}
				auto NameOrdinals(void* ImageBase)
				{
					auto a = (ushort*)((char*)ImageBase + AddressOfNameOrdinals);
					return nt::utility::EnumProxy(a, a + NumberOfNames);
				}
			};
			struct OptionalHeader64
			{
				enum Type : ushort
				{
					X86 = 0x10b,
					X64 = 0x20b,
					ROM = 0x107,
				}Magic;
				byte    MajorLinkerVersion;
				byte    MinorLinkerVersion;
				uint   SizeOfCode;
				uint   SizeOfInitializedData;
				uint   SizeOfUninitializedData;
				RVA   AddressOfEntryPoint;
				RVA   BaseOfCode;
				ulonglong   ImageBase;
				uint       SectionAlignment;
				uint       FileAlignment;
				ushort        MajorOperatingSystemVersion;
				ushort        MinorOperatingSystemVersion;
				ushort        MajorImageVersion;
				ushort        MinorImageVersion;
				ushort        MajorSubsystemVersion;
				ushort        MinorSubsystemVersion;
				uint       Win32VersionValue;
				uint       SizeOfImage;
				uint       SizeOfHeaders;
				uint       CheckSum;
				ushort        Subsystem;
				ushort        DllCharacteristics;
				ulonglong   SizeOfStackReserve;
				ulonglong   SizeOfStackCommit;
				ulonglong   SizeOfHeapReserve;
				ulonglong   SizeOfHeapCommit;
				uint       LoaderFlags;
				uint       NumberOfRvaAndSizes;
				ImageDataDirectory DataDirectory[16];
#ifdef _WIN64
				ImageExportDirectory& GetExportDirectoryMemory()
				{
					return *(ImageExportDirectory*)(((char*)ImageBase) + DataDirectory[0].VirtualAddress);
				}
#endif
			};
			struct OptionalHeader32
			{
				enum Type : ushort
				{
					X86 = 0x10b,
					X64 = 0x20b,
					ROM = 0x107,
				}Magic;
				byte    MajorLinkerVersion;
				byte    MinorLinkerVersion;
				uint   SizeOfCode;
				uint   SizeOfInitializedData;
				uint   SizeOfUninitializedData;
				RVA   AddressOfEntryPoint;
				RVA   BaseOfCode;
				RVA   BaseOfData;
				uint   ImageBase;
				uint   SectionAlignment;
				uint   FileAlignment;
				ushort    MajorOperatingSystemVersion;
				ushort    MinorOperatingSystemVersion;
				ushort    MajorImageVersion;
				ushort    MinorImageVersion;
				ushort    MajorSubsystemVersion;
				ushort    MinorSubsystemVersion;
				uint   Win32VersionValue;
				uint   SizeOfImage;
				uint   SizeOfHeaders;
				uint   CheckSum;
				ushort    Subsystem;
				ushort    DllCharacteristics;
				uint   SizeOfStackReserve;
				uint   SizeOfStackCommit;
				uint   SizeOfHeapReserve;
				uint   SizeOfHeapCommit;
				uint   LoaderFlags;
				uint   NumberOfRvaAndSizes;
				ImageDataDirectory DataDirectory[16];
#ifndef _WIN64
				ImageExportDirectory& GetExportDirectoryMemory()
				{
					return *(ImageExportDirectory*)(((char*)ImageBase) + DataDirectory[0].VirtualAddress);
				}
#endif
			};
#ifdef _WIN64
			using OptionalHeader = OptionalHeader64;
#else
			using OptionalHeader = OptionalHeader32;
#endif // _WIN64

			char Signature[4] = "PE\0";
			ImageHeader FileHeader;
			template<typename T>
			T& GetOptionalHeader()
			{
				return *((T*)&this->optional_header);
			}
			SectionHeader* GetSections()
			{
				return (SectionHeader*)((char*)&this->optional_header + this->FileHeader.SizeOfOptionalHeader);
			}
		private:
			byte optional_header;

			Uncopyable(PeHeader);
		};
		PeHeader& GetPeHeader()
		{
			return *((PeHeader*)((char*)this + PeAddress));
		}

		Uncopyable(DosImageHeader);
	};
}