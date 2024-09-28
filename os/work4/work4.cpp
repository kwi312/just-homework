#include "shared.h"

fMAIN {
	int threadTime[5];
	std::vector<std::thread> threads;
	restart:
	threads.clear();
	PRINT("time:");
	std::cin >> threadTime[0] >> threadTime[1] >> threadTime[2] >> threadTime[3] >> threadTime[4];
	for (int i = 0; i < 5; ++i) {
		threadInfo_t* tdata = new threadInfo_t;
		tdata->tid = i;
		tdata->workTime = threadTime[i];
		threads.push_back(std::thread(&threadFunc, tdata));
	}
	for (int i = 0; i < 5; ++i) {
		threads[i].join();
	}
	getInput:
	GET_INPUT();
	return 0;
}
