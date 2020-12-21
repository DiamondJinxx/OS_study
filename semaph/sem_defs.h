/* ключ разделяемой памяти */
#define SEG_KEY 0x12345678

/* КЛюч набора семафоров */
#define SEM_KEY 0x12345678

/* Максимальная длина перемдаваемой строки */
#define  MAXSTRLEN 100

/* union для вызова semctl() */
union semun{
  int val;
};

