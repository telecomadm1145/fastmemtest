#pragma once
#include "peb.h"
#include "ntpeb.h"
#include "PE.h"
#include "Array.h"
#include "Hash.h"
namespace nt::native
{
	using ModuleHandle = void*;
	template<class String>
	ModuleHandle GetModuleHandle(const String name)
	{
		auto peb = (Peb*)NtCurrentPeb();
		if (name == 0)
			return GetModuleHandle(peb->ProcessParameters->ImagePathName.Buffer);
		auto begin = peb->Ldr->InMemoryOrderModuleList.InMemoryOrderLinks.Blink;
		for (NtDoubleLinkedList* mod = begin; mod->Blink != begin; mod = mod->Blink)
		{
			auto ldr = (PebLoaderList*)&mod[-1];
			if (nt::utility::IsEndsWithNoCase(ldr->FullDllName.Buffer, name))
			{
				return ldr->DllBase;
			}
			if (ldr->FullDllName.MaximumLength == 0)
				break;
		}
		return 0;
	}
	template<typename Callback>
	void EnumDllProcs(ModuleHandle module, Callback c)
	{
		auto dih = (DosImageHeader*)module;
		auto& pe = dih->GetPeHeader();
		auto& oh = pe.GetOptionalHeader<DosImageHeader::PeHeader::OptionalHeader>();
		auto& dd = oh.GetExportDirectoryMemory();
		auto names = dd.Names((void*)oh.ImageBase);
		auto ordinals = dd.NameOrdinals((void*)oh.ImageBase);
		auto funcs = dd.Functions((void*)oh.ImageBase);
		auto funcorder = 0;
		for (auto namerva : names)
		{
			auto funcname = (char*)oh.ImageBase + namerva;
			auto func = (void*)((byte*)oh.ImageBase + funcs[ordinals[funcorder]]);
			if (!c(funcname, func))
			{
				break;
			}
			funcorder++;
		}
	}
	using ProcAddr = void*;
	inline ProcAddr GetProcAddressByCrc(ModuleHandle module, nt::utility::Crc64 name)
	{
		void* procaddress = 0;
		EnumDllProcs(module, [&procaddress, name](char* fname, void* func)-> bool{
				if (nt::utility::GetCrc64(fname) == name)
				{
					procaddress = func;
					return false;
				}
				else // just for better format
					return true;
			});
		return procaddress;
	}
	template<class String>
	ProcAddr GetProcAddress(ModuleHandle module, const String name)
	{
		void* procaddress = 0;
		EnumDllProcs(module, [&procaddress, name](char* fname, void* func)-> bool{
				if (nt::utility::IsSameStringNoCase(name, fname))
				{
					procaddress = func;
					return false;
				}
				else
					return true;
			});
		return procaddress;
	}
}