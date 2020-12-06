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
#include<sys/time.h>
#include<signal.h>
#include "rd.h"
#include "dokill.h"
#define MAX 1000
#define BLANK "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"


psinfo P[MAX]; //proc에 있는 모든 프로세스 저장
int P_SIZE = 0; //P 배열 크기
	
psinfo bash[MAX];//bash만 들어있음 터미널에서 돌아가는 프로세스는 그프로세스의 PPID가 bash PID 이다 
int bash_SIZE = 0;

psinfo wantkill[MAX]; //죽이고 싶은 프로세스 저장
int WK_SIZE = 0; 

int CK[MAX];  //checkTokill store ary
int CK_SIZE = 0; 

int lines, cols;
int curindex = 0;
int strsize = strlen("enter what you want to do?(q:exit, k:kill WANTKILL PROCESS, b:kill BASH PROCESS, e:enter exception pid, t:enter time)");

int start = 0;	//for문 시작 지점 -> 위아래 스크롤 구현
int map_num = 0;	//좌우 이동

int store_pid(); //store P and bash
void getCmdLine(char *file, char *buf, int size); //명령어 반환(comm)
void find_kill(psinfo*, psinfo*, int, int); //터미널에서 돌아가는 모든 프로세스
void get_display();	//프로세스 상태 받기 및 출력
void get_value();
void set_time_except(psinfo* ary, int size, int t);	//예외 처리할 시간 입력받기
void set_blank()
{	
	int i;
	for(i = 3; i > 0; i--)
	{
		move(lines - i, 0);
		printw(BLANK);
		refresh();
	}
}
   
void blank_all()
{
	for(int i = 2; i < lines - 3; i++)
	{
		move(i, 0);
		printw(BLANK);
	}
	refresh();
}

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) *1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void update_ps(int num)
{	
	signal(SIGALRM, SIG_IGN);
	get_value();
	get_display();
	move(lines - 3, strsize);
	refresh();
	signal(SIGALRM, update_ps);
}

int main(int argc, char* argv[])
{	
	
	int input;
	char input_temp;
	// int delay = 500; //0.5초마다 ps 갱신	
	int delay = 500; //0.5초마다 ps 갱신	
	
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);		//default
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);	//this pid
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA);	//TOTAL
    init_pair(4, COLOR_WHITE, COLOR_BLUE);		//COMMAND


    clear();
		
	move(0, 0);
	attron(COLOR_PAIR(4));
	printw("this terminal pid : %d", getppid()); //현재 프로세스가 실행되고 있는 터미널pid
	attron(COLOR_PAIR(1));
	refresh();
	
	signal(SIGALRM, update_ps);
	while(1){
	getmaxyx(stdscr,lines,cols); // 현재 띄워진 창의 행,열 크기 저장 
		




		//signal(SIGALRM, update_ps);
		set_ticker(delay);
		set_blank();
		move(lines - 3, 0);
		printw("enter what you want to do?(q:exit, k:kill WANTKILL PROCESS, b:kill BASH PROCESS, e:enter exception pid, t:enter time)");
		refresh();
		noecho();
		input = getch();


		if (input != 27) {
			switch(input){
				case 113:	//q
					move(lines - 2, 0);
					printw("Bye Bye.");
					refresh();
					sleep(1);
					endwin();
					return 0;
				case 107:	//k
					set_blank();
					move(lines - 3, 0);
					do_kill(wantkill, WK_SIZE);		//want_kill process 종료
					break;
				case 101:	//e
					echo();
					set_blank();
					move(lines - 3, 0);
					printw("enter exception pid");		//예외 프로세서 pid 입력받기
					refresh();
					get_pid(wantkill,  WK_SIZE, bash,  bash_SIZE, lines);
				
					store_CK(wantkill, WK_SIZE, CK, &CK_SIZE);
					store_CK(bash, bash_SIZE, CK, &CK_SIZE);
					set_CK(P, P_SIZE, CK, CK_SIZE);
					break;
				case 98:	//b
					echo();
					signal(SIGALRM, SIG_IGN);
					set_blank();
					move(lines - 3, 0);
					printw("If you kill bash, that can effect to other linux users of system. Do you really want to kill bash?(Y/N)");
					refresh();
					scanw("%c", &input_temp);
					refresh();
					move(lines - 2, 0);
					if(input_temp == 'Y' || input_temp == 'y')
						do_must_kill(bash, bash_SIZE);	//bash 프로세서 종료
					signal(SIGALRM, update_ps);
					break;
				case 116:	//t
					echo();
					signal(SIGALRM, SIG_IGN);
					set_blank();
					move(lines - 3, 0);
					printw("enter time to not kill(exit : -1)");		//일정시간 입력받기
					refresh();
					int time;
					scanw("%d", &time);
					refresh();
					if (time == -1)
						break;
					set_time_except(wantkill, WK_SIZE, time);
					set_time_except(bash, bash_SIZE, time);

					store_CK(wantkill, WK_SIZE, CK, &CK_SIZE);
					store_CK(bash, bash_SIZE, CK, &CK_SIZE);
					set_CK(P, P_SIZE, CK, CK_SIZE);

					signal(SIGALRM, update_ps);
					break;
				case 410:
					break;
				default:
					set_blank();
					move(lines - 3, 0);
					printw("you type wrong option please type it right.");
					refresh();
					sleep(1);
					break;
				}
		} else {
			input = getch();
			input = getch();
			switch (input) {
				case 65:	//up
					if (start > 10)
						start -= 10;
					else
						start = 0;
					break;
				case 66:	//down
					if ((map_num % 3) == 0 && start < P_SIZE - lines + 15)
						start += 10;
					else if ((map_num % 3) == 1 && start < bash_SIZE - lines + 15)
						start += 10;
					else if ((map_num % 3) == 2 && start < WK_SIZE - lines + 15)
						start += 10;
					break;
				case 68:	//left
					map_num--;
					start = 0;
					break;
				case 67:	//right
					map_num++;
					start = 0;
					break;
				default:
					move(lines - 3, 0);
					printw("you type wrong option please type it right..!");
					refresh();
					sleep(1);
					break;
			}
		}
	}
}

int store_pid() 
{
	DIR *dir = NULL;                     
    struct dirent *entry = NULL;       
    struct stat fileStat;         

    int pid;                        
    char cmdLine[500] = "\0";
    char tempPath[500] = "\0";
    int i = 0;

    dir = opendir("/proc");
//    memset(cmdLine, 0, sizeof(cmdLine));

    

    while ((entry = readdir(dir)) != NULL) 
    {   
		lstat(entry->d_name, &fileStat); 

		 // set_blank();
		 // move(lines - 3, 0);
		 // printw("test%d", i);
		 // refresh();
	//	sleep(1);                                          
                                                            
		if(S_ISDIR(fileStat.st_mode))
		{
			pid = atoi(entry->d_name);          
       
       	                    
			if(pid > 0)
			{
				P[i++].pid = pid;
	
				sprintf(tempPath, "/proc/%d/cmdline", pid); 
        		getCmdLine(tempPath, cmdLine, sizeof(cmdLine));
        
        		if(!strcmp(cmdLine, "bash"))
        		{
        			bash[bash_SIZE++].pid = pid;
        		}
        	
			}
		}
    }
//   	free(cmdLine);
    rewinddir(dir);
    closedir(dir);
    
    return i;
}

void getCmdLine(char *file, char *buf, int size) 
{
    FILE *srcFp;
    srcFp = fopen(file, "r");      

    memset(buf, 0, sizeof(size));
    fgets(buf, 500, srcFp);
    fclose(srcFp);
//    free(buf);
}


void find_kill(psinfo* ary1, psinfo* ary2, int size1, int size2)
{
	int pid, ppid;
	
	for(int i = 0;  i < size2; i++)
	{
		pid = ary2[i].pid;

		for(int j = 0; j < size1; j++)
		{
			ppid = ary1[j].ppid;
			if(pid == ppid)
			{
				wantkill[WK_SIZE].pid = ary1[j].pid;

				strcpy(wantkill[WK_SIZE].comm, ary1[j].comm);
				wantkill[WK_SIZE].state = ary1[j].state;
				wantkill[WK_SIZE].ppid = ary1[j].ppid;
				wantkill[WK_SIZE].runningTime = ary1[j].runningTime;
				if(wantkill[WK_SIZE].pid == getpid()) // current process and it's bash
						wantkill[WK_SIZE].checkTokill = 1;
				wantkill[WK_SIZE++].start_time = ary1[j].start_time;
			//	fprintf(stderr, "%d\n", WK_SIZE);
			}
		}
	
	}
}


void get_display(){
	move(2, 0);
	attron(COLOR_PAIR(2));
	printw(" PID				      COMMAND STATE PPID   startTime runningTime check");
	
	refresh();
	move(1, 0);
	if ((map_num % 3) == 0){
		attron(COLOR_PAIR(3));
		printw("TOTAL : %d\t\tAll Process Table  ", P_SIZE);
		refresh();
	}
	else if ((map_num % 3) == 1){
		attron(COLOR_PAIR(3));
		printw("TOTAL : %d\t\tBash Process Table  ", bash_SIZE);
		refresh();
	}
	else if ((map_num % 3) == 2){
		attron(COLOR_PAIR(3));
		printw("TOTAL : %d\t\tUser Process Table  ", WK_SIZE);
		refresh();
	}
	refresh();
	attron(COLOR_PAIR(1));

	if ((map_num % 3) == 0)
		curindex = print_psinfo(P, P_SIZE, curindex, lines);
	else if ((map_num % 3) == 1)
		curindex = print_psinfo(bash, bash_SIZE, curindex, lines);
	else if ((map_num % 3) == 2)
		curindex = print_psinfo(wantkill, WK_SIZE, curindex, lines);
}

void get_value()
{
	reset_arry(P, &P_SIZE);
	reset_arry(bash, &bash_SIZE);
	reset_arry(wantkill, &WK_SIZE);

	P_SIZE = store_pid();	//store at bash
	
	for(int i = 0; i < P_SIZE; i++)
	{
		int pid = P[i].pid;
		store_psinfo(P, pid, i);
	}

	for(int i = 0; i < bash_SIZE; i++)
	{
		int pid = bash[i].pid;
		store_psinfo(bash, pid, i);
	}

	move(lines - 3, 0);
	
	find_kill(P, bash, P_SIZE, bash_SIZE);
	
	set_CK(P, P_SIZE, CK, CK_SIZE);
	set_CK(bash, bash_SIZE, CK, CK_SIZE);
	set_CK(wantkill, WK_SIZE, CK, CK_SIZE);


}

void set_time_except(psinfo* ary, int size, int t){
	for(int i = 0; i < size; i++)
	{
		if(ary[i].runningTime < t){
			move(lines - 2, 0);
			printw(BLANK);
			move(lines - 2, 0);
		    	ary[i].checkTokill = 1;
		    //	printw("pid(%d) is exception", ary[i].pid);
		    	printw("pid(%d) is exception = %d", ary[i].pid, ary[i].checkTokill);
		    	refresh();
		    	sleep(1); //1초마다 예외된 값 출력
		   
		}
	}
} 
