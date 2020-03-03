/**********************************************************************
** * Program: SmallSH
** * Author: Lake Peterson
** * Date: February 28, 2020
** * Description: Low detailed version of a BASH shell
** * Input: Takes in user input to complete bashic shell commands
** * Output: Returns the users requests based on command line input
**********************************************************************/

/**********************************************************************
** * Included Libraries
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

/**********************************************************************
** * Global Variables
**********************************************************************/

int foregroundTracker = 1;
int skip = 1;

/**********************************************************************
** * Prototypes
**********************************************************************/

void shell(char*);
void changeDirectory(char* [2048], int);
void currentStatus(int);
void catchSIGINT(int);
void catchSIGSTP(int);
char* wordToPID(int, const char*, const char*);

/**********************************************************************
** * Function: main
**********************************************************************/

int main(int argc, char const *argv[])
{
  char userInput[2048];

  shell(userInput);

  return 0;
}

/**********************************************************************
** * Function: shell
** * Description: Completes the basic operations of the shell
** * Parameters: char* userInput
** * Pre-Conditions: User input is necessary in order to complete operations
** * Post-Conditions: Returns the users requests based on command line input
**********************************************************************/

void shell(char* userInput)
{
  int valid = 1;
  int validInput;
  int validOutput;
  int indexCount;
  int status = 0;
  int i = 0;
  int processTracker = 0;
  int currentPID;
  char* parsedString;
  char* commandLine[2048];
  char cleanString[512];
  char commentCheck[100];
  char* pidCheck;
  pid_t spawnPID = -5;
  struct sigaction SIGINT_action = {0};                                         /*Taken entirely from our provided leacture videos*/
  struct sigaction SIGSTP_action = {0};

  SIGINT_action.sa_handler = catchSIGINT;                                       /*Taken entirely from our provided lecture videos*/
  sigfillset(&(SIGINT_action.sa_mask));
  SIGINT_action.sa_flags = 0;

  SIGSTP_action.sa_handler = catchSIGSTP;                                       /*Taken entirely from our provided lecture videos*/
  sigfillset(&(SIGSTP_action.sa_mask));
  SIGSTP_action.sa_flags = 0;

  sigaction(SIGINT, &SIGINT_action, NULL);                                      /*Taken entirely from our provided lecture videos*/
  sigaction(SIGTSTP, &SIGSTP_action, NULL);

  while(valid == 1)                                                             /*Loops until user decides to exit the shell.*/
  {
    printf(": ");                                                               /*Prints out command line prompt.*/
    fflush(stdout);
    fgets(userInput, 512, stdin);                                               /*Takes in the users input.*/
    parsedString = strtok(userInput, " ");                                      /*Parses through the string (delimeter is a space).*/

    indexCount = 0;
    char inputFile[10000] = {0};
    char outputFile[10000] = {0};

    while(parsedString != NULL)
    {                                                                           /*Loops through the string and stores each individual string.*/
      if(strcmp(parsedString, "<") == 0)                                        /*Checks to see if the "<" token is found.*/
      {
        parsedString = strtok(NULL, " ");                                       /*Parses the string and stores the file name and skips the "<" token.*/
        sscanf(parsedString, "%s", inputFile);
        parsedString = strtok(NULL, " ");
      }
      else if(strcmp(parsedString, ">") == 0)                                   /*Checks to see if the ">" token is found.*/
      {
        parsedString = strtok(NULL, " ");                                       /*Parses the string and stores the file name and skips the ">" token.*/
        sscanf(parsedString, "%s", outputFile);
        parsedString = strtok(NULL, " ");
      }
      else if(strstr(parsedString, "$$") != NULL)                               /*Parses the string and stores the file name and skips the "$$" token.*/
      {
        sscanf(parsedString, "%s", cleanString);                                /*Calls wordToPID to repalace the "$$" tokens with the PID.*/
        commandLine[indexCount++] = strdup(wordToPID(getpid(), cleanString, "$$"));
        parsedString = strtok(NULL, " ");
      }
      else
      {
        sscanf(parsedString, "%s", cleanString);                                /*into an array that we can access for later use.*/
        commandLine[indexCount] = strdup(cleanString);
        parsedString = strtok(NULL, " ");
        indexCount++;                                                           /*Keep track of what index we are on due to unknown arguments.*/
      }
    }

    strcpy(commentCheck, (commandLine[0]));                                     /*Copies the first command line argument into a variable.*/

    if(userInput[0] == '\n')                                                    /*Checks to see if the user enters an empty comment.*/
    {
      commandLine[0] = strdup("emptyComment");
    }

    if(strcmp(commandLine[indexCount - 1], "&") == 0)                           /*Checks to see if the user enters an & symbol.*/
    {
      commandLine[indexCount - 1] = NULL;
      processTracker = 1;
    }
    else
    {
      commandLine[indexCount] = NULL;
      processTracker = 0;
    }

    if(strcmp(commandLine[0], "exit") == 0)                                     /*Checks to see if user wants to exit the shell.*/
    {
      valid = 0;
      exit(0);
    }
    else if(strcmp(commandLine[0], "cd") == 0)                                  /*Checks to see if the user wants to change the directory.*/
    {
      changeDirectory(commandLine, indexCount);
    }
    else if(strcmp(commandLine[0], "status") == 0)                              /*Checks to see if the user wants to know the current status.*/
    {
      currentStatus(status);
    }
    else if(strcmp(commandLine[0], "#") == 0 || strcmp(commandLine[0], "emptyComment") == 0 || commentCheck[0] == '#')
    {                                                                           /*Checks to see if a user has written a comment or nothing at all.*/
      /*This is a comment.*/
    }
    else if(strcmp(commandLine[0], "echo") == 0 && commandLine[1] != NULL && strcmp(commandLine[1], "$$") == 0)
    {
      printf("%d\n", getpid());
    }
    else if(skip == 0)                                                          /*Used for formatting.*/
    {
      skip = 1;
    }
    else
    {
      spawnPID = fork();                                                        /*Starts the forking process by calling the fork() process.*/

      switch(spawnPID)
      {
        case -1:                                                                /*If the case is -1 then there is an error and something did not*/
        {                                                                       /*work correctly.*/
          perror("HULL BREACH!\n");
          fflush(stdout);
          status = 1;
          break;
        }
        case 0:                                                                 /*If case is 0 this indicates that we need to run execvp or we need*/
        {                                                                       /*to deal with our file management, also set defaults. CHILD*/
          if(processTracker == 0 || foregroundTracker == 0)
          {
            SIGINT_action.sa_handler = SIG_DFL;                                 /*Sets our SIGINT back to default functionality.*/
            sigaction(SIGINT, &SIGINT_action, NULL);
          }

          if(inputFile[0] != 0)                                                 /*Checks to see if the input file is valid.*/
          {
            validInput = open(inputFile, O_RDONLY);                             /*Allows us to complete basic funcionality such as word count.*/
            if(validInput == -1)
            {
              printf("cannot open %s for input\n", inputFile);                  /*Indicates that the file cannot be opened.*/
              fflush(stdout);
              _exit(1);
            }
            if(dup2(validInput, 0) == -1)                                       /*Indicates that file was not valid.*/
            {
              perror("HULL BREACH - VALID INPUT!\n");
              fflush(stdout);
              _exit(1);
            }
            close(validInput);                                                  /*Close the file we were working with.*/
          }

          if(outputFile[0] != 0)                                                /*Checks to see if the output file is valid.*/
          {
            validOutput = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0744); /*Reads, Creates, and Truncates, the last portion will set the
                                                                                /*permissions.*/
            if(validOutput == -1)                                               /*Indicates that the file cannot be opened.*/
            {
              printf("cannot open %s for output\n", outputFile);
              fflush(stdout);
              _exit(1);
            }
            if(dup2(validOutput, 1) == -1)                                      /*Indicates that file was not valid.*/
            {
              perror("HULL BREACH - VALID OUTPUT!\n");
              fflush(stdout);
              _exit(1);
            }
            close(validOutput);                                                 /*Close the file we were working with.*/
          }

          if(execvp(commandLine[0], commandLine) < 0)                           /*Utilize execvp() to handle other functionality and indicates,*/
          {                                                                     /*whether or not the file/directory/command is valid.*/
            printf("%s: no such file or directory\n", commandLine[0]);
            fflush(stdout);
            _exit(1);
          }
          break;
        }
        default:                                                                /*If case is 0 this indicates that we need to run execvp or we need*/
        {                                                                       /*to deal with our file management, also set defaults. PARENT*/
          if(processTracker == 0 || foregroundTracker == 0)                     /*Checks to see if we are in the foreground or not.*/
          {
            waitpid(spawnPID, &status, 0);                                      /*Tells the parent to wait for the child.*/
          }
          else
          {
            printf("background pid is %i\n", spawnPID);                         /*Display the background PID.*/
            fflush(stdout);
            usleep(200000);
          }
          break;
        }
      }
    }
    usleep(200000);
    spawnPID = waitpid(-1, &status, WNOHANG);                                   /*Used for making the Parent "Hang" for the child.*/

    while(spawnPID > 0)
    {
      printf("background pid %i is done: ", spawnPID);                          /*Displays the background ID when the process for the Child,*/
      fflush(stdout);                                                           /*has completed it task, then displays the exit status*/
      currentStatus(status);
      spawnPID = waitpid(-1, &status, WNOHANG);
    }
  }
}

/**********************************************************************
** * Function: changeDirectory
** * Description: Changes the directory to the requested directory
** * Parameters: char commandLine[512][2048], int indexCount
** * Pre-Conditions: The command cd must be referenced first
** * Post-Conditions: The user is moved to the into the requested directory
**********************************************************************/

void changeDirectory(char* commandLine[2048], int indexCount)
{
  if(indexCount == 2)                                                           /*If the index count is equal to two the shell will change the*/
  {                                                                             /*the directory to the one the user has requested.*/
     chdir(commandLine[1]);
  }
  else                                                                          /*If only cd is input by the user the home directory is the*/
  {                                                                             /*default location.*/
    chdir(getenv("HOME"));
  }
}

/**********************************************************************
** * Function: currentStatus
** * Description: Gives the current status of the last foreground process
** * Parameters: int status
** * Pre-Conditions: Status must be initilized to zero
** * Post-Conditions: Provide the current status of the last foreground process
**********************************************************************/

void currentStatus(int status)
{
  int exitStatus = -5;                                                          /*Initilize exitStatus to be a value that we know can no be used.*/

  if(!WIFEXITED(status))                                                         /*Used to determine the exit status of the child.*/
  {
    printf("terminated by signal %i\n", status);
    fflush(stdout);
  }
  else                                                                          /*Used when the process is ended prematurely.*/
  {
    exitStatus = WEXITSTATUS(status);
    printf("exit value %i\n", exitStatus);
    fflush(stdout);
  }
}

/**********************************************************************
** * Function: catchSIGINT
** * Description: Performs the ^C functionality
** * Parameters: int signo
** * Pre-Conditions: The signls smust be defined beforehand
** * Post-Conditions: Handles all of the ^C functionality
**********************************************************************/

void catchSIGINT(int signo)
{
  if(foregroundTracker == 1)                                                    /*Determines whether or not we are in the foreground*/
  {
    printf("\n");                                                               /*Used for formatting*/
    fflush(stdout);
    skip = 0;
  }
  else
  {
    printf("\n");                                                               /*Used for formatting*/
    fflush(stdout);
    skip = 0;
  }
}

/**********************************************************************
** * Function: catchSIGSTP
** * Description: Performs the ^Z functionality
** * Parameters: int signo
** * Pre-Conditions: The signls smust be defined beforehand
** * Post-Conditions: Handles all of the ^Z functionality
**********************************************************************/

void catchSIGSTP(int signo)
{
  if(foregroundTracker == 1)                                                    /*Determines whether or not we are in the foreground*/
  {
    foregroundTracker = 0;                                                      /*Creates a toggle with ^Z to enter the foreground only mode*/
    write(1, "\nEntering foreground-only mode (& is now ignored)\n", 51);
    skip = 0;                                                                   /*Used for formatting*/
  }
  else
  {
    foregroundTracker = 1;                                                      /*Creates a toggle with ^Z to exit the foreground only mode*/
    write(1, "\nExiting foreground-only mode.\n", 31);
    skip = 0;                                                                   /*Used for formatting*/
  }
}

/**********************************************************************
** * Function: wordToPID
** * Description: Returns a new string replacing the "$$" for the PID
** * Parameters: int PID, const char* commandLineArg, const char* original
** * Pre-Conditions: An original string with "$$" token must be passed
** * Post-Conditions: Returns a string with the PID added on the end
**********************************************************************/

char* wordToPID(int PID, const char* commandLineArg, const char* original)
{
    int i;
    int indexCount = 0;
    int count = 0;
    int pidLength;
    int originalLength;
    char pid[100];
    char *replacePID;

    sprintf(pid, "%d", PID);                                                    /*Writes formatted data into a string.*/

    pidLength = strlen(pid);
    originalLength = strlen(original);

    for(i = 0; commandLineArg[i] != '\0'; i++)                                  /*Loops through the string we desired and deletes the "$$" tokens.*/
    {
      if(strstr(&commandLineArg[i], original) == &commandLineArg[i])            /*Checks for "$$" token*/
      {
        count++;
        i += originalLength - 1;
      }
    }

    replacePID = (char*)malloc(i + count * (pidLength - originalLength) + 1);

    while(*commandLineArg)                                                      /*Loops until the desired string has been created.*/
    {
      if(strstr(commandLineArg, original) == commandLineArg)
      {
        strcpy(&replacePID[indexCount], pid);                                   /*Adds the PID to the end of the string wothout the "$$" tokens.*/
        indexCount += pidLength;
        commandLineArg += originalLength;
      }
      else                                                                      /*If here then just keep moving through the string.*/
      {
        replacePID[indexCount++] = *commandLineArg++;
      }
    }

    replacePID[indexCount] = '\0';

    return replacePID;                                                          /*Return the new string.*/
}
