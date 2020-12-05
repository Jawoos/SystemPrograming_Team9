void do_kill(psinfo* ary, int size);
void do_must_kill(psinfo* ary, int size);
void get_pid(psinfo* ary1, int size1, psinfo* ary2, int size2, int lines);
void store_CK(psinfo* ary, int size, int* CK, int* cksize);
void set_CK(psinfo* ary, int size, int* CK, int cksize);
