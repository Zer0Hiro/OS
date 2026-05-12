#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  // not enough arguments
  if(argc < 4) 
  {
    printf("ERROR\n");
    exit(1);
  }
  // get main number
  int num = atoi(argv[1]);

  // find some of numbers == num
  for(int i = 1; i < argc; i++)
  {
    for(int j = i+1; j < argc; j++)
    {
      if(atoi(argv[i])+atoi(argv[j])==num)
      {
        printf("2 numbers exist: %d+%d=%d\n",atoi(argv[i]),atoi(argv[j]),num);
        return 1;
      }

    }
  }
  printf("2 numbers do not exist\n");
  return 0;
}
