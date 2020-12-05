#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <time.h>
#include <linux/limits.h>
#include <sys/times.h>
#include <ncurses.h>
#include "rd.h"


extern int start;
extern int map_num;

void read_one(int *x, FILE* input) { fscanf(input, "%d ", x); }
void read_unsigned(unsigned long long *x, FILE* input) { fscanf(input, "%llu ", x); }
void read_str(char *x, FILE* input) {  fscanf(input, "%s ", x);}
void read_char(char *x, FILE* input) {  fscanf(input, "%c ", x);}

int get_timesinceboot(long tickspersec) {
  FILE *procuptime;
  int sec, ssec;

  procuptime = fopen("/proc/uptime", "r");
  fscanf(procuptime, "%d.%ds", &sec, &ssec);
  fclose(procuptime);
  return (sec*tickspersec)+ssec;
}

void print_timedif(unsigned long long x, float rtime, long tickspersec) 
{
  int sinceboot = get_timesinceboot(tickspersec);
  int running = sinceboot - x;
  time_t rt = time(NULL) - (running / tickspersec);
  char buf[1024];

  strftime(buf, sizeof(buf), "%m.%d %H:%M", localtime(&rt));
  printw("%s (%8.2fs)", buf, rtime);
  refresh();
}

void make_Time(unsigned long long x, float* rtime)
{
	long tickspersec;
	tickspersec = sysconf(_SC_CLK_TCK);
	int sinceboot = get_timesinceboot(tickspersec);
 	int running = sinceboot - x;

  	*rtime = running / tickspersec + (running % tickspersec) / 100.0;
}

int print_psinfo(psinfo* ary, int size, int curindex, int lines)
{
	long tickspersec;
	int i = 0;
	int j = 0;
	
	tickspersec = sysconf(_SC_CLK_TCK);
	
	move(1, 0);
	if ((map_num % 3) == 0)
		printw("TOTAL : %d\t\tAll Process Table  ", size);
	else if ((map_num % 3) == 1)
		printw("TOTAL : %d\t\tBash Process Table  ", size);
	else if ((map_num % 3) == 2)
		printw("TOTAL : %d\t\tUser Process Table  ", size);
	refresh();
	
	

	for(i = start, j = 0; j < lines - 6; i++, j++)
	{

		if(i >= size){
			move(j + 3, 0);
			printw("\t\t\t\t\t\t\t\t\t\t\t\t");
			refresh();
		}
		else{
			move(j + 3, 0);
			printw("%4d %40s %5c %4d ", ary[i].pid, ary[i].comm, ary[i].state, ary[i].ppid);
			print_timedif(ary[i].start_time, ary[i].runningTime, tickspersec);
			printw("   %d\n", ary[i].checkTokill);
			refresh();
		}
	}
	
	return i;

}

void reset_arry(psinfo* ary, int* size)
{
	for(int i = 0; i < *size; i++)
	{
		ary[i].pid = -1;
		strcpy(ary[i].comm, "");
		ary[i].state = '\0';
		ary[i].ppid = -1;
		ary[i].start_time = 0;
		
	}
	*size = 0;

}

void store_psinfo(psinfo* ary, int pid, int i)
{
	char curPid[20] = "\0";
	int cnt = 0;
	int temp;
	char nowpwd[255];
	
	getcwd(nowpwd, 255); //현재 경로 저장
	
	FILE* input;

	input = NULL;
	
	sprintf(curPid, "%d", pid);
	
	if(pid)
	{	
		chdir("/proc");
		if(chdir(curPid) == 0)
			input = fopen("stat", "r");
		if(!input)
		{
			perror("open");
			exit(1);
		}
	}
	else
		input = stdin;
		
	read_one(&(ary[i].pid), input);
	read_str(ary[i].comm, input);
	read_char(&(ary[i].state), input);
	read_one(&(ary[i].ppid), input);
	ary[i].checkTokill = 0;
	if(ary[i].pid == getpid() || ary[i].pid == getppid()) // current process and it's bash
		ary[i].checkTokill = 1;

	while(cnt < 17)
	{
		read_one(&temp, input);
		cnt++;
	}
	cnt = 0;		
	read_unsigned(&(ary[i].start_time), input);
	make_Time(ary[i].start_time, &(ary[i].runningTime));
	
	chdir(nowpwd);
	rewind(input);
	fclose(input);
	
}
