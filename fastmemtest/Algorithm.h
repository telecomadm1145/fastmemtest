#pragma once
namespace nt
{
	using StringHash = unsigned long long;
	StringHash _get_crc(const unsigned char* buf, size_t len);
	template<class CharT,class TSize = size_t>
	TSize GetStringLength(const CharT* str)
	{
		TSize counter = 0;
		while (*(str++) != 0)
			counter++;
		return counter;
	}
	template<class CharT>
	StringHash GetHash(const CharT* str)
	{
		return _get_crc((const unsigned char*)str, GetStringLength(str));
	}
}