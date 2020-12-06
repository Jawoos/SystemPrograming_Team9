#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ncurses.h>
#include "rd.h"

#define BLANK "\t\t\t\t\t\t\t\t\t\t        "

void do_kill(psinfo* ary, int size){
	int total = 0;
	if(size == 1)
	{
		printw("There is nothing available process to kill");
		refresh();
	}
	for(int i = 0; i < size; i++){
		//printf("print check of pid(%d) : %d and my pid is %d\n", ary[i].pid, ary[i].checkTokill, getpid());
		if(ary[i].checkTokill == 0){
			//printf("processor (%d) will be killed\n", ary[i].pid);
			total++;
			kill(ary[i].pid, SIGINT);
		}
	}
	printw("total kill: %d", total);
	refresh();
	sleep(2);
}


void do_must_kill(psinfo* ary, int size){
	int total = 0;
	if(size == 1)
	{
		printw("There is nothing available process to kill");
		refresh();
	}
	for(int i = 0; i < size; i++){
		//printf("print check of pid(%d) : %d and my pid is %d ppid is %d\n", ary[i].pid, ary[i].checkTokill, getpid(), getppid());
		if(ary[i].checkTokill == 0){
			//printf("processor (%d) will be killed\n", ary[i].pid);
			//	sleep(60);
			total++;
			kill(ary[i].pid, SIGKILL);
		}
	}
	printw("total kill: %d", total);
	refresh();
	sleep(2);
}


void get_pid(psinfo* ary1, int size1, psinfo* ary2, int size2, int lines)
{
	signal(SIGALRM, SIG_IGN);
	int input;
	int i;
	sleep(1);
	while(input != -1)
	{
		move(lines - 2, 0);
		printw("\t\t\t\t\t");
		refresh();
		move(lines - 2, 0);
		printw("what is want to kill pid?(exit : -1)");
		scanw("%d", &input);
		refresh();
		move(lines -1, 0);
		sleep(1);
		addstr(BLANK);
		refresh();

		for(i = 0; i < size1; i++) 
		{
			if(ary1[i].pid == input)
			{
				move(lines -1, 0);
				printw("pid(%d) is exception", ary1[i].pid);
				refresh();
				ary1[i].checkTokill = 1;
			}
		}

		for(i = 0; i < size2; i++)
		{
			if(ary2[i].pid == input)
			{
				move(lines -1, 0);
				printw("pid(%d) is exception", ary2[i].pid);
				refresh();
				ary2[i].checkTokill = 1;
			}
		}
	}
}

void store_CK(psinfo* ary, int size, int* CK, int* cksize) //store checkTokill's pid
{
	for(int i = 0;  i < size; i++)
	{
		if(ary[i].checkTokill == 1)
			CK[(*cksize)++] = ary[i].pid;
	}
}

void set_CK(psinfo* ary, int size, int* CK, int cksize) //set checkTokill values
{
	for(int i = 0;  i < size; i++)
	{
		for(int j = 0; j < cksize; j++)
		{
			if(CK[j] == ary[i].pid)
				ary[i].checkTokill = 1;
		}
	}
}
