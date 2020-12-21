#include <stdio.h>
#include <unistd.h>

#define MAXLEN 100


int main(int argc, char* argv[])
{
  int fd[2];
  int proc, len;
  char s[MAXLEN];

  /* Создаем канал */

  if( pipe(fd) < 0) {printf("Ошибка pipe()!\n"); return 1;}
  /* СОзданный канал наследуется обоими процессамм: родительским и дочерним */
  if((proc = fork()) < 0){
    printf("Ошибка fork(),");
    return 2;
  }
  
  if ( proc == 0)		/* дочерний класс - писатель */
  {
    close(fd[0]);		/* закрываем канал для чтения */
    printf("Введиет строку:");
    if(fgets(s, MAXLEN, stdin) != NULL)
    {
      len = strlen(s) + 1;	/* длина строки + '\0' */
      /* запись s в канал */
      if(write(fd[1], s, len) != len)
      {
	printf("Ошибка записи в канал!\n");
	return 3;
      }
    }
    return 0; 			/* конец дочернего процесса */
  }
  else				/* родительский процесс */
  {
    close(fd[1]);		/* закрыли канал для записи */
    if((len = read(fd[0], s, MAXLEN)) < 0)
    {
      printf("Ошибка чтения из канала!\n");
      return 4;
    }
    if(len)
      printf("Строка = %s\n", s);
    else
      printf("Конец файла!\n");
    return 0;			/* конец родительского процесса */
  }

  return 0;
}
