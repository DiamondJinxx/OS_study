
#include <unistd.h>
#include <stdio.h>

void main(int argc, char *argv[])
{
  int i, proc;
  
  if( (proc = fork()) < 0 ){ printf("Ошибка fork()\n");}
  else
    if( proc != 0)		/* родительский процесс */
    {
      for(i = 0; i < 2; i++){
	printf("Родителький процесс, PID = %d\n", getpid());sleep(1);
	wait();
      }
    }
    else			/* если процесс дочерний */
    {
      for(i = 0; i < 5; i++){
	printf("Дочерний процесс, PID = %d\n", getpid());sleep(1);
      }
    }
}
