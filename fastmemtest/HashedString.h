#pragma once
#include "Algorithm.h"
namespace nt::utility
{
	class HashedString
	{
	private:
		void _check_hash();
		void _setup(const char*) noexcept;
		void _add_ref() noexcept;
		void _release() noexcept;

	public:
		HashedString() noexcept = default;
		StringHash Hash = 0;
		HashedString(const HashedString& rhs)
		{
			Hash = rhs.Hash;
			_add_ref();
		}
		HashedString& operator=(const HashedString& rhs) 
		{
			Hash = rhs.Hash;
			_add_ref();
			return *this;
		}
		bool operator<(const HashedString& rhs) const
		{
			return Hash < rhs.Hash;
		}
		bool operator==(const HashedString& rhs) const
		{
			return Hash == rhs.Hash;
		}
		bool operator>(const HashedString& rhs) const
		{
			return Hash > rhs.Hash;
		}
		~HashedString()
		{
			_release();
		}
		HashedString(StringHash hash)
		{
			Hash = hash;
			_check_hash();
		}
		HashedString(const char* utf8) noexcept
		{
			_setup(utf8);
		}

		const char* GetUnicodeString();
		static unsigned long long GetStringPoolCount() noexcept;
		static void Collect() noexcept;
	};
}