#pragma once
#include <array>

namespace nt::utility
{
	using Crc64 = unsigned __int64;
	using Crc64Entry = Crc64;
	constexpr auto Crc64Fill = 0x95AC9329AC4BC9B5ULL;
	constexpr auto Crc64Init = 0xFFFFFFFFFFFFFFFFULL;
	constexpr inline auto GenetareCrc64TableCT()
	{
		std::array<Crc64Entry, 256> blocks{};
		for (int i = 0; i < 256; ++i)
		{
			Crc64Entry crc = i;
			for (int j = 0; j < 8; ++j)
				if (crc & 1)
					crc = (crc >> 1) ^ Crc64Fill;
				else
					crc >>= 1;
			blocks.at(i) = crc;
		}
		return blocks;
	}
	constexpr inline Crc64 GetCrc64CT(const char* data, size_t s)
	{
		Crc64 res = Crc64Init;
		constexpr auto Crc64Table = GenetareCrc64TableCT();
		for (size_t i = 0; i < s; i++)
			res = Crc64Table[(res ^ data[i]) & 0xff] ^ (res >> 8);
		return res;
	}
	constexpr inline Crc64 GetCrc64CT(const char* str)
	{
		return GetCrc64CT(str, GetStringLength(str));
	}
	template<typename Array>
	void FillCrc64Table(Array blocks)
	{
		for (int i = 0; i < 256; ++i)
		{
			Crc64Entry crc = i;
			for (int j = 0; j < 8; ++j)
				if (crc & 1)
					crc = (crc >> 1) ^ Crc64Fill;
				else
					crc >>= 1;
			blocks[i] = crc;
		}
	}
	inline Crc64 GetCrc64(const char* data, size_t s)
	{
		Crc64 res = Crc64Init;
		static Crc64Entry* Crc64Table = 0;
		if (Crc64Table == 0)
		{
			Crc64Table = new Crc64Entry[256];
			FillCrc64Table(Crc64Table);
		}
		for (size_t i = 0; i < s; i++)
			res = Crc64Table[(res ^ data[i]) & 0xff] ^ (res >> 8);
		return res;
	}
	inline Crc64 GetCrc64(const char* str)
	{
		return GetCrc64(str, GetStringLength(str));
	}
}