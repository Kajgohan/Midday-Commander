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
  int backGroundFlag;
  int runningFlag;
  int pid;
  int multipleProcesses;

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
  printf("  r. running processes :Print List of Running Processes\n");
  printf("Option?: ");
}

void printRunningBackgroundProcesses(){
  struct Node *iterator = head;
    while(iterator != NULL){
      if((iterator->runningFlag) && (iterator->backGroundFlag)){

        for(int x = 0; x <= iterator->multipleProcesses; x++){
          printf("  %d. %s Running on process %d\n------------------------------------------------------------------------------------\n", iterator->key, iterator->command, iterator->pid);
      }
      }
      iterator = iterator->next;
    }



}

//Checks if the command entered is an available command in the linked list
//uses the key to search the nodes
int scanAddedCommands(int inputChar){
  struct Node * scan = head;
  if (head == NULL){
    return(0);
  }
  else{
    while(scan != NULL){
      if (scan->key == inputChar){
        return(1);
      }
      else{
        scan = scan->next;
      }
    }
    return(0);
  }
}

void addRunningFlag(inputChar){
  struct Node * execute = head;
    while(execute->key != inputChar){
      execute = execute->next;
    }

    size_t len = strlen(execute->command);
    if (execute->command[len-2] == '&'){
      if(execute->runningFlag == 1){
        execute->multipleProcesses = execute->multipleProcesses +1;
      }
      execute->runningFlag = 1;
    }
}
//Once the command is confirmed to be in the lsit, this methhod executes the command.
void executeCommand(int inputChar){
  //this adds a running tag to the linked list
  struct Node * execute = head;
  int counter = 0;
  char ** argvList = malloc(sizeof (char)*128*32);
  while(execute->key != inputChar){
    execute = execute->next;
  }

//assigns this process id to this node
    execute->pid = getpid();

    argvList[counter] = strtok(execute->command, " \n&");
    while(argvList[counter] != NULL){
      counter+=1;
      argvList[counter] = strtok(NULL, " \n&");
    }
    counter+=1;
    argvList[counter] = NULL;

    execvp(argvList[0], argvList);
}

void addCommand(int commandID){
    struct Node* newCommand = (struct Node *)malloc(sizeof(struct Node));
    newCommand->command = malloc(sizeof(char)*128);
    newCommand->next = NULL;
    printf("--Add A Command--\n");
    printf("Command To Add?: ");
    fgets(newCommand->command, 128, stdin);
    //adds terminating string so that during a print f function the printf knows when to stop and there is no seg fault
    newCommand->command[127] = '\0';
    if (head == NULL){
        newCommand->key = commandID;
        newCommand->next = NULL;
        newCommand->multipleProcesses = 0;
        newCommand->runningFlag = 0;
        head = newCommand;
        tail = newCommand;
    }
    else{
        newCommand->key = commandID;
        newCommand->next = NULL;
        newCommand->multipleProcesses = 0;
        newCommand->runningFlag = 0;
        tail->next = newCommand;
        tail = newCommand;
  }
  //This part of the add command checks for the & at the end of the string
  //if there is an and it lets the data struct know by making the backGroundFlag true
  size_t len = strlen(tail->command);
  printf("%c", tail->command[len-3]);
  if (tail->command[len-2] == '&'){
    tail->backGroundFlag = 1;
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
  printf("Elapsed Time: %10.2f Milliseconds\n", time);
  printf("Page Faults: %ld\n", majflt);
  printf("Reclaimed Page Faults: %ld\n------------------------------------------------------------------------------\n", minflt);
}

int backgroundFlagScan(int inputChar){
  struct Node * scan = head;

    while(scan != NULL){
      if (scan->key == inputChar && scan->backGroundFlag){
        return(1);
      }
      else if (scan->key == inputChar && !scan->backGroundFlag){
        return(0);
      }
      else{
        scan = scan->next;
      }
  }
  return(0);
}

int waitCheck(){
  struct  rusage usage;
  struct Node * scan = head;
  int pid = wait3(NULL, WNOHANG, &usage);
  if (pid != -1 && pid != 0){
      printf("--Background Process %d Statistics--\n", pid);
      printf("Page Faults: %ld\n", usage.ru_majflt);
      printf("Reclaimed Page Faults: %ld\n------------------------------------------------------------------------------\n", usage.ru_minflt);
        while(scan != NULL){
          if ((scan->pid == pid) && (scan->multipleProcesses == 0)){
            scan->runningFlag = 0;
            return(1);

          }
          else if((scan->pid = pid) && (scan->multipleProcesses != 0)){
            scan->multipleProcesses = (scan->multipleProcesses) -1;
            printf("%s Finished Running", scan->command);
            return(1);
          }
          else{
            scan = scan->next;
          }
      }
  }
  return(1);

  }

int main(){
  char * num = malloc(sizeof(char)* 128);
  char currentDirectory[128];
  char  * arg = malloc(sizeof (char)* 128);
  char  * path = malloc(sizeof (char)* 128);
  char ** argvList = malloc(sizeof (char)*128*32);
  int inputChar;
  int forkId;
  int statisticsFlag = 1;
  int key = 4;
  int long majfltOld;
  int long minfltOld;
  struct rusage usage;
  struct timeval* start = malloc(sizeof(struct timeval));
  struct timeval* end = malloc(sizeof(struct timeval));
  int runChild = 1;
  char *arList [2]  = {"whoami", NULL};
  char *argList [2] = {"last", NULL};


  majfltOld = 0;
  minfltOld = 0;
  printf("==== Mid-Day Commander, v0 ==== \n");
  //overall loop to keep the program prompting the user for another command
  while(1){

    //statistics flag is default at 1 so that all foreground processes print their statistics
   type_prompt();
   runChild = 1;
   //how I recieve the initial command
   if(feof(stdin)){
     exit(0);
   }
   fgets(num, 128, stdin);
   if(feof(stdin)){
     exit(0);
   }
   num = strtok(num, "\n");

   if(feof(stdin)){
     exit(0);
   }

   //conditional parses the input to an int, ascii for chars and int values for ints
   if(atoi(num) == 0){
     inputChar = (int)num[0];
   }
   else{
     inputChar = atoi(num);
   }

   //have to set the statisticsFlag before the fork.
   statisticsFlag = 1;
   //Check the key against the list too see if there is a flag
   //if there is a flag, warn the parent about not printing statistics or waiting
   if(scanAddedCommands(inputChar)){
     if(backgroundFlagScan(inputChar)){
        statisticsFlag = 0;
     }
   }

   //start time for the non-background processes
   //check to print statistics for finished background processes
   if(waitCheck()){
     //printf("test");
   }

   //num = NULL;

   //this is where the parent gets a child processes
   //use the return value to seperate actions I want the parent to do and actions I want the Child to do

   switch(inputChar)
   {
     case 1:
           printf("\n--Who Am I?--\n");

           break;
     case 2:
           printf("\n--Last Logins--\n");
           break;
     case 3:
           printf("\n--Directory Listings--\n");
           argvList[0] = "ls";
           printf("Arguments?: ");
           fgets(arg, 128, stdin);
           //begins the arg parsing
           argvList[1] = strtok(arg, " \n");
           int counter = 1;
           while(argvList[counter] != NULL){
             counter+=1;
             argvList[counter] = strtok(NULL, " \n");
           }
           printf("\nPath?: ");
           fgets(path, 128, stdin);
           path = strtok(path, "\n");
           //adds path to the argument
           argvList[counter] = path;
           counter+=1;
           argvList[counter] = NULL;
           break;
           //the following cases just make sure that the commands that need to run on the parent process do not hit the default case
           case 'a':
               addCommand(key);
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
     //need an r case than calls a function to see the running background functions
           case 'r':
             printf("\n--Background Processes--\n");
             printRunningBackgroundProcesses();
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
     else{
       addRunningFlag(inputChar);
     }
   }
     gettimeofday(start);

   forkId = fork();
    if((forkId == 0) && (runChild)){
      //printf("%d\n", inputChar);
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
      }
        exit(0);
    }

        //need an conditional, if the command has a & dont execute statistics or wait
        if(statisticsFlag){
          wait(NULL);
          //have to turn off the running flag when the child process of that command ends
          gettimeofday(end);
          //converting the time to miliseconds
          double time_t = (double) (((double)(end->tv_sec * 1000.00)+(double)(end->tv_usec)/1000.00)) - ((double)(start->tv_sec * 1000.00)+((double)(start->tv_usec)/1000.00));
          statistics(time_t, majfltOld, minfltOld);
          getrusage(RUSAGE_CHILDREN, &usage);
        }
        //update usage values so that the next call is just the data from that call

        majfltOld = usage.ru_majflt;
        minfltOld = usage.ru_minflt;

    }
    return 0;
  }
