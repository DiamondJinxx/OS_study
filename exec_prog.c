
#include <unistd.h>
#include <stdio.h>

void main(int argc, char *argv[])
{
/*
  char *filepath = "/bin/cat";
  char * args[] = {filepath, "--help", NULL};
  execve(filepath, args, __environ);
*/
  printf("Запускаем ls...\n");
  execl("/bin/ls", "/bin/ls", "-l", "/", NULL);
  printf("Ошибка exec()!\n"); exit(1);
}
