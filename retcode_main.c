#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>



int main(int argc, char *argv[])
{
  int proc, code;
  if( (proc = fork()) < 0 ){ printf("Ошибка fork()\n"); return 1;}
  if( proc == 0)		/* в дочернем процессе запустим retcode */
  {
    execl("retcode", "retcode", NULL);
    printf("Ошибка запуска 'retcode'\n");
    return 123;			/* код завершения родительского процесса */
  }
  else				/*Родительский процесс */
  {
    wait(&code);
    if(WIFEXITED(code))
      printf("Дочерний процесс завершен с кодом=%d\n",WEXITSTATUS(code));
    else if(WIFSIGNALED(code))
      printf("Дочерний процесс завершен сигналом %d\n", WTERMSIG(code));
    return 0;

  }
  
}
