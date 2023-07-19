#include "Array.h"
#include "ntdll.h"
#include <exception>
extern "C"
{
	void __std_terminate()
	{
		exit(-1);
	}
	int __cdecl _purecall()
	{
		throw std::exception("Called an virtual function.");
	}
}

class type_info
{
public:
	virtual ~type_info() noexcept;
};
type_info::~type_info() noexcept
{
}

namespace std
{
	void _Xlength_error(const char* msg)
	{
		throw std::exception(msg);
	}
}