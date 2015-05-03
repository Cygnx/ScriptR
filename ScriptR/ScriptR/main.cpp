#include <windows.h>

extern void Main(void);
extern void TrainerLoop();
::BOOL WINAPI DllWork ( __in ::HMODULE hModule )
{
		AllocConsole();
		CreateThread(NULL, 0, (::LPTHREAD_START_ROUTINE)&Main, (::HMODULE)hModule, 0, NULL);
		CreateThread(NULL, 0, (::LPTHREAD_START_ROUTINE)&TrainerLoop, (::HMODULE)hModule, 0, NULL) ;
	return true;
}

::BOOL WINAPI DllMain ( __in ::HMODULE hModule, __in ::DWORD dwReason, __in __reserved ::LPVOID lpvReserved )
{
	::HANDLE hThread = NULL;
	if ( dwReason == DLL_PROCESS_ATTACH ) 
	{
		if (( hThread = ::CreateThread(NULL, 0, (::LPTHREAD_START_ROUTINE)&DllWork, (::HMODULE)hModule, 0, NULL) ) == NULL ) 
		{
			return FALSE;
		}
		if ( ::CloseHandle(hThread) == FALSE )
		{
			//do nothing
		}
	}
	return TRUE;
}