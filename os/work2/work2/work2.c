#ifndef UNICODE
#define UNICODE
#endif
/*code in C++ will be completely similar due to same api*/
#include <Shlwapi.h>
#include <WinBase.h>
#include <strsafe.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <heapapi.h>
#include <winerror.h>
#include <WinConTypes.h>
#include <WinUser.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")

typedef struct {
	HANDLE handle;
	DWORD sleepTime;
	HANDLE heap;
	DWORD tid;
}THREADARGUMENTINFO;

static HANDLE console;
static HANDLE consoleMutex;

void err(LPCWSTR errstr) {
	SetConsoleTextAttribute(console, FOREGROUND_RED);
	WriteConsoleW(console, errstr, (DWORD)lstrlenW(errstr), 0, NULL);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	ExitProcess(1);
}
void twrite(LPCWSTR str) {
	/*WaitForSingleObject(consoleMutex, INFINITE);*/
	WriteConsoleW(console, str, (DWORD)lstrlenW(str), 0, NULL);
	/*if (!ReleaseMutex(consoleMutex)) {
		err(L"mutex release error");
	}*/
}
void threadWorker(THREADARGUMENTINFO* info) {
	STRSAFE_LPCWSTR threadString = HeapAlloc(info->heap, HEAP_ZERO_MEMORY, sizeof(STRSAFE_LPCWSTR) * 128);
	if (SUCCEEDED(StringCchPrintfW(threadString, 128, L"Thread with id: %d started.\t%d\n", info->tid, info->sleepTime))) {
		twrite(threadString);
	}
	else {
		err(L"thread string error");
	}
	Sleep(info->sleepTime);
	if (SUCCEEDED(StringCchPrintfW(threadString, 128, L"Thread with id: %d stopped\n", info->tid))) {
		twrite(threadString);
	}
	else {
		err(L"thread string error");
	}
	HeapFree(info->heap, 0, threadString);
	HeapFree(info->heap, 0, info);
}
int wmain(int argc, wchar_t** argv) {

	console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE consoleIn = GetStdHandle(STD_INPUT_HANDLE);
	/*consoleMutex = CreateMutexW(NULL, FALSE, L"CONSOLE_MUTEX");*/
	int threadCount = argc - 1;
	if (threadCount != 5) {
		err(L"argument count mismatch");
	}
	HANDLE hheap = HeapCreate(0, 0, 0);
	if (hheap == NULL) {
		err(L"heap creation error");
	}
	HANDLE threadHandles[5];
restart:
	for (size_t tid = 1; tid <= threadCount; tid++) {
		THREADARGUMENTINFO* threadInfo = HeapAlloc(hheap, HEAP_ZERO_MEMORY, sizeof(THREADARGUMENTINFO));
		if (threadInfo == NULL) {
			err(L"thread data allocation info");
		}
		if (!StrToIntExW(argv[tid], STIF_DEFAULT, &(threadInfo->sleepTime))) {
			err(L"string to integer conversion error");
		}
		twrite(L"starting thread\n");
		threadHandles[tid - 1] = CreateThread(NULL, 0, threadWorker, threadInfo, 0, NULL);
		threadInfo->handle = threadHandles[tid - 1];
		threadInfo->heap = hheap;
		threadInfo->tid = tid - 1;
		ResumeThread(threadInfo->handle);
	}
	DWORD wStatus = 0;
	do {
		wStatus = WaitForMultipleObjects(threadCount, threadHandles, TRUE, INFINITE);
		if (wStatus == WAIT_FAILED) {
			LPCWSTR errString = HeapAlloc(hheap, HEAP_ZERO_MEMORY, sizeof(LPCWSTR) * 128);
			StringCchPrintfW(errString, 128, L"wait failed: %d\n", GetLastError());
			err(errString);
		}
	} while (wStatus != WAIT_OBJECT_0);
	INPUT_RECORD choice;
rechoice:
	twrite(L"Press \"X\" to exit or \"R\" to restart:");
reInput:
	if (!ReadConsoleInputW(consoleIn, &choice, 1, &wStatus)) {
		err(L"console input error");
	}
	twrite(L"\n");
	if (choice.Event.KeyEvent.bKeyDown) { goto reInput; }
	if ((choice.Event).KeyEvent.wVirtualKeyCode == 0x52) { /*R*/
		goto restart;
	} else if ((choice.Event).KeyEvent.wVirtualKeyCode == 0x58) { /*X*/
		ExitProcess(0);
	} else {
		goto rechoice;
	}
	return 0;
}