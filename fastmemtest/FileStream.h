#pragma once
#include "Stream.h"
#include "Ntdll.h"
namespace System::IO
{
	enum class FileAccess
	{
		Read = 1,
		Write = 2,
		ReadWrite = 3,
	};
	class FileStream : public Stream
	{
	private:
		void* handle = 0;
		FileAccess access = FileAccess::Read;
	public:
		FileStream(void* handle, FileAccess access)
		{
			this->handle = handle;
			this->access = access;
		}
		FileStream(const wchar_t* filename, nt::native::ntdll::FileMode mode, FileAccess access = FileAccess::Read,nt::native::ntdll::FileShareMode share = nt::native::ntdll::FileShareMode::None);
		bool CanRead() override 
		{
			return ((int)access & 1) == 1;
		}
		bool CanWrite() override
		{
			return ((int)access & 2) == 2;
		}
		bool CanSeek() override
		{
			return true;
		}
		virtual void Close() override;
		virtual int Read(char* buffer, size_t length) override;
		virtual int Write(const char* buffer, size_t length) override;
		virtual void Flush() override;
		virtual long Seek(long offset, SeekOrigin origin) override;
	};
}