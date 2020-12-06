# SystemPrograming_Team9
## 팀 멤버
팀장: 오강산(rightkingleftking)   
팀원: 김대영(KIM970509), 박재우(Bread Park, Jawoos)
## 프로젝트 주제
프로세스 킬을 간편하게 만들자
## 주요기능
현재 실행중인 프로세스들 정보 표시 및 간편한 킬
## 사용한 라이브러리
- #include <stdio.h>
- #include <string.h>
- #include <stdlib.h>
- #include <unistd.h>
- #include <dirent.h>
- #include <sys/stat.h>
- #include <sys/types.h>
- #include <time.h>
- #include <linux/limits.h>
- #include <sys/times.h>
- #include <ncurses.h>
- #include "rd.h"
## 사용 방법
sudo apt-get install libncurses5-dev libncursesw5-dev 를 통해 라이브러리를 다운로드하고 make를 통해 실행파일을 만들어준다.

프로그램 실행 후 사용법은 다음과 같다.

- q: 프로그램 종료
- k: 현재 실행중인 bash 및 시스템 프로세스 제외한 나머지 프로세스 종료
- b: 현재 실행중인 bash 프로세스 종료
- e: 죽이지 않을 프로세스 pid 입력 받기
  * -1 입력시 종료
- t: 특정 실행 시간 미만 프로세스 살리기
  * 특정 시간 입력시 그 시간 이하로 실행된 프로세스는 예외 처리됨
- 방향키: 이동하는데 사용됨
  * 위, 아래 방향키: 표시할 정보가 많을때 위아래 스크롤 같이 사용된다
  * 좌, 우 방향키: 테이블을 옮겨 다닐수 있있다
## 페이지 구성
- All Process Table: 실행중인 모든 프로세스를 표시해준다
- Bash Process Table: 실행중인 Bash 프로세스를 표시해준다
- User Process Table: 실행중인 user 프로세스를 표시해준다
## 소스코드 구성
- main.c: 메인 함수와, 시그널 함수, 예외시간 입력받기, 415lines
- rd.c: 프로세스 정보 파일시스템에서 받아오기, 150lines
- dokill.c: 프로세스 킬, 116lines
