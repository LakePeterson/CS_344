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

/**********************************************************************
** * Prototypes
**********************************************************************/

void shell(char*);
void changeDirectory(char [][2048], int);

/**********************************************************************
** * Function: main
**********************************************************************/

int main()
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
  int indexCount;
  int i = 0;
  char* parsedString;
  char cleanString[512];
  char commandLine[512][2048];
  char s[100];

  while(valid == 1)                                                             /*Loops until user decides to exit the shell.*/
  {
    printf(": ");                                                               /*Prints out command line prompt.*/
    fgets(userInput, 512, stdin);                                               /*Takes in the users input.*/
    parsedString = strtok(userInput, " ");                                      /*Parses through the string (delimeter is a space).*/

    indexCount = 0;

    while(parsedString != NULL)                                                 /*Loops through the string and stores each individual string*/
    {                                                                           /*into an array that we can access for later use.*/
      sscanf(parsedString, "%s", cleanString);
      strcpy(commandLine[indexCount], cleanString);
      parsedString = strtok(NULL, " ");

      indexCount++;                                                             /*Keep track of what index we are on due to unknown arguments.*/
    }

    fflush(stdin);

    if(strcmp(commandLine[0], "exit") == 0)                                     /*Checks to see if user wants to exit the shell.*/
    {
      valid = 0;
      exit(0);
    }
    else if(strcmp(commandLine[0], "cd") == 0)                                  /*Checks to see if the user wants to change the directory.*/
    {
      changeDirectory(commandLine, indexCount);
    }
    else if(strcmp(commandLine[0], "status") == 0)                              /*Checks to see if the user wants toi know the current status.*/
    {
      printf("Status was entered\n");
    }
    else if(strcmp(commandLine[0], "#") == 0)                                   /*Checks to see if a user has written a comment.*/
    {
      /*Do Nothing --- This is a Comment"*/
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

void changeDirectory(char commandLine[512][2048], int indexCount)
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
