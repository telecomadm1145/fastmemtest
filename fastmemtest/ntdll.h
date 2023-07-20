#pragma once
#include "Importing.h"

namespace nt::native::ntdll
{
	enum class NtStatus : unsigned int
	{
		Success = 0,
	};
	enum class AccessMask : unsigned int
	{
		None = 0,
		Delete = 0x00010000L,
		ReadControl = 0x00020000L,
		WriteDac = 0x00040000L,
		WriteOwner = 0x00080000L,
		Sync = 0x00100000L,
		StdRightsRequired = 0x000F0000L,
		StdRightsRead = ReadControl,
		StdRightsWrite = ReadControl,
		StdRightsExecute = ReadControl,
		StdRightsAll = 0x001F0000L,
		SpecRightsAll = 0x0000FFFFL,
		AccessSystemSec = 0x01000000L,
		MaxAllowed = 0x02000000L,
		GenericRead = 0x80000000L,
		GenericWrite = 0x40000000L,
		GenericExecute = 0x20000000L,
		GenericAll = 0x10000000L,
		ReadData = (0x0001),    // file & pipe
		ListDirectory = (0x0001),    // directory
		WriteData = (0x0002),    // file & pipe
		AddFile = (0x0002),    // directory
		AppendData = (0x0004),    // file
		AddSubDirectory = (0x0004),    // directory
		CreatePipeInstance = (0x0004),    // named pipe
		ReadEa = (0x0008),    // file & directory
		WriteEa = (0x0010),    // file & directory
		Execute = (0x0020),    // file
		Traverse = (0x0020),    // directory
		DeleteChild = (0x0040),    // directory
		ReadAttributes = (0x0080),    // all
		WriteAttributes = (0x0100),    // all
		AllAccess = (StdRightsRequired | Sync | 0x1FF),
		FileGenericRead = (StdRightsRead | ReadData | ReadAttributes | ReadEa | Sync),
		FileGenericWrite = (StdRightsWrite | WriteData | WriteAttributes | WriteEa | AppendData | Sync),
		FileGenericExecute = (StdRightsExecute | ReadAttributes | Execute | Sync),
	};
	struct ObjectAttributes {
		ulong           Length = sizeof(ObjectAttributes); // of struct
		void* RootDirectory;
		NtString* ObjectName;
		ulong           Attributes;
		void* SecurityDescriptor;
		void* SecurityQualityOfService;
	};
	struct IoStatusBlock {
		union {
			enum class IoStatus : unsigned int
			{
				Superseded = 0x00000000,
				Opened = 0x00000001,
				Created = 0x00000002,
				Overwritten = 0x00000003,
				Exists = 0x00000004,
				DoesNotExist = 0x00000005,
			};
			IoStatus Status;
			void* Pointer; //Reversed
		};
		unsigned long Information;
	};
	enum class FileShareMode : ulong
	{
		None = 0,
		Read = 0x1,
		Write = 0x2,
		Delete = 0x4,
	};
	enum class FileAttribute : ulong
	{
		None = 0,
		Readonly = 0x00000001,
		Hidden = 0x00000002,
		System = 0x00000004,
		Directory = 0x00000010,
		Archive = 0x00000020,
		Device = 0x00000040,
		Normal = 0x00000080,
		Temporary = 0x00000100,
		SparseFile = 0x00000200,
		ReparsePoint = 0x00000400,
		Compressed = 0x00000800,
		Offline = 0x00001000,
		NotContentIndexed = 0x00002000,
		Encrypted = 0x00004000,
	};
	/*

		CreateDisposition 值	 	如果文件存在，则执行操作 	如果文件不存在，则执行操作
		Supersede			 	替换文件。 				创建文件。
		Create				 	返回错误。 				创建文件。
		Open				 	打开文件。 				返回错误。
		OpenIf					打开文件。 				创建文件。
		Overwrite				打开文件并覆盖该文件。		返回错误。
		OverwriteIf			 	打开文件并覆盖该文件。		创建文件。

	*/
	enum class FileMode : ulong
	{
		Normal = 0,
		Supersede = 0x00000000,
		Open = 0x00000001,
		Create = 0x00000002,
		OpenIf = 0x00000003,
		Overwrite = 0x00000004,
		OverwriteIf = 0x00000005,
		MaximumDisposition = 0x00000005,
	};
	NtStatus NtCreateFile(
		void** FileHandle, // out
		AccessMask        DesiredAccess,
		ObjectAttributes* ObjectAttributes,
		IoStatusBlock* IoStatusBlock, // out
		long long* AllocationSize, // opt
		FileAttribute              FileAttributes,
		FileShareMode              ShareAccess,
		FileMode              CreateDisposition,
		ulong              CreateOptions,
		void* EaBuffer,// opt
		ulong              EaLength
	);

	using IoApcRoutine = void (*) (void* ApcContext, IoStatusBlock* IoStatusBlock, ulong Reserved);
	using HeapMemory = void*;
	enum HeapFlags : uint {
		Default = 0,
		// No DEP
		CreateEnableExecute = 0x00040000,
		// Generate exception on OOM
		GenerateExceptions = 0x00000004,
		NoSerialize = 0x00000001,
	};
	NtStatus NtWriteFile(
		void* FileHandle,
		void* Event, // opt
		IoApcRoutine* ApcRoutine, // opt
		void* ApcContext, // opt
		IoStatusBlock* IoStatusBlock, // out
		const void* Buffer,
		ulong Length,
		ulonglong* ByteOffset, // opt in
		ulong* Key // opt
	);
	NtStatus NtReadFile(
		void* FileHandle,
		void* Event, // opt
		IoApcRoutine  ApcRoutine, // opt
		void* ApcContext, // opt
		IoStatusBlock* IoStatusBlock, // out
		void* Buffer, // out
		ulong Length,
		ulonglong* ByteOffset, // opt in
		ulong* Key // opt
	);
	NtStatus NtClose(
		void* Handle
	);
	struct ActCtxSectionKeyedData {
		ulong cbSize = sizeof(ActCtxSectionKeyedData);
		ulong ulDataFormatVersion;
		void* lpData;
		ulong ulLength;
		void* lpSectionGlobalData;
		ulong ulSectionGlobalDataLength;
		void* lpSectionBase;
		ulong ulSectionTotalLength;
		void* hActCtx;
		ulong ulAssemblyRosterIndex;
		ulong                                       ulFlags;
		struct ActCtxSectionKeyedDataAssemblyMetadata {
			void* lpInformation;
			void* lpSectionBase;
			ulong ulSectionLength;
			void* lpSectionGlobalDataBase;
			ulong ulSectionGlobalDataLength;
		} AssemblyMetadata;
	};
	struct strsection_header
	{
		uint magic;
		ulong size;
		uint unk1[3];
		ulong count;
		ulong index_offset;
		uint unk2[2];
		ulong global_offset;
		ulong global_len;
	};
	struct wndclass_redirect_data
	{
		ulong size;
		uint res;
		ulong name_len;
		ulong name_offset;   /* versioned name offset */
		ulong module_len;
		ulong module_offset; /* container name offset to the section base */
	};

	struct FilePositionInformation
	{
		ulonglong CurrentByteOffset;
	};
	struct FileBasicInformation
	{
		ulonglong CreationTime;
		ulonglong LastAccessTime;
		ulonglong LastWriteTime;
		ulonglong ChangeTime;
		ulong FileAttributes;
	};
	struct FileStandardInformation
	{
		ulonglong AllocationSize;
		ulonglong EndOfFile;
		ulong NumberOfLinks;
		bool DeletePending;
		bool Directory;
	};
	struct FileNameInformation
	{
		ulong FileNameLength;
		wchar_t FileName[1];
	};
	enum class FileInformation
	{
		FileBasicInformation = 4,
		FileStandardInformation = 5,
		FileInternalInformation = 6,
		FileEaInformation = 7,
		FileAccessInformation = 8,
		FileNameInformation = 9,
		FilePositionInformation = 14,
		FileModeInformation = 16,
		FileAlignmentInformation = 17,
		FileAllInformation = 18,
		FileNetworkOpenInformation = 34,
		FileAttributeTagInformation = 35,
		FileIoPriorityHintInformation = 43,
		FileIsRemoteDeviceInformation = 51,
		FileKnownFolderInformation = 76 // WinServer2022
	};
	NtStatus NtQueryInformationFile(void* handle,
		IoStatusBlock* status, // out
		void* fileinformation, // out
		ulong length,
		FileInformation type
	);
	NtStatus NtSetInformationFile(void* handle,
		IoStatusBlock* status, // out
		void* fileinformation, // in
		ulong length,
		FileInformation type
	);
	NtStatus RtlFindActivationContextSectionString(ulong  	flags,
		const Guid* guid,
		ulong  	section_kind,
		const NtString* section_name,
		ActCtxSectionKeyedData* ptr
	);
	NtStatus RtlReleaseActivationContext(void*);
	// Safe -> Unsafe
	NtStatus ProbeAndCaptureLargeString(Win32String* plstrSafe, // out
		Win32String* plstrUnsafe // in
	);
	HeapMemory RtlAllocateHeap(void* heap, HeapFlags flags, size_t s);
	bool RtlFreeHeap(void* heap, HeapFlags flags, HeapMemory block);
	void RtlExitUserProcess(uint uExitCode);
	struct SystemPerformanceInformation
	{
		ulonglong IdleProcessTime;
		ulonglong IoReadTransferCount;
		ulonglong IoWriteTransferCount;
		ulonglong IoOtherTransferCount;
		ulong IoReadOperationCount;
		ulong IoWriteOperationCount;
		ulong IoOtherOperationCount;
		ulong AvailablePages;
		ulong CommittedPages;
		ulong CommitLimit;
		ulong PeakCommitment;
		ulong PageFaultCount;
		ulong CopyOnWriteCount;
		ulong TransitionCount;
		ulong CacheTransitionCount;
		ulong DemandZeroCount;
		ulong PageReadCount;
		ulong PageReadIoCount;
		ulong CacheReadCount;
		ulong CacheIoCount;
		ulong DirtyPagesWriteCount;
		ulong DirtyWriteIoCount;
		ulong MappedPagesWriteCount;
		ulong MappedWriteIoCount;
		ulong PagedPoolPages;
		ulong NonPagedPoolPages;
		ulong PagedPoolAllocs;
		ulong PagedPoolFrees;
		ulong NonPagedPoolAllocs;
		ulong NonPagedPoolFrees;
		ulong FreeSystemPtes;
		ulong ResidentSystemCodePage;
		ulong TotalSystemDriverPages;
		ulong TotalSystemCodePages;
		ulong NonPagedPoolLookasideHits;
		ulong PagedPoolLookasideHits;
		ulong Spare3Count;
		ulong ResidentSystemCachePage;
		ulong ResidentPagedPoolPage;
		ulong ResidentSystemDriverPage;
		ulong CcFastReadNoWait;
		ulong CcFastReadWait;
		ulong CcFastReadResourceMiss;
		ulong CcFastReadNotPossible;
		ulong CcFastMdlReadNoWait;
		ulong CcFastMdlReadWait;
		ulong CcFastMdlReadResourceMiss;
		ulong CcFastMdlReadNotPossible;
		ulong CcMapDataNoWait;
		ulong CcMapDataWait;
		ulong CcMapDataNoWaitMiss;
		ulong CcMapDataWaitMiss;
		ulong CcPinMappedDataCount;
		ulong CcPinReadNoWait;
		ulong CcPinReadWait;
		ulong CcPinReadNoWaitMiss;
		ulong CcPinReadWaitMiss;
		ulong CcCopyReadNoWait;
		ulong CcCopyReadWait;
		ulong CcCopyReadNoWaitMiss;
		ulong CcCopyReadWaitMiss;
		ulong CcMdlReadNoWait;
		ulong CcMdlReadWait;
		ulong CcMdlReadNoWaitMiss;
		ulong CcMdlReadWaitMiss;
		ulong CcReadAheadIos;
		ulong CcLazyWriteIos;
		ulong CcLazyWritePages;
		ulong CcDataFlushes;
		ulong CcDataPages;
		ulong ContextSwitches;
		ulong FirstLevelTbFills;
		ulong SecondLevelTbFills;
		ulong SystemCalls;
	};
	enum class SystemInformation
	{
		SystemPerformanceInformation = 2,

	};
	NtStatus NtQuerySystemInformation(SystemInformation type,
		void* sysinfo,
		ulong length,
		ulong* retlength
	);
	NtStatus NtDeviceIoControlFile(void* handle,
		void* event,
		IoApcRoutine apcroutine,
		void* apccontext,
		IoStatusBlock* iostatus,
		ulong ioctlcode,
		void* input,
		ulong length,
		void* output,
		ulong outlength
	);
	NtStatus NtAllocateVirtualMemory(
		void* proc,
		void** baseaddress,
		ulonglong ZeroBits,
		size_t* regionsize,
		ulong allocationtype,
		ulong protect
	);
	NtStatus NtFreeVirtualMemory(
		void* proc,
		void** baseaddress,
		size_t* regionsize,
		ulong freetype // 0x00008000
	);
	enum class ExceptionDisposition
	{
		ExceptionContinueExecution,
		ExceptionContinueSearch,
		ExceptionNestedException,
		ExceptionCollidedUnwind
	};
	struct ExceptionRecord {
		constexpr static auto MaxParams = 15; // maximum number of exception parameters
		uint                    ExceptionCode;
		uint                    ExceptionFlags;
		struct ExceptionRecord* ExceptionRecord;
		void* ExceptionAddress;
		uint                    NumberParameters;
		void* ExceptionInformation[MaxParams];
	};
	void RtlRaiseException(ExceptionRecord* rec);
	class Ntdll : public nt::utility::ImportingDll
	{
	private:
		nt::utility::DllHelper _dll{ "ntdll.dll" };
	public:
		// Heap api
		DeclareWinApi2(RtlAllocateHeap); // needs memory to startup
		DeclareWinApi(RtlFreeHeap);

		// File api
		DeclareWinApi(NtClose);
		DeclareWinApi(NtCreateFile);
		DeclareWinApi(NtReadFile);
		DeclareWinApi(NtWriteFile);
		DeclareWinApi(NtQueryInformationFile);
		DeclareWinApi(NtSetInformationFile);
		DeclareWinApi(NtDeviceIoControlFile);

		// Proc api
		DeclareWinApi(RtlExitUserProcess);

		// Exception api
		DeclareWinApi(RtlRaiseException);

		// Mem api
		DeclareWinApi(NtAllocateVirtualMemory);
		DeclareWinApi(NtFreeVirtualMemory);

		// Misc api
		DeclareWinApi(ProbeAndCaptureLargeString);
	
		DeclareWinApi(NtQuerySystemInformation);

		DeclareWinApi(RtlFindActivationContextSectionString);
		DeclareWinApi(RtlReleaseActivationContext);

	public:
		static Ntdll Instance;
	};
}
namespace nt::native
{
	constexpr auto& Ntdll = nt::native::ntdll::Ntdll::Instance;
}