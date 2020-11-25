#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "rd.h"

void do_kill(psinfo* ary, int size){
	for(int i = 0; i < size; i++){
		printf("print check of pid(%d) : %d and my pid is %d\n", ary[i].pid, ary[i].checkTokill, getpid());
		if(ary[i].pid != getpid() && ary[i].checkTokill == 0){
			printf("processor (%d) will be killed\n", ary[i].pid);
			kill(ary[i].pid, SIGINT);
		}
	}
}

void get_pid(psinfo* ary, int size)
{
	int input;

	getchar();
	while(input != -1)
	{
		printf("what is want to kill pid?(exit : -1)");
		scanf(" %d", &input);
		printf("input is %d\n", input);

		for(int i = 0; i < size; i++)
		{
			if(ary[i].pid == input)
				ary[i].checkTokill = 1;

		}
		getchar();
	}





}
