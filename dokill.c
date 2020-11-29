#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "rd.h"

void do_kill(psinfo* ary, int size){
	if(size == 0)
		printf("There is nothing available process to kill\n");
	for(int i = 0; i < size; i++){
		printf("print check of pid(%d) : %d and my pid is %d\n", ary[i].pid, ary[i].checkTokill, getpid());
		if(ary[i].pid != getpid() && ary[i].checkTokill == 0 && ary[i].pid != getppid()){
			printf("processor (%d) will be killed\n", ary[i].pid);
			kill(ary[i].pid, SIGINT);
		}
	}
}


void do_must_kill(psinfo* ary, int size){
	if(size == 0)
		printf("There is nothing available process to kill\n");
	for(int i = 0; i < size; i++){
		printf("print check of pid(%d) : %d and my pid is %d ppid is %d\n", ary[i].pid, ary[i].checkTokill, getpid(), getppid());
		if(ary[i].pid != getpid() && ary[i].checkTokill == 0 && ary[i].pid != getppid()){
			printf("processor (%d) will be killed\n", ary[i].pid);
		//	sleep(60);
			kill(ary[i].pid, SIGKILL);
		}
	}
}


void get_pid(psinfo* ary1, int size1, psinfo* ary2, int size2)
{
	int input;

	getchar();
	while(input != -1)
	{
		printf("what is want to kill pid?(exit : -1)");
		scanf(" %d", &input);
//		printf("input is %d\n", input);

		for(int i = 0; i < size1; i++)
		{
			if(ary1[i].pid == input)
			{
				printf("pid(%d) is exception\n", ary1[i].pid);
				ary1[i].checkTokill = 1;
               		}
           
		}

		for(int i = 0; i < size2; i++)
            {
                if(ary2[i].pid == input)
                    {
                        printf("pid(%d) is exception\n", ary2[i].pid);
                        ary2[i].checkTokill = 1;
                    }

                }
		getchar();
	}
}
