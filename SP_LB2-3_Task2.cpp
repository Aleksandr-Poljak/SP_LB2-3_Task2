#include <windows.h>
#include <stdio.h>

void main(void)
{
	const int nTimerUnitsPerSecond = 10000000;
	int i = 0;
	LARGE_INTEGER li15;
	li15.QuadPart = -(15 * nTimerUnitsPerSecond);
	LARGE_INTEGER li10;
	li10.QuadPart = -(10 * nTimerUnitsPerSecond);
	HANDLE hTimer15 = CreateWaitableTimer(NULL, FALSE, NULL);
	HANDLE hTimer10 = CreateWaitableTimer(NULL, FALSE, NULL);
	while (i < 10)
	{
		if (SetWaitableTimer(hTimer15, &li15, 1000, NULL, NULL,
			FALSE))
		{
			printf("Waiting 15 seconds...\n");
			WaitForSingleObject(hTimer15, INFINITE);
			printf("Start #% d. \n", i + 1);
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			PROCESS_INFORMATION pi;
			WCHAR str[] = L"C:\\Windows\\notepad.exe";
			CreateProcessW(NULL,
				str,
				NULL,
				NULL,
				FALSE,
				CREATE_DEFAULT_ERROR_MODE,
				NULL,
				NULL,
				&si,
				&pi);
			CancelWaitableTimer(hTimer15);
			if (SetWaitableTimer(hTimer10, &li10, 1000, NULL, NULL,
				FALSE))
			{
				printf(("Waiting 10 seconds...\n"));
				WaitForSingleObject(hTimer10, INFINITE);
				printf("Stop #%d. \n", i + 1);
				TerminateProcess(pi.hProcess, 0);
				CancelWaitableTimer(hTimer10);
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			i++;
		}
	}
}
