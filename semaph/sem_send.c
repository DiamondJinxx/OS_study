#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include "sem_defs.h"

int main(int argc, char* argv[])
{
  int shmid;			/* id сегмента данных */
  char *str;			/* передаваемая строка */
  int semid;			/* id набора семафоров */
  struct sembuf semoper[1];	/* Используется semop() */


  /* Получение доступа к разделяемой памяти */
  if( (shmid = shmget(SEG_KEY, MAXSTRLEN + 1, 0600)) < 0 ){
    printf("Ошибка shmget()!\n");
    return 1;
  }
  
  /* Подключение сегмента разделяемой памяти */
  if( (str = (char*)shmat(shmid, NULL, 0)) == (char*)(-1) )
  {
    printf("Ошибка подключения разделяемой памяти");
    return 1;
  }
  
  /* Получение доступа к набору семафоров */
  if( (semid = semget(SEM_KEY, 1, 0600)) < 0 )
  {
      printf("Ошибка доступа к набору семафоров!\n");
      return 2;
  }
  /* получение монопольного доступа к разделяемым данным */
  /* Р - операция - захват доступа */
  semoper[0].sem_num = 0;	/* номер семафора в наборе */
  semoper[0].sem_op = -1;
  semoper[0].sem_flg = 0;
  semop(semid, semoper, 1);	/* выполненеи операции */
  /* Ввод строки с клавиатуры */
  printf("Введите строку:");
  fgets(str, MAXSTRLEN, stdin);

  /* СОвобождаем ресурс */
  /* V - операция */
  semoper[0].sem_num = 0;
  semoper[0].sem_op = 1;
  semoper[0].sem_flg = 0;
  semop(semid, semoper, 1);			/* Выполнение операции */

  /* Отключение сегмента разделяемой памяти */
  shmdt(shmid);
  return 0;
}
