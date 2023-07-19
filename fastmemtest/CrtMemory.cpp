#include "Heap.h"
namespace nt::internal_func::heap
{
	using namespace nt::native;
	void* AllocStub(size_t s)
	{
		return DefaultHeap.Alloc(s);
	}
	void FreeStub(void* b)
	{
		return DefaultHeap.Free(b);
	}
}

using namespace nt::internal_func::heap;

void* operator new[](size_t s)
{
	return AllocStub(s);
}
void* operator new(size_t s)
{
	return AllocStub(s);
}
void operator delete[](void* b)
{
	return FreeStub(b);
}
void operator delete(void* b)
{
	return FreeStub(b);
}
void operator delete(void* b, size_t)
{
	return FreeStub(b);
}
extern "C"
{
	void* memmove(void* dest, const void* src, size_t count);
	void* memset(void* dest, int c, size_t count);
	void* memcpy(void* dest, const void* src, size_t count);
#pragma function(memmove,memset,memcpy)
	void* memcpy(void* dest, const void* src, size_t count)
	{
		nt::utility::FastDataCopy(src, dest, count);
		return dest;
	}
	void* memmove(void* dest, const void* src, size_t count)
	{
		nt::utility::FastDataCopy(src, dest, count);
		return dest;
	}
	void* memset(void* dest,int c,size_t count)
	{
		nt::utility::Clear((byte*)dest, count, (byte)c);
		return dest;
	}
}
