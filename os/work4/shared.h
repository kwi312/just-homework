#include <unistd.h>
#define fMAIN int main(int argc, char** argv)
#define GET_INPUT() 	PRINT("press \'R\' to restart or \'X\' to exit");\
	char btn = getc(stdin);\
	if (btn == 'r' || btn == 'R') {\
		goto restart;\
	} else if (btn == 'x' || btn == 'X') {\
		exit(0);\
	} else {\
		goto getInput;\
	}
#ifdef C_WITH_CLASSES
#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>
#define STARTMSG "thread started: "
#define STOPMSG "thread stopped: "
#define PRINTN(str, number) std::cout << str << number << std::endl;
#define PRINT(str) std::cout << str;
#else
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define STARTMSG "thread %d started\n"
#define STOPMSG "thread %d stopped\n"
#define PRINTN(str, number) printf(str, number);
#define PRINT(str) printf(str);
#endif

typedef struct {
	int tid;
	int workTime;
} threadInfo_t;

void* threadFunc(void* info) {
	PRINTN(STARTMSG, ((threadInfo_t*)info)->tid);
	sleep(((threadInfo_t*)info)->workTime);
	PRINTN(STOPMSG, ((threadInfo_t*)info)->tid);
	#ifdef C_WITH_CLASSES
		delete[] (threadInfo_t*)info;
	#else
		free(info);
	#endif
	return NULL;
}
