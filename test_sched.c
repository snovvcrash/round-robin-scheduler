#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include "sched.h"

#define SAFE_DLSYM(name, type) do {												\
		name = type dlsym(handle, #name);										\
		if (!name) {															\
			fprintf(stderr,"Could not get symbol " #name ": %s", dlerror());	\
			dlclose(handle);													\
			return __LINE__;													\
		}																		\
	} while (0)

struct process {
	int id;
	int full_time;
	int current_time;
};

struct process process[PROCESS_NUMBER];

int main(int argc, char* argv[]) {
	void* handle;
	char* library_name;
	int (*sched_init)(void);
	int (*add_proc)(void);
	int (*sched)(int, int);
	void (*sched_fin)(void);
	int id, i, j;
	
    if (argc != 2) {
		fprintf(stderr, "Usage: %s <sched_lib>\n" ,argv[0]);
		return __LINE__;
	}
	
	if (access(argv[1], F_OK)) {
		perror("access");
		return __LINE__;
	}
	
	library_name = realpath(argv[1], NULL);
	
	if (!library_name) {
		perror("realpath");
		return __LINE__;
	}	
	
	handle = dlopen(library_name, RTLD_NOW);
	free(library_name);
	
	if (!handle) {
		fprintf(stderr, "Could not load %s: %s\n", argv[1], dlerror());
		return __LINE__;
	}
	
	SAFE_DLSYM(sched_init, (int (*)(void)));
	SAFE_DLSYM(add_proc,   (int (*)(void)));
	SAFE_DLSYM(sched,      (int (*)(int,int)));
	SAFE_DLSYM(sched_fin,  (void (*)(void)));

	if (sched_init()) {
		fprintf(stderr, "Error in sched_init\n");
		dlclose(handle);
		return __LINE__;
	}
	
	srand(time(NULL));
	
	for (i = 0; i < PROCESS_NUMBER; i++) {
		process[i].full_time = (rand() % 100) + 1;
		process[i].current_time = 0;
		process[i].id = add_proc();
		
		if (process[i].id == -1) {
			fprintf(stderr, "Error in add_proc\n");
			sched_fin();
			dlclose(handle);
			return __LINE__;
		}
		
		printf("Add process(%d)\n", process[i].id);
	}
	
	for (i = 0; i < PROCESS_NUMBER; i++)
		printf("process[%d] = %d\n\n", i+1, process[i].full_time);
	
	id = sched(1,1);
	
	while (id != -1) {
		for (i = 0; i < PROCESS_NUMBER; i++)
			if ( process[i].id == id ) break;
		
		if (i == PROCESS_NUMBER) {
			fprintf(stderr, "Incorrect id: %d\n", id);
			sched_fin();
			dlclose(handle);
			return __LINE__;
		}
		
		if (process[i].current_time == process[i].full_time) {
			fprintf(stderr,"Process %d is dead\n",id);
			sched_fin();
			dlclose(handle);
			return __LINE__;
		}
		
		printf("Process %d was started\n",id);
		process[i].current_time++;
		j = (rand() % 10) + 1;
		printf("time[%d] = %d\n\n", i+1, j);
		id = sched(j, (process[i].current_time != process[i].full_time));
	}
	
	printf("Test is done\n");	
	sched_fin();
	dlclose(handle);
	
    return 0;
}
