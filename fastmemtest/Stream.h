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
		/// ���̳�����дʱ���رյ�ǰ��
		/// </summary>
		virtual void Close() = 0;
		/// <summary>
		/// ���̳�����дʱ���ӵ�ǰ����ȡһ���ֽڵ����ݲ�������λ�����������ݺ�(position+=length)
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="length"></param>
		/// <returns></returns>
		virtual int Read(char* buffer,size_t length) = 0;
		/// <summary>
		/// ���̳�����дʱ����ǰ��д��һ���ֽڵ����ݲ�������λ�����������ݺ�(position+=length)
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="length"></param>
		/// <returns></returns>
		virtual int Write(const char* buffer, size_t length) = 0;
		/// <summary>
		/// ����������������ղ�����ʵ��IO����(�������л�����ʱ)
		/// </summary>
		virtual void Flush() = 0;
		/// <summary>
		/// ���õ�ǰ����λ��
		/// </summary>
		/// <param name="offset">��originָ����λ�õ�ƫ��(����Ϊ��)</param>
		/// <param name="origin">offset��Ե�ԭ��λ��</param>
		/// <returns>������λ��</returns>
		virtual long Seek(long offset, SeekOrigin origin) = 0;
		/// <summary>
		/// �Ƿ���Զ�ȡ
		/// </summary>
		virtual bool CanRead() {
			return false;
		}
		/// <summary>
		/// �Ƿ���Զ�ȡ/����λ��
		/// </summary>
		virtual bool CanSeek() {
			return false;
		}
		/// <summary>
		/// �Ƿ����д��
		/// </summary>
		virtual bool CanWrite(){
			return false;
		}
		/// <summary>
		/// �Ƿ�������ó�ʱ��Ϊtrueʱ��ʹ��Read/WriteTimeout fields.
		/// </summary>
		virtual bool CanTimeout() {
			return false;
		}
		/// <summary>
		/// ��ȡ��ǰλ�ã���CanSeekΪfalse���򷵻�-1
		/// </summary>
		virtual ulong Position() {
			if (!CanSeek())
				return -1;
			return Seek(0, SeekOrigin::Current);
		}
		/// <summary>
		/// ��ȡ�����ȣ���CanSeekΪfalse���򷵻�0
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
					// �ر��˸�����
				}
				int Read(char* buffer, size_t length) override
				{
					return 0; // ����˸���į
				}
				int Write(const char* buffer, size_t length) override
				{
					return 0; // ��д�˸���į
				}
				void Flush() override
				{
					// ���ˢ�˸���į
				}
				long Seek(long offset, SeekOrigin origin) override
				{
					return 0; // ��ô Seek ���� 0
				}
			};
			ppstream = new NullStream();
		}
	};
}