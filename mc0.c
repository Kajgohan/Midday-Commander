#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

void type_prompt(){

  printf("G'day, Commander! What command would you like to run? \n");
  printf("  1. whoami   :Prints out the result of the whoami command \n");
  printf("  2. last     :Prints out the result of the last command\n");
  printf("  3. ls       :Prints out the result of a listing on a user-specified path\n");
  printf("Option?: ");
}

void statistics(double time, int long majfltOld, int long minfltOld){
  int long majflt;
  int long minflt;
  struct rusage usage;
  getrusage(RUSAGE_CHILDREN, &usage);
  majflt = usage.ru_majflt - majfltOld;
  minflt = usage.ru_minflt - minfltOld;
  printf("\n--Statistics--\n");
  printf("Elapsed Time: %10.2f Milliseconds\n", time);
  printf("Page Faults: %ld\n", majflt);
  printf("Reclaimed Page Faults: %ld\n\n", minflt);

}

int main(){
  char * num = malloc(sizeof(char)* 128);
  char  * arg = malloc(sizeof (char)* 128);
  char  * path = malloc(sizeof (char)* 128);
  char ** argvList = malloc(sizeof (char)*128*32);
  //char **argPointer = argvList;
  int forkId;
  int intNum;
  int long majfltOld;
  int long minfltOld;
  struct rusage usage;
  struct timeval* start = malloc(sizeof(struct timeval));
  struct timeval* end = malloc(sizeof(struct timeval));
  int statsFlag = 1;
  char *arList [2]  = {"whoami", NULL};
  char *argList [2] = {"last", NULL};



  majfltOld = 0;
  minfltOld = 0;

  printf("==== Mid-Day Commander, v0 ==== \n");
  while(1){
    statsFlag = 1;
    type_prompt();

   fgets(num, 10, stdin);
   num = strtok(num, "\n");
   intNum = atoi(num);
   //printf("%s\n", num);
  if(feof(stdin)){
    exit(0);
  }

  switch(intNum)
  {
    case 1:
          printf("\n--Who Am I?--\n");
          //printf("|%d|\n", forkId);
          break;
    case 2:
          printf("\n--Last Logins--\n");
          //printf("|%d|\n", forkId);
          break;
    case 3:
          printf("\n--Directory Listings--\n");
          argvList[0] = "ls";
          printf("Arguments?: ");
          fgets(arg, 128, stdin);
          argvList[1] = strtok(arg, " \n");
          int counter = 1;
          while(argvList[counter] != NULL){
            counter+=1;
            argvList[counter] = strtok(NULL, " \n");
          }
          printf("\nPath?: ");
          fgets(path, 100, stdin);
          path = strtok(path, " \n");
          argvList[counter] = path;
          counter+=1;
          argvList[counter] = NULL;
          //printf("|%d|\n", forkId);
          break;
    default:
      printf("\nSorry, Invalid Command\n");
      statsFlag = 0;
          break;
  }
    gettimeofday(start);
   forkId = fork();
    if( forkId == 0){

          switch(intNum)
          {

            case 1:
            execvp("whoami", arList);
            break;
            case 2:
            execvp("last", argList);
            break;
            case 3:
            execvp("ls", argvList);
            break;
            default:
              break;
          }}
      wait(0);
      if(statsFlag){
        gettimeofday(end);
        //printf("%ld", end_c);
        double time_t = (double) (((double)(end->tv_sec * 1000.00)+(double)(end->tv_usec)/1000.00)) - ((double)(start->tv_sec * 1000.00)+((double)(start->tv_usec)/1000.00));
        statistics(time_t, majfltOld, minfltOld);
        getrusage(RUSAGE_CHILDREN, &usage);
    }
      majfltOld = usage.ru_majflt;
      minfltOld = usage.ru_minflt;

    //printf("|%d|\n", forkId);
  }
  return 0;
}
