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
#include "dokill.h"

#define MAX 1000



psinfo P[MAX]; //proc에 있는 모든 프로세스 저장
int P_SIZE = 0; //P 배열 크기
	
psinfo bash[30];//bash만 들어있음 터미널에서 돌아가는 프로세스는 그프로세스의 PPID가 bash PID 이다 
int bash_SIZE = 0;

psinfo wantkill[MAX]; //죽이고 싶은 프로세스 저장
int WK_SIZE = 0; 


int store_pid(); //store P and bash
int getCmdLine(char *file, char *buf); //명령어 반환(comm)
void find_kill(psinfo*, psinfo*, int, int); //터미널에서 돌아가는 모든 프로세스
void get_display();	//프로세스 상태 받기 및 출력

int main(int argc, char* argv[])
{	
	char input;
	char input_temp;
	
	printf("this terminal pid : %d\n", getppid()); //현재 프로세스가 실행되고 있는 터미널pid
	
	get_display();

	while(1){
		printf("enter what you want to do?(q:exit, k:kill WANTKILL PROCESS, b:kill BASH PROCESS, e:enter exception pid, t:enter time, p: display status again)");
		scanf(" %c", &input);\
		printf("input is %c\n", input);
		switch(input){
			case 'q':
				exit(0);
			case 'k':
				do_kill(wantkill, WK_SIZE);		//want_kill process 종료
				break;
			case 'e':
				printf("enter 예외 pid\n");		//예외 프로세서 pid 입력받기
				get_pid(P, P_SIZE);
				break;
			case 'b':
				printf("If you kill bash, that can effect to other linux users of system. Do you really want to kill bash?(Y/N)");
				scanf(" %c", &input_temp);
				if(input_temp == 'Y' || input_temp == 'y')
					do_must_kill(bash, bash_SIZE);	//bash 프로세서 종료
				break;
			case 't':
				printf("get time to kill\n");		//일정시간 입력받기
				break;
			case 'p':									//프로세스 상태 재출력
				get_display();
				break;
			default:
				printf("you type wrong option please type it right.\n");
				break;
		}
	}
	
	
	

	
}
int store_pid() 
{
	DIR *dir;                     
    struct dirent *entry;       
    struct stat fileStat;         

    int pid;                        
    char cmdLine[256];
    char tempPath[256];
    int i = 0;

    dir = opendir("/proc");   

    while ((entry = readdir(dir)) != NULL) 
    {   
	lstat(entry->d_name, &fileStat);          
  
	if(!S_ISDIR(fileStat.st_mode))         
		continue;                                  
                                                            

       pid = atoi(entry->d_name);          
       if (pid <= 0) 
       	continue;                     

	P[i++].pid = pid;
	
	sprintf(tempPath, "/proc/%d/cmdline", pid); 
        getCmdLine(tempPath, cmdLine);
        
        if(!strcmp(cmdLine, "bash"))
        {
        	bash[bash_SIZE++].pid = pid;
        }   
	
	
    }
    closedir(dir);
    
    return i;
}

int getCmdLine(char *file, char *buf) 
{
    FILE *srcFp;
    srcFp = fopen(file, "r");          

    memset(buf, 0, sizeof(buf));
    fgets(buf, 256, srcFp);
    fclose(srcFp);
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
				wantkill[WK_SIZE].checkTokill = 1;
				wantkill[WK_SIZE++].start_time = ary1[j].start_time;
			
			}
		}
	
	}
	
	
}



void get_display(){
	P_SIZE = store_pid();
	
	for(int i = 0; i < P_SIZE; i++)
	{
		int pid = P[i].pid;
		store_psinfo(P, pid, i);
	}
	
	for(int i = 0; i < bash_SIZE; i++)
	{
		int pid = bash[i].pid;
		store_psinfo(bash, pid, i);
		bash[i].checkTokill = 1;
	}
	
	printf("==============================ALL PROCESS==============================\n");
	print_psinfo(P, P_SIZE);
	printf("==============================BASH PROCESS==============================\n");
	print_psinfo(bash, bash_SIZE);
	
	find_kill(P, bash, P_SIZE, bash_SIZE);
	
	printf("==============================RESET ARRY==============================\n");
	reset_arry(P, &P_SIZE);
	print_psinfo(P, P_SIZE);
	printf("%d\n", P_SIZE);
	
	printf("==============================WANTKILL PROCESS==============================\n");
	print_psinfo(wantkill, WK_SIZE);
	printf("================================================================================\n");
}


