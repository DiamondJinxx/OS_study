#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  int code;

  printf("Введите код (0..255)\n");
  scanf("%d", &code);
  exit(code);
}
