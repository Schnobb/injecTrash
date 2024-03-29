#include "ntundoc.h"

namespace ntundoc
{
	LPFUN_NtCreateThreadEx GetNtCreateThreadEx()
	{
		HMODULE ntdll = GetModuleHandleA("ntdll.dll");
		if (!ntdll)
			return nullptr;

		return (LPFUN_NtCreateThreadEx)GetProcAddress(ntdll, "NtCreateThreadEx");
	}

	LPFUN_RtlCreateUserThread GetRtlCreateUserThread()
	{
		HMODULE ntdll = GetModuleHandleA("ntdll.dll");
		if (!ntdll)
			return nullptr;

		return (LPFUN_RtlCreateUserThread)GetProcAddress(ntdll, "RtlCreateUserThread");
	}
}