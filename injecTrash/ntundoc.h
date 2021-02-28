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

	LPFUN_NtCreateThreadEx GetNtCreateThreadEx();
}