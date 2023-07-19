#pragma once
#include "FileStream.h"
namespace System
{
	class Console
	{
		static IO::Stream* pStdOut;
		static IO::Stream* pStdIn;
		static IO::Stream* pStdErr;
	public:
		static void Load();
		static IO::Stream& Out()
		{
			return *pStdOut;
		}
		static IO::Stream& In()
		{
			return *pStdIn;
		}
		static IO::Stream& Err()
		{
			return *pStdErr;
		}
		static void Write(const char* str)
		{
			Out().Write(str,nt::utility::GetStringLength(str));
		}
		static void WriteLine(const char* str)
		{
			Out().Write(str, nt::utility::GetStringLength(str));
			char crlf[] = "\n";
			Out().Write(crlf, sizeof(crlf));
		}
		static void ReadLine(char* str, size_t sz)
		{
			In().Read(str, sz);
		}
		static void ReadLine()
		{
			char _[1024];
			In().Read(_, 1024);
		}
	};
}
