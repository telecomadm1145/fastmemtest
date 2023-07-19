#include "ntdll.h"
#pragma warning(disable:4273)
extern "C"
{

	struct __std_exception_data
	{
		char const* _What;
		bool        _DoFree;
	};

	void __std_exception_copy(__std_exception_data const* _From, __std_exception_data* _To)
	{
		_To->_What = _From->_What;
	}

	void __std_exception_destroy(__std_exception_data* _Data)
	{
		// Do nothing.
	}

	__declspec() void _invalid_parameter(
		wchar_t const* const expression,
		wchar_t const* const function_name,
		wchar_t const* const file_name,
		unsigned int   const line_number,
		uintptr_t const reserved)
	{

	}

	__declspec() void _invalid_parameter_noinfo(void)
	{

	}

	__declspec(noreturn) void _invalid_parameter_noinfo_noreturn(void)
	{

	}

	__declspec(noreturn) void _invoke_watson(
		wchar_t const* const expression,
		wchar_t const* const function_name,
		wchar_t const* const file_name,
		unsigned int   const line_number,
		uintptr_t       const reserved)
	{

	}
	int _CrtDbgReport(int reportType,
		const char* filename,
		int linenumber,
		const char* moduleName,
		const char* format, ...)
	{
		return 0;
	}
	int _CrtDbgReportW(int reportType,
		const wchar_t* filename,
		int linenumber,
		const wchar_t* moduleName,
		const wchar_t* format, ...)
	{
		return 0;
	}

	constexpr auto EH_EXCEPTION_NUMBER = ('msc' | 0xE0000000); // The NT Exception # that we use;
	// As magic numbers increase, we have to keep track of the versions that we are
	// backwards compatible with.  The top 3 bits of the magic number DWORD are
	// used for other purposes, so while the magic number started as a YYYYMMDD
	// date, it can't represent the year 2000 or later.  Old CRTs also recognize
	// unknown magic numbers with a >= test.  Therefore, we just increment the
	// the magic number by one every time we need another.
	//
	// EH_MAGIC_NUMBER1     The original, still used in exception records for
	//                      native or mixed C++ thrown objects.
	// EH_MAGIC_NUMBER2     Used in the FuncInfo if exception specifications are
	//                      supported and FuncInfo::pESTypeList is valid.
	// EH_MAGIC_NUMBER3     Used in the FuncInfo if FuncInfo::EHFlags is valid, so
	//                      we can check if the function was compiled -EHs or -EHa.
	constexpr auto EH_MAGIC_NUMBER1 = 0x19930520;
	constexpr auto EH_MAGIC_NUMBER2 = 0x19930521;
	constexpr auto EH_MAGIC_NUMBER3 = 0x19930522;

	// please ignore the "_ThrowInfo*" error.
	void __stdcall _CxxThrowException(void* pExceptionObject, _ThrowInfo* pThrowInfo)
	{
		using namespace nt::native::ntdll;
		auto& er = *new ExceptionRecord();
		er.ExceptionCode = EH_EXCEPTION_NUMBER;
		er.ExceptionFlags = 1; // EXCEPTION_NONCONTINUABLE
		void* parameters[] = {
	(void*)EH_MAGIC_NUMBER1,
	pExceptionObject,
	(void*)pThrowInfo,
		};
		er.ExceptionAddress = _CxxThrowException;
		er.NumberParameters = _countof(parameters);
		nt::utility::Copy<void**, void**>(parameters, &er.ExceptionInformation[0], _countof(parameters));
		Ntdll::Instance.RtlRaiseException(&er);
		delete& er;
	}
}