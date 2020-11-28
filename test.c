/* main.c에서 전체프로세스의  다시 출력이 안되는 이유 : 
   opendir을 할 때 같은 경로를 줘도 특정 조건에서 다른 값이 받아짐
   그 조건을 아직  모르겠음
   조건 테스트용 파일
   */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void tt(int i){
    DIR* dp = NULL;

        dp = opendir("/proc");
	closedir(dp);
	printf("\n%d %d\n",dp,i);
 
}

int main(){
    DIR* dp = NULL;
    tt(1);
    tt(2);
    tt(3);
    dp = opendir("/proc");
    printf("\n%d 4\n",dp);
    dp = NULL;
    tt(5);
    tt(6);
    return 0;
}
