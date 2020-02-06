/**********************************************************************
** * Program:
** * Author: Lake Peterson
** * Date:
** * Description:
** * Input:
** * Output:
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

#define MIN_CONNECTIONS (int)3
#define MAX_CONNECTIONS (int)6
#define BUFFER_SIZE (int)1000

typedef struct
{
  int connection[MAX_CONNECTIONS];
  int numConnections;
  char roomName[7];
  char type;
} buildRoom;

buildRoom* createRoomStructs();
char* findsRecentDir();

int main()
{
  createRoomStructs();

  return 0;
}

buildRoom* createRoomStructs()
{
  DIR* directory;
  struct dirent* file;
  char* recentDir;
  char directoryPath[50];
  char filePath[50];
  FILE* openFile;
  char fileLine[BUFFER_SIZE];


  recentDir = findsRecentDir();
  sprintf(directoryPath, "./%s/", recentDir);

  directory = opendir(directoryPath);

  while((file = readdir(directory)) != NULL)
  {
    if(file->d_name[0] != '.')
    {
      sprintf(filePath, "%s%s", directoryPath, file->d_name);
      openFile = fopen(filePath, "r");

      while(fgets(fileLine, BUFFER_SIZE, openFile) != NULL)
      {
        printf("%s\n", fileLine);
      }
      printf("\n");
      fclose(openFile);
    }
  }
  closedir(directory);
}

char* findsRecentDir()
{
  DIR* directory;
  struct stat buffer;
  struct dirent* folder;
  long recentTime = -1;
  char* recentFolder;

  directory = opendir(".");

  while((folder = readdir(directory)) != NULL)
  {
    stat(folder->d_name, &buffer);

    if(folder->d_name[14] == '.' && folder->d_name[15] != 'c' && buffer.st_mtime >= recentTime)
    {
      recentTime = buffer.st_mtime;
      recentFolder = folder->d_name;
    }
  }
  closedir(directory);

  return recentFolder;
}
