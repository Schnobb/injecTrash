#pragma once
#include <Windows.h>

namespace ntundoc
{
	typedef NTSTATUS(WINAPI* LPFUN_NtCreateThreadEx)
		(
			OUT PHANDLE hThread,
			IN ACCESS_MASK DesiredAccess,
			IN LPVOID ObjectAttributes,
			IN HANDLE ProcessHandle,
			IN LPTHREAD_START_ROUTINE lpStartAddress,
			IN LPVOID lpParameter,
			IN BOOL CreateSuspended,
			IN DWORD StackZeroBits,
			IN DWORD SizeOfStackCommit,
			IN DWORD SizeOfStackReserve,
			OUT LPVOID lpBytesBuffer
			);

	typedef struct _CLIENT_ID
	{
		PVOID UniqueProcess;
		PVOID UniqueThread;
	} CLIENT_ID, * PCLIENT_ID;

	typedef NTSTATUS(*LPFUN_RtlCreateUserThread)
		(
			HANDLE,
			PSECURITY_DESCRIPTOR,
			BOOLEAN, 
			ULONG,
			PULONG, 
			PULONG,
			PVOID, 
			PVOID,
			PHANDLE, 
			PCLIENT_ID
			);

	LPFUN_NtCreateThreadEx GetNtCreateThreadEx();
	LPFUN_RtlCreateUserThread GetRtlCreateUserThread();
}