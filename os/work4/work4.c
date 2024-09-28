#include "shared.h"

fMAIN {
	int threadTime[5];
	pthread_t threads[5];
	int readed;
	char cmd;
	restart:
	PRINT("time:");
	if ((readed = scanf("%d %d %d %d %d", &threadTime[0], &threadTime[1], &threadTime[2], &threadTime[3], &threadTime[4])) != 5) {
		fprintf(stderr, "invalid input:%d\n", readed);
		exit(0);
	}
	for (int i = 0; i < 5; ++i) {
		threadInfo_t* tdata = malloc(sizeof(threadInfo_t));
		tdata->tid = i;
		tdata->workTime = threadTime[i];
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&threads[i], &attr, threadFunc, tdata);
	}
	for (int i = 0; i < 5; ++i) {
		pthread_join(threads[i], NULL);
	}
	getInput:
	GET_INPUT();
	return 0;
}
