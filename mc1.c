#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

struct Node{

  int key;
  char *command;
  struct Node *next;
};

struct Node* head = NULL;
struct Node* tail = NULL;

//Iterates through the Linked List and prints for the main menu
void printAddedPrompts(){
  struct Node *iterator = head;
  if (head != NULL){
    while(iterator != NULL){
      printf("  %d. %s", iterator->key, iterator->command);
      iterator = iterator->next;
    }
  }
}

//Checks if the command entered is an available command in the linked list
int scanAddedCommands(int inputChar){
  struct Node * scan = head;
  //printf("scanner runs\n");
  if (head == NULL){
    return(0);
  }
  else{
    while(scan != NULL){
      if (scan->key == inputChar){
        printf("%d\n", scan->key);
        return(1);
      }
      else{
        scan = scan->next;
      }
    }
    return(0);
  }

}
//Once the command is confirmed to be in the lsit, this methhod executes the command.
void executeCommand(int inputChar){
  int counter = 0;
  //printf("we out here\n");
  char ** argvList = malloc(sizeof (char)*128*32);
  struct Node * execute = head;
    while(execute->key != inputChar){
      execute = execute->next;
    }
    argvList[counter] = strtok(execute->command, " \n");
    while(argvList[counter] != NULL){
      counter+=1;
      argvList[counter] = strtok(NULL, " \n");
    }
    counter+=1;
    argvList[counter] = NULL;
    execvp(argvList[0], argvList);
}
void type_prompt(){

  printf("G'day, Commander! What command would you like to run? \n");
  printf("  1. whoami   :Prints out the result of the whoami command \n");
  printf("  2. last     :Prints out the result of the last command\n");
  printf("  3. ls       :Prints out the result of a listing on a user-specified path\n");
  printAddedPrompts();
  printf("  a. Add Command :Adds New Command to the menu\n");
  printf("  c. Change Directory :Changes Process Working Directory\n");
  printf("  e. Exit :Leave Mid-Day Commander\n");
  printf("  p. pwd :Prints Working Directory\n");
  printf("Option?: ");
}
int addCommand(int commandID){

    struct Node* newCommand = (struct Node *)malloc(sizeof(struct Node));
    newCommand->command = malloc(sizeof(char)*128);
    newCommand->next = NULL;


    printf("--Add A Command--\n");
    printf("Command To Add?: ");
    fgets(newCommand->command, 128, stdin);
    newCommand->command[127] = '\0';
    if (head == NULL){
        newCommand->key = commandID;
        //newCommand->command = commandNew;
        newCommand->next = NULL;
        //printf("%s", newCommand->command);
        head = newCommand;
        tail = newCommand;
        return(1);
    }
    else{
        //newCommand->command = commandNew;
        newCommand->key = commandID;
        newCommand->next = NULL;
        tail->next = newCommand;
        tail = newCommand;
        return(1);
  }
}


void statistics(double time, int long majfltOld, int long minfltOld){
  int long majflt;
  int long minflt;
  struct rusage usage;
  getrusage(RUSAGE_CHILDREN, &usage);
  majflt = usage.ru_majflt - majfltOld;
  minflt = usage.ru_minflt - minfltOld;
  printf("\n--Statistics--\n");
  printf("Elapsed Time: %4.2f Milliseconds\n", time);
  printf("Page Faults: %ld\n", majflt);
  printf("Reclaimed Page Faults: %ld\n------------------------------------------------------------------------------\n", minflt);

}

int main(){
  char * num = malloc(sizeof(char)* 128);
  char currentDirectory[128];
  char  * arg = malloc(sizeof (char)* 128);
  char  * path = malloc(sizeof (char)* 128);
  char ** argvList = malloc(sizeof (char)*128*32);
  int inputChar;
  int forkId;
  int key = 4;
  int long majfltOld;
  int long minfltOld;
  struct rusage usage;
  struct timeval* start = malloc(sizeof(struct timeval));
  struct timeval* end = malloc(sizeof(struct timeval));
  char *argList [2] = {"last", NULL};
  char *arList [2]  = {"whoami", NULL};
  int runChild = 1;



  majfltOld = 0;
  minfltOld = 0;

  printf("==== Mid-Day Commander, v0 ==== \n");
  while(1){
    runChild = 1;
   type_prompt();

   fgets(num, 128, stdin);
   num = strtok(num, "\n");
   if(feof(stdin)){
     exit(0);
   }
   if(atoi(num) == 0){
     inputChar = (int)num[0];
   }
   else{
     inputChar = atoi(num);
   }

   switch(inputChar)
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
           fgets(path, 128, stdin);
           path = strtok(path, "\n");
           argvList[counter] = path;
           counter+=1;
           argvList[counter] = NULL;
           //printf("|%d|\n", forkId);
           break;
           case 'a':
               if(addCommand(key)){
                 //printf("commandAdded\n");
               }
               key+=1;
               break;
           case 'c':
             printf("\n--Change Directory--\n");
             printf("New Directory?:");
             fgets(path, 128, stdin);
             if(chdir(strtok(path, " \n"))){
               printf("Invalid Path\n");
             }
             break;
           case'e':
             printf("Logging you out Commander.\n");
             exit(0);
             break;
           case'p':
             printf("\n--Current Directory--\n");
             getcwd(currentDirectory, sizeof(currentDirectory));
             printf("Directory: %s", currentDirectory);
             break;
           default:
           if(scanAddedCommands(inputChar) == 0){
             runChild = 0;
           }
             break;
   }
   gettimeofday(start);
 forkId = fork();
 if((forkId == 0) && (runChild)){
     switch(inputChar){
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
       if(scanAddedCommands(inputChar)){
         executeCommand(inputChar);
     }
     break;
   }
     exit(0);
 }

      //printf("%ld", start_c);
      wait(0);
      gettimeofday(end);
      //printf("%ld", end_c);
      double time_t = (double) (((double)(end->tv_sec * 1000.00)+(double)(end->tv_usec)/1000.00)) - ((double)(start->tv_sec * 1000.00)+((double)(start->tv_usec)/1000.00));
      statistics(time_t, majfltOld, minfltOld);
      getrusage(RUSAGE_CHILDREN, &usage);
      majfltOld = usage.ru_majflt;
      minfltOld = usage.ru_minflt;
    }
    return 0;
  }
