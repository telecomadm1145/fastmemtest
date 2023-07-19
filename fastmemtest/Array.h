#pragma once
#include <concepts>
#include "defines.h"
namespace nt::utility
{
	template<typename CharT>
	constexpr inline CharT LowerCase(CharT c)
	{
		return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c;
	}
	template<typename CharT>
	constexpr inline CharT UpperCase(CharT c)
	{
		return c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
	}
	template<typename String>
	constexpr inline auto GetStringLength(String str)
	{
		size_t i = 0;
		auto cstr = str;
		while (*cstr++ != 0)
			i++;
		return i;
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsSame(Iter1 begin, Iter1 end, Iter2 bbegin)
	{
		auto bi = bbegin;
		for (auto i = begin; i != end; i++)
		{
			if (*i != *bi)
				return false;
			bi++;
		}
		return true;
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsSameString(Iter1 begin, Iter2 bbegin)
	{
		return IsSameString(begin, bbegin, [](auto a, auto b) {return a == b; });
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsSameStringNoCase(Iter1 begin, Iter2 bbegin)
	{
		return IsSameString(begin, bbegin, [](auto a, auto b) {return a == b || LowerCase(a) == LowerCase(b); });
	}
	template<typename Iter1, typename Iter2, typename Func>
	constexpr inline bool IsSameString(Iter1 begin, Iter2 bbegin, Func f)
	{
		auto i = begin;
		auto bi = bbegin;
		while (*i || *bi)
		{
			if (!f(*i++, *bi++))
				return false;
		}
		return *i == 0 && *bi == 0;
	}
	template<typename Iter>
	constexpr inline Iter GetStringEnd(Iter begin)
	{
		Iter i = begin;
		while (*i++ != 0) {}
		return i;
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsStartsWith(Iter1 begin, Iter2 bbegin)
	{
		return IsStartsWith(begin, bbegin, [](auto a, auto b) {return a == b; });
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsStartsWithNoCase(Iter1 begin, Iter2 bbegin)
	{
		return IsStartsWith(begin, bbegin, [](auto a, auto b) {return a == b || LowerCase(a) == LowerCase(b); });
	}
	template<typename Iter1, typename Iter2, typename Func>
	constexpr inline bool IsStartsWith(Iter1 begin, Iter2 bbegin, Func f)
	{
		auto i = begin;
		auto bi = bbegin;
		do
		{
			if (!f(*i, *bi))
				return *bi == 0;
		} while (*i++ && *bi++);
		return true;
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsEndsWith(Iter1 begin, Iter2 bbegin)
	{
		return IsEndsWith(begin, bbegin, [](auto a, auto b) {return a == b; });
	}
	template<typename Iter1, typename Iter2>
	constexpr inline bool IsEndsWithNoCase(Iter1 begin, Iter2 bbegin)
	{
		return IsEndsWith(begin, bbegin, [](auto a, auto b) {return a == b || LowerCase(a) == LowerCase(b); });
	}
	template<typename Iter1, typename Iter2, typename Func>
	constexpr inline bool IsEndsWith(Iter1 begin, Iter2 bbegin, Func f)
	{
		if (begin == nullptr)
			return false;
		if (bbegin == nullptr)
			return false;
		auto i = begin;
		auto bi = bbegin;
		do
		{
			if (f(*i, *bi))
			{
				bi++;
			}
		} while (*i++ && *bi);
		return *bi == 0;
	}
	template<typename Iter1, typename Iter2>
	constexpr inline void Copy(Iter1 begin, Iter1 end, Iter2 bbegin)
	{
		auto i = begin;
		auto bi = bbegin;
		while (i != end)
		{
			*bi = *i;
			i++;
			bi++;
		}
	}
	template<typename Iter1, typename Iter2>
	constexpr inline void Copy(Iter1 begin, Iter2 bbegin, size_t count)
	{
		return Copy(begin, begin + count, bbegin);
	}
	template<typename Iter1, typename T>
	constexpr inline void Clear(Iter1 begin, Iter1 end, const T data)
	{
		auto i = begin;
		while (i++ != end)
			*i = data;
	}
	template<typename Iter1, typename T>
	constexpr inline void Clear(Iter1 begin, size_t c, const T data)
	{
		return Clear(begin, begin + c, data);
	}
	template<std::unsigned_integral T>
	consteval T Log2Align(T val)
	{
		return val > 1 ? Log2Align(val >> 1) + 1 : 0;
	}
	template<typename Pack = size_t, auto PackSizeLog2 = Log2Align(sizeof(Pack))>
	inline void FastDataCopy(const void* from, void* to, size_t count)
	{
		auto bigCount = count >> PackSizeLog2;
		auto alignCount = bigCount << PackSizeLog2;
		Copy((const size_t*)from, (size_t*)to, bigCount);
		if (alignCount != count) // Ä©Î²×Ö½Ú
		{
			auto from_end = (const byte*)from + alignCount;
			auto to_end = (byte*)to + alignCount;
			Copy(from_end, to_end, count - alignCount);
		}
	}
	constexpr inline auto MakeNtString(wchar_t* b)
	{
		if (b == 0)
			return nt::native::NtString{ 0, 2, (wchar_t*)L"" };
		auto strlen = GetStringLength(b) * sizeof(wchar_t);
		return nt::native::NtString{ (ushort)strlen, (ushort)(strlen + sizeof(wchar_t)), (wchar_t*)b };
	}
	template<typename Char>
	constexpr inline auto MakeWin32String(Char* b)
	{
		auto strlen = GetStringLength(b) * sizeof(Char);
		return nt::native::Win32String{ (ushort)strlen, (ushort)(strlen + sizeof(Char)), std::is_same_v<Char, char>, (wchar_t*)b };
	}
	template<typename Begin, typename End>
	class EnumProxy
	{
		Begin b;
		End e;
	public:
		ForceInline EnumProxy(Begin begin, End end)
		{
			b = begin;
			e = end;
		}
		ForceInline Begin begin()
		{
			return b;
		}
		ForceInline End end()
		{
			return e;
		}
		ForceInline auto& operator [](size_t i)
		{
			return b[i];
		}
	};
}