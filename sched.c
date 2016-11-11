/* Works stably on 32-bit. */

#ifdef __cplusplus
#  define EXTERN extern "C"
#else
#  define EXTERN
#endif // __cplusplus

#include "sched.h"

static int process_count;
static int activ_process;
static int first_time;
static int flag_fin;
static int check_time;
static int step;
static int check_fin[PROCESS_NUMBER];
static int offset[PROCESS_NUMBER];
static int quantum[PROCESS_NUMBER][PROCESS_NUMBER];

EXTERN int sched_init(void)
{
	process_count = 0;
	activ_process = 1;
	first_time = 1;
	flag_fin = 0;
	check_time = 0;
	step = 1;
	return 0;
}

EXTERN int add_proc(void)
{
	check_fin[++process_count] = 0;
	check_fin[process_count+1] = 0;
	offset[process_count] = 1;
	return process_count;
}

EXTERN int sched(int time, int cont)
{	
	if (first_time) {
		first_time = 0;
		return 1;
	}
	
	quantum[activ_process][offset[activ_process]] = time;
	(offset[activ_process])++;
	
	if (!cont) {
		check_fin[activ_process] = 1;
		flag_fin++;
	}
	
	if (flag_fin == process_count) return -1;
	
	if (quantum[activ_process][step] != check_time) check_time++;
	
	if ((quantum[activ_process][step] == check_time) || (!cont)) {
		activ_process++;
		check_time = 0;
		
		if (process_count < activ_process) {
			activ_process = 1;
			step++;
		}
		
		while (check_fin[activ_process]) {
			activ_process++;
			if (process_count < activ_process) activ_process = 1;
		}
	}
	
	return activ_process;
}

EXTERN int sched_fin(void)
{
	return 0;
}
