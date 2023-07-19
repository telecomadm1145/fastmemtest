#include "App.h"
#include "Console.h"
#include "String.h"
#include "PowRandom.h"

class MemTestApp : public nt::App
{
	ulonglong GetMem()
	{
		using namespace nt::native;
		auto mod = GetModuleHandle("kernel32.dll");
		using proc_type = bool(ulonglong*);
		static constexpr auto crc64 = nt::utility::GetCrc64CT("GetPhysicallyInstalledSystemMemory");
		auto proc = (proc_type*)GetProcAddressByCrc(mod,crc64);
		ulonglong mem = 0;
		if (proc(&mem))
		{
			return mem;
		}
		return 0;
	}
	// 通过 App 继承
	virtual void Startup() override
	{
		using namespace nt::native;
		using namespace System;
		Console::WriteLine("内存速测 by github.com/telecomadm1145");
		PowRandom pero;
		Console::Write("系统内存大小:");
		auto len = GetMem();
		char buf[30];
		nt::utility::ToString(len / 1024, buf, 30);
		Console::Write(buf);
		Console::WriteLine("mb.");
		ulonglong totalerrors = 0;
		int times = 0;
		while (true)
		{
			Console::Write("正在运行第");
			nt::utility::ToString(times, buf, 30);
			Console::Write(buf);
			Console::WriteLine("次测试.");
			void* pbase = (void*)0;
			size_t regionsize = len * 1024 * 0.7;
			auto err = Ntdll.NtAllocateVirtualMemory((void*)-1ULL, &pbase, 0, &regionsize, 0x00001000, 0x04);
			if (err != ntdll::NtStatus::Success)
			{
				Console::WriteLine("分配遇到异常，请联系Devs.");
				Console::ReadLine();
				exit(0);
				return;
			}
			Console::WriteLine("正在用随机数填充中");
			char* pcur = (char*)pbase;
			while (pcur < (char*)pbase + regionsize)
			{
				for (size_t i = 0; i < 1016; i++)
				{
					pcur[i] = pero.Next() & 0xff;
				}
				auto crc = nt::utility::GetCrc64(pcur, 1016);
				if ((crc & 0xffff) == 0)
				{
					Console::Write("进度:");
					auto prog = int(((double)(pcur - pbase) / regionsize) * 100);
					nt::utility::ToString(prog, buf, 30);
					Console::Write(buf);
					Console::WriteLine("%.");
				}
				*(ulonglong*)(pcur + 1016) = crc;
				pcur += 1024;
			}
			Console::WriteLine("填充完成，开始检查");
			ulonglong errors = 0;
			pcur = (char*)pbase;
			while (pcur < (char*)pbase + regionsize)
			{
				auto crc = nt::utility::GetCrc64(pcur, 1016);
				if ((crc & 0xffff) == 0)
				{
					Console::Write("进度:");
					auto prog = int(((double)(pcur - pbase) / regionsize) * 100);
					nt::utility::ToString(prog, buf, 30);
					Console::Write(buf);
					Console::WriteLine("%.");
				}
				if (*(ulonglong*)(pcur + 1016) != crc)
				{
					errors++;
					totalerrors++;
				}
				pcur += 1024;
			}
			Console::Write("本次测试中找到了");
			nt::utility::ToString(errors, buf, 30);
			Console::Write(buf);
			Console::Write("个错误,总共");
			nt::utility::ToString(totalerrors, buf, 30);
			Console::Write(buf);
			Console::WriteLine("个错误.");
			err = Ntdll.NtFreeVirtualMemory((void*)-1ULL, &pbase, &regionsize, 0x00008000);
			if (err != ntdll::NtStatus::Success)
			{
				Console::WriteLine("释放遇到异常，请联系Devs.");
				Console::ReadLine();
				exit(0);
				return;
			}
			//Console::WriteLine("按任意键继续下一轮测试.");
			//Console::ReadLine();
			times++;
		}
		exit(0);
	}
};

void nt::App::Create(App*& rout)
{
	rout = new MemTestApp();
}
