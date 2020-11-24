#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "rd.h"

void do_kill(psinfo* ary, int size){

	printf("TOTAL : %d\n", size);
	for(int i = 0; i < size; i++){
	//	printf("print check of pid(%d) : %d and my pid is %d\n", ary[i].pid, ary[i].checkTokill, getpid());
		if(ary[i].pid != getpid() && ary[i].checkTokill == 0)
			kill(ary[i].pid, SIGINT);
		return;
	}
}