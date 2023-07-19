#include "HashedString.h"
#include <map>
namespace nt::utility
{
	namespace hs
	{
		struct HashedStringRecord
		{
			unsigned long long refcount = 0;
			const char* utf8 = nullptr;
		};
		std::map<StringHash, HashedStringRecord> stringpool;
	}
}
using namespace nt::utility::hs;
void nt::utility::HashedString::_check_hash()
{
	if (Hash != 0 || stringpool[Hash].utf8 == nullptr)
		throw std::exception("Bad hash.");
}

void nt::utility::HashedString::_setup(const char* str) noexcept
{
	if (Hash != 0)
		return;
	Hash = nt::GetHash(str);
	if (stringpool[Hash].utf8 != nullptr)
		return;
	HashedStringRecord rec{ 0 };
	rec.refcount = 1;
	auto len = nt::GetStringLength(str);
	auto clone = new char[len + 1];
	std::copy(str, str + len, clone);
	clone[len] = 0;
	rec.utf8 = clone;
	stringpool[Hash] = rec;

}

void nt::utility::HashedString::_add_ref() noexcept
{
	if (Hash == 0)
		return;
	stringpool[Hash].refcount++;
}

void nt::utility::HashedString::_release() noexcept
{
	if (Hash == 0)
		return;
	stringpool[Hash].refcount--;
	if (stringpool[Hash].refcount <= 0)
	{
		delete[] stringpool[Hash].utf8;
		stringpool[Hash].utf8 = nullptr;
	}
}

const char* nt::utility::HashedString::GetUnicodeString()
{
	if (Hash == 0)
	{
		return (const char*)"";
	}
	if (stringpool[Hash].utf8 == nullptr)
	{
		throw std::exception("Hashstring has released");
	}
	return stringpool[Hash].utf8;
}

unsigned long long nt::utility::HashedString::GetStringPoolCount() noexcept
{
	return stringpool.size();
}

void nt::utility::HashedString::Collect() noexcept
{

}
