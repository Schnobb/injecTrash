#include <iostream>
#include <Windows.h>
#include "memUtils.h"
#include "processUtils.h"
#include "ntundoc.h"

int wmain(int argc, wchar_t* argv[])
{
	if (argc != 3)
	{
		wchar_t filename[_MAX_FNAME + 1];
		wchar_t extension[_MAX_EXT + 1];
		_wsplitpath_s(argv[0], nullptr, 0, nullptr, 0, filename, _MAX_FNAME + 1, extension, _MAX_EXT + 1);

		wprintf_s(L"Usage: %s%s <dll to inject> <process>\n", filename, extension);
		return 1;
	}

	wchar_t* dllPath = argv[1];
	wchar_t* processName = argv[2];

	if (GetFileAttributesW(dllPath) == INVALID_FILE_ATTRIBUTES)
	{
		wprintf_s(L"Could not open file '%s'\n", dllPath);
		return 2;
	}

	HANDLE hProcess = processUtils::GetProcessHandle(processName);
	if (!hProcess)
	{
		wprintf_s(L"Could not find process '%s'\n", processName);
		return 3;
	}


	uintptr_t dllPathAddress = (uintptr_t)VirtualAllocEx(hProcess, nullptr, wcsnlen_s(dllPath, MAX_PATH) * sizeof(wchar_t) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!dllPathAddress)
	{
		wprintf_s(L"Could not allocate memory for DLL path\n");
		return 5;
	}


	memUtils::WriteMemoryEx(hProcess, dllPathAddress, dllPath, wcsnlen_s(dllPath, MAX_PATH) * sizeof(wchar_t) + 1);
	wprintf_s(L"DLL Path Location: 0x%p\n", reinterpret_cast<uintptr_t*>(dllPathAddress));

	HMODULE hKernel32 = GetModuleHandleA("Kernel32.dll");
	if (!hKernel32)
	{
		wprintf_s(L"Kernel32.dll is not loaded?!\n");
		return 4;
	}

	uintptr_t loadLibraryWAddress = (uintptr_t)GetProcAddress(hKernel32, "LoadLibraryW");
	ntundoc::LPFUN_NtCreateThreadEx NtCreateThreadEx = ntundoc::GetNtCreateThreadEx();
	ntundoc::LPFUN_RtlCreateUserThread RtlCreateUserThread = ntundoc::GetRtlCreateUserThread();

	ntundoc::CLIENT_ID cid;

	HANDLE hThread;

	NTSTATUS status = RtlCreateUserThread(hProcess, 0, false, 0, 0, 0, (LPTHREAD_START_ROUTINE)loadLibraryWAddress, (LPVOID)dllPathAddress, &hThread, &cid);
	WaitForSingleObject(hThread, INFINITE);
	if (status < 0 || !hThread)
	{
		wprintf_s(L"Could not start thread via RtlCreateUserThread (0x%08x)\n", status);
		return 6;
	}

	wprintf_s(L"Thread: 0x%p\n", hThread);

	if (WaitForSingleObject(hThread, 5000))
	{
		wprintf_s(L"Thread timed out or failed to execute\n");
		return 7;
	}

	wprintf_s(L"Injection successful!\n");
	VirtualFreeEx(hProcess, (LPVOID)dllPathAddress, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return 0;
}