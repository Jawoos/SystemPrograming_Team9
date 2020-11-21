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
#include "rd.h"

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

void print_timediff(char *name, unsigned long long x, long tickspersec) {
  int sinceboot = get_timesinceboot(tickspersec);
  int running = sinceboot - x;
  time_t rt = time(NULL) - (running / tickspersec);
  char buf[1024];

  strftime(buf, sizeof(buf), "%m.%d %H:%M", localtime(&rt));
  printf("%s: %s (%lu.%lus)\n", name, buf, running / tickspersec, running % tickspersec);
}

void print_psinfo(psinfo* ary, int size)
{
	long tickspersec;
	tickspersec = sysconf(_SC_CLK_TCK);
	
	printf("TOTAL : %d\n", size);
	for(int i = 0; i < size; i++)
	{
		printf("PID : %d, COMMAND : %s, STATE : %c, PPID : %d, ", 
					ary[i].pid, ary[i].comm, ary[i].state, ary[i].ppid);
		print_timediff("start_time", ary[i].start_time, tickspersec);
	}

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
	char* curPid;
	int cnt = 0;
	int temp;
	int ck = 0;
	
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
		
	while(cnt < 17)
	{
		read_one(&temp, input);
		cnt++;
	}
	cnt = 0;		
	read_unsigned(&(ary[i].start_time), input);
	
	rewind(input);
	fclose(input);
	
}