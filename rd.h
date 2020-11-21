
void read_one(int *x, FILE* input);
void read_unsigned(unsigned long long *x, FILE* input);
void read_str(char *x, FILE* input);
void read_char(char *x, FILE* input);

int get_timesinceboot(long tickspersec);

void print_timediff(char *name, unsigned long long x, long tickspersec);



typedef struct psinfo
{
	int pid;
	char comm[255];
	char state;
	int ppid;
	unsigned long long start_time;

}psinfo;



void print_psinfo(psinfo* ary, int size); //출력
void reset_arry(psinfo* ary, int* size); //reset arry
void store_psinfo(psinfo* ary, int, int); //pid를 통한 ps 정보 저장


