#pragma once

#include "Dll.h"
#include <type_traits>

#define DeclareWinApi(f) private: constexpr static auto crc_##f = nt::utility::GetCrc64CT(#f);\
public:decltype(f)* f = _dll[crc_##f]
#define DeclareWinApi2(f) public:decltype(f)* f = _dll[#f]

namespace nt::utility
{
	class ProcPtr {
	public:
		explicit ProcPtr(void* ptr) : _ptr(ptr) {}

		template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
		operator T* () const {
			return reinterpret_cast<T*>(_ptr);
		}

	private:
		void* _ptr;
	};

	class DllHelper {
	private:
		void* _module = 0;

	public:

		explicit DllHelper(const char* dllname) {
			_module = nt::native::GetModuleHandle(dllname);
		}

		~DllHelper() {}
		ProcPtr operator[](const char* proc_name) const {
			return ProcPtr(nt::native::GetProcAddress(_module, proc_name));
		}
		ProcPtr operator[](Crc64 proc_name_crc) const {
			return ProcPtr(nt::native::GetProcAddressByCrc(_module, proc_name_crc));
		}
	};
	class ImportingDll {
	protected:
		ImportingDll() {};
		Uncopyable(ImportingDll);
	};
}