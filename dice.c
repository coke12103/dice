#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>

int get_rand(int count);

int main(int argc, char **argv){
  int result = 0;
  int count = atoi(argv[1]);
  printf("1d%d\n", count);

  printf("ARGV is %d count\n", argc-1);

  if(argc > 1){
    for(int i = 1; i<argc;++i){
      printf("%d: ", i);
      printf("%s\n", argv[i]);
    }
  }


  result = get_rand(count);

  printf("1d%d", count);
  printf(": %d", result);

  return 0;
}

int get_rand(int count){
  srand(time(NULL));

  return rand()%count+1;
}
