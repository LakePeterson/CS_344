////////////////////////////////////////////////////////////////////////
// Program:
// Author: Lake Peterson
// Date:
// Description:
// Input:
// Output:
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

int createDirectory();
void createRooms(char*);

int main()
{
  createDirectory();



  return 0;
}

int createDirectory()
{
  int getPID = getpid();
  char stringPID[50];
  char directory[100] = "peterlak.buildrooms.";

  sprintf(stringPID, "%d", getPID);
  strcat(directory, stringPID);

  int valid = mkdir(directory, 0700);

  if(valid == 0)
  {
    createRooms(directory);
    return 1;
  }
  else
  {
    return 0;
  }
}

void createRooms(char* directory)
{
  char filePath[100];
  sprintf(filePath, "./%s/livingRoom", directory);

  printf("%s\n", filePath);

  FILE* outFile = fopen(filePath, "w");

  fclose(outFile);

}
