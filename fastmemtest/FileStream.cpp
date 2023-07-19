#include "FileStream.h"
#include "ntdll.h"

System::IO::FileStream::FileStream(const wchar_t* filename, nt::native::ntdll::FileMode mode, FileAccess access, nt::native::ntdll::FileShareMode share)
{
}

void System::IO::FileStream::Close()
{
	nt::native::Ntdll.NtClose(handle);
	handle = 0;
	access = (FileAccess)0;
}

int System::IO::FileStream::Read(char* buffer, size_t length)
{
	if (handle == nullptr || !CanRead())
		return 0;
	using namespace nt::native;
	ntdll::IoStatusBlock isb{};
	if (nt::native::Ntdll.NtReadFile(handle, 0, 0, 0, &isb, buffer, length, 0, 0) == ntdll::NtStatus::Success)
	{
		
	}
	return 0;
}

int System::IO::FileStream::Write(const char* buffer, size_t length)
{
	if (handle == nullptr || !CanWrite())
		return 0;
	using namespace nt::native;
	ntdll::IoStatusBlock isb{};
	if (nt::native::Ntdll.NtWriteFile(handle, 0, 0, 0, &isb, buffer, length, 0, 0) == ntdll::NtStatus::Success)
	{

	}
	return 0;
}

void System::IO::FileStream::Flush()
{
}

long System::IO::FileStream::Seek(long offset, SeekOrigin origin)
{
	return 0;
}
