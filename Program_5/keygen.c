/**********************************************************************
** * Program: keygen
** * Author: Lake Peterson
** * Date: March 13, 2020
** * Description: Generates a unique key based on user requested length
** * Input: User inputs a length via command line
** * Output: Returns a generated character key of requested length
**********************************************************************/

/**********************************************************************
** * Included Libraries
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**********************************************************************
** * Prototypes
**********************************************************************/

void generateKey(int, char const*);

/**********************************************************************
** * Function: main
**********************************************************************/

int main(int argc, char const *argv[])
{
  generateKey(argc, argv[1]);

  return 0;
}

/**********************************************************************
** * Function: generateKey
** * Description: Generates a unique key based on user requested length
** * Parameters: int argc, char const* argv
** * Pre-Conditions: User inputs a length via command line
** * Post-Conditions: Returns a generated character key of requested length
**********************************************************************/

void generateKey(int argc, char const* argv)
{
  if(argc == 2)
  {
    int commandArgLength;
    int buffer;
    int i;
    int pickedIndex;
    char* keyCharacters;
    srand(time(NULL));                                                          // Set the seed of the random number generator.

    commandArgLength = atoi(argv);                                              // Using atoi this will convert the string into an integer.
    buffer = commandArgLength + 1;                                              // Buffer is used to set the size of the key string.
    keyCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";;                              // String of characters that are used for random number selection.

    char keyGen[buffer];

    for(i = 0; i < commandArgLength; i++)                                       // Loops until the length of the key has been met.
    {
      pickedIndex = rand() % 27;                                                // Generate a random number.
      keyGen[i] = keyCharacters[pickedIndex];                                   // Save the random character into the key string.
    }

    keyGen[commandArgLength] = '\0';                                            // Once the key has been created assign a NULL terminator to the end.

    printf("%s\n", keyGen);                                                     // Print out the created key.
  }
  else
  {
    fprintf(stderr, "Too few command line arguments\n");                        // Error handling, in which the number of arguments in not correct.
    exit(0);
  }
}
