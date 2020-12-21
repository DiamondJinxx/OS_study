#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include "sem_defs.h"

int main(int argc, char* argv[])
{
  int shmid;			/* ID сегмента разделяемой памяти */
  char *str; 			/* УКазатель на принимаемую строку */
  int semid;			/* ID набора семафоров */
  int retcode  = 1;		/* КОд возврата из программы */
  union semun semcd;		/* Используется semctl() */
  struct sembuf semoper[1];	/* используется semop() */
  int exitflag = 1;

  /* Создание сгемента разделяемой памяти */
  if((shmid = shmget(SEG_KEY, MAXSTRLEN + 1, IPC_CREAT | IPC_EXCL | 0600)) < 0){
    printf("Ошибка shmget()!\n"); return 1;
  }

  /* подключение сегмента резделяемой памяти */

  if( (str = (char * )shmat(shmid, NULL, 0)) == (char*)(-1) )
  {
    printf("Ошибка доступа к разделяемой памяти shmat()\n");
    goto destroy_segment;
  }

  /* Создание набоа семафоров */
  if((semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0600)) < 0)
  {
    printf("Ошибка semget()!\n");
    goto destroy_segment;
  }

  /* Установка начального значения семафора, равного 1 */
  semcd.val = 1;
  if(semctl(semid, 0, SETVAL, semcd) < 0){
      printf("Ошибка semctl()!\n");
      goto destroy_semaphore;
  }
  /* Цикл приема строк через разделяемую память */
  while(exitflag)
  {
    /* обеспечиваем моопольный доступ к ресурсу - разделяемой памяти */
    /* P-операция - занимаем ресурс */
    semoper[0].sem_num = 0;	/* номер семафора в наборе */
    semoper[0].sem_op = -1;	/* операция */
    semoper[0].sem_flg = 0;
    semop(semid, semoper, 1);			/* Выполнение операции */

    if(str[0])			/* Если имеется строка */
    {
      printf("Приянта строка : %s\n", str);
      /* Проверка - передана ли команда ВЫХОДА? */
      if(strncmp(str, "EXIT", 4) == 0) /* если строки идентичны - получили команду выхода */
      {
	exitflag = 0;
	printf("ВЫХОД.\n");
      }

      /* СБрасываем признак наличия строки */
      str[0] = 0;
    }
    /* СОвобождаем ресурс */
    /* V - операция */
    semoper[0].sem_num = 0;
    semoper[0].sem_op = 1;
    semoper[0].sem_flg = 0;
    semop(semid, semoper, 1);			/* Выполнение операции */
  }						/* конец цикла приема строк */
  
  /* Нормальное завершение */
  /* Отсоединение сегмента */
  shmdt(str);
  retcode = 0;


 destroy_semaphore:		/* уничтожение семафоров */
  semctl(semid, IPC_RMID, 0);
  
 destroy_segment:
  shmctl(shmid, IPC_RMID, NULL);
  return retcode;
}
