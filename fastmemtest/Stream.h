#pragma once
#include "Defines.h"
namespace System::IO
{
	enum class SeekOrigin
	{
		Begin = 0,
		Current = 1,
		End = 2,
	};
	class Stream
	{
	public:
		int ReadTimeout = -1;
		int WriteTimeout = -1;
		/// <summary>
		/// 当继承类重写时，关闭当前流
		/// </summary>
		virtual void Close() = 0;
		/// <summary>
		/// 当继承类重写时，从当前流读取一定字节的数据并将流的位置提升到数据后(position+=length)
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="length"></param>
		/// <returns></returns>
		virtual int Read(char* buffer,size_t length) = 0;
		/// <summary>
		/// 当继承类重写时，向当前流写入一定字节的数据并将流的位置提升到数据后(position+=length)
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="length"></param>
		/// <returns></returns>
		virtual int Write(const char* buffer, size_t length) = 0;
		/// <summary>
		/// 将缓冲区的数据清空并进行实际IO操作(仅当流有缓冲区时)
		/// </summary>
		virtual void Flush() = 0;
		/// <summary>
		/// 设置当前流的位置
		/// </summary>
		/// <param name="offset">与origin指定的位置的偏移(可以为负)</param>
		/// <param name="origin">offset相对的原点位置</param>
		/// <returns>流的新位置</returns>
		virtual long Seek(long offset, SeekOrigin origin) = 0;
		/// <summary>
		/// 是否可以读取
		/// </summary>
		virtual bool CanRead() {
			return false;
		}
		/// <summary>
		/// 是否可以读取/设置位置
		/// </summary>
		virtual bool CanSeek() {
			return false;
		}
		/// <summary>
		/// 是否可以写入
		/// </summary>
		virtual bool CanWrite(){
			return false;
		}
		/// <summary>
		/// 是否可以设置超时，为true时则使用Read/WriteTimeout fields.
		/// </summary>
		virtual bool CanTimeout() {
			return false;
		}
		/// <summary>
		/// 获取当前位置，若CanSeek为false，则返回-1
		/// </summary>
		virtual ulong Position() {
			if (!CanSeek())
				return -1;
			return Seek(0, SeekOrigin::Current);
		}
		/// <summary>
		/// 获取流长度，若CanSeek为false，则返回0
		/// </summary>
		virtual ulong Length() {
			if (!CanSeek())
				return 0;
			auto cur = Position();
			auto len = Seek(0, SeekOrigin::End);
			Seek(cur, SeekOrigin::Begin);
			return len;
		}
		static void CreateNull(Stream*& ppstream)
		{
			class NullStream : public Stream
			{
				bool CanWrite() override
				{
					return true;
				}
				bool CanRead() override
				{
					return true;
				}
				void Close() override
				{
					// 关闭了个空气
				}
				int Read(char* buffer, size_t length) override
				{
					return 0; // 你读了个寂寞
				}
				int Write(const char* buffer, size_t length) override
				{
					return 0; // 你写了个寂寞
				}
				void Flush() override
				{
					// 你冲刷了个寂寞
				}
				long Seek(long offset, SeekOrigin origin) override
				{
					return 0; // 怎么 Seek 都是 0
				}
			};
			ppstream = new NullStream();
		}
	};
}