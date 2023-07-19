#pragma once
#include "Defines.h"
#include "Importing.h"
#include "ntdll.h"

namespace nt::native
{
	class Heap
	{
	private:
		void* hheap = 0;
		Heap(void* ptr)
		{
			hheap = ptr;
		}
		static inline Heap GetCurrent()
		{
			return Heap(((Peb*)NtCurrentPeb())->GetProcessHeap());
		}
	public:
		static inline Heap FromHandle(void* ptr)
		{
			return Heap(ptr);
		}
		void* Alloc(size_t s)
		{
			return Ntdll.RtlAllocateHeap(hheap, nt::native::ntdll::HeapFlags::Default, s);
		}
		void Free(void* b)
		{
			Ntdll.RtlFreeHeap(hheap, nt::native::ntdll::HeapFlags::GenerateExceptions, b);
		}
		static Heap Instance;
	};
	constexpr Heap& DefaultHeap = Heap::Instance;
}