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

#define MAX_CONNECTIONS (int)6
#define BUFFER_SIZE (int)1000
#define BUFFER_INPUT (int)100

typedef struct
{
  int connection[MAX_CONNECTIONS];
  int numConnections;
  char roomName[7];
  char type;
} buildRoom;

void createGame();
void createRoomStructs(buildRoom*);
char* findsRecentDir();
int indexFinder(buildRoom*, char*, int);
void gameInterface(buildRoom*, char*);
char* findRoom(buildRoom*, int, char*);
void findConnections(buildRoom*, int, char*);
int findUserIndex(buildRoom*, int, char*);
int checkConnections(buildRoom*, int, char*);
void printWin(buildRoom*, int*, int);

int main()
{
  createGame();

  return 0;
}

void createGame()
{
  buildRoom* rooms = malloc(7 * sizeof(buildRoom));
  char* currentRoom = malloc(10 * sizeof(char));

  createRoomStructs(rooms);
  gameInterface(rooms, currentRoom);
}

void createRoomStructs(buildRoom* rooms)
{
  FILE* openFile;
  DIR* directory;
  struct dirent* file;
  char* recentDir;
  char directoryPath[50];
  char filePath[50];
  char storeVal[50];
  char fileLine[BUFFER_SIZE];
  int i = 0;
  int roomSize = 7;
  int nameIndex = 0;
  int typeIndex = 0;
  int connectionIndex = 0;

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
        sscanf(fileLine, "%*[^:]: %[^]%*[^\n]", storeVal);
        if(fileLine[5] != 'T' && fileLine[0] != 'C')
        {
          rooms[nameIndex].numConnections = 0;
          strcpy(rooms[nameIndex].roomName, storeVal);
          nameIndex++;
        }
        else if(fileLine[5] == 'T' && fileLine[0] != 'C')
        {
          rooms[typeIndex].type = storeVal[0];
          typeIndex++;
        }
      }
      fclose(openFile);
    }
  }

  directory = opendir(directoryPath);

  while((file = readdir(directory)) != NULL)
  {
    if(file->d_name[0] != '.')
    {
      sprintf(filePath, "%s%s", directoryPath, file->d_name);
      openFile = fopen(filePath, "r");

      while(fgets(fileLine, BUFFER_SIZE, openFile) != NULL)
      {
        sscanf(fileLine, "%*[^:]: %[^]%*[^\n]", storeVal);
        if(fileLine[0] == 'C')
        {
          rooms[connectionIndex].connection[rooms[connectionIndex].numConnections] = indexFinder(rooms, storeVal, roomSize);
          rooms[connectionIndex].numConnections++;
        }
      }
      fclose(openFile);
      connectionIndex++;
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

int indexFinder(buildRoom* rooms, char* storeVal, int roomSize)
{
  int i = 0;

  for(i = 0; i < roomSize; i++)
  {
    if(strcmp(rooms[i].roomName, storeVal) == 0)
    {
      return i;
    }
  }
  return 0;
}

void gameInterface(buildRoom* rooms, char* room)
{
  int i = 0;
  int j = 0;
  int tracker = 0;
  int roomSize = 7;
  int checkInput = 0;
  int checkWin = 0;
  int userPath[1000];
  int savedIndex = 0;
  char currentRoom[10];
  char roomConnections[50];
  char userInput[100];
  char savedInput[100];

  strcpy(currentRoom, findRoom(rooms, roomSize, room));

  while(checkWin == 0)
  {
    while(checkInput == 0)
    {
      printf("CURRENT ROOM: %s\n", currentRoom);
      findConnections(rooms, roomSize, currentRoom);
      printf("WHERE TO? >");
      fgets(userInput, BUFFER_INPUT, stdin);
      sscanf(userInput, "%s", savedInput);

      if(checkConnections(rooms, roomSize, savedInput) == 1)
      {
        checkInput = 1;
        strcpy(currentRoom, savedInput);
        savedIndex = findUserIndex(rooms, roomSize, savedInput);
        userPath[tracker] = savedIndex;
        tracker++;
        printf("\n");
      }
      else
      {
        printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
      }
    }
    if(rooms[savedIndex].type == 'E')
    {
      printWin(rooms, userPath, tracker);
      checkWin = 1;
    }
    else
    {
      checkInput = 0;
    }
  }
}

char* findRoom(buildRoom* rooms, int roomSize, char* room)
{
  int i = 0;

  for(i = 0; i < roomSize; i++)
  {
    if(rooms[i].type == 'S')
    {
      strcpy(room, rooms[i].roomName);
    }
  }

  return room;
}

void findConnections(buildRoom* rooms, int roomSize, char* currentRoom)
{
  int i = 0;
  int j = 0;

  for(i = 0; i < roomSize; i++)
  {
    if(strcmp(currentRoom, rooms[i].roomName) == 0)
    {
      printf("POSSIBLE CONNECTIONS: ");
      for (j = 0; j < rooms[i].numConnections; ++j)
      {
        printf("%s", rooms[rooms[i].connection[j]].roomName);
        if(j < (rooms[i].numConnections - 1))
        {
          printf(", ");
        }
      }
      printf(".\n");
    }
  }
}

int findUserIndex(buildRoom* rooms, int roomSize, char* savedInput)
{
  int i = 0;
  int savedIndex = -1;

  for(i = 0; i < roomSize; i++)
  {
    if(strcmp(rooms[i].roomName, savedInput) == 0)
    {
      savedIndex = i;
    }
  }
  return savedIndex;
}

int checkConnections(buildRoom* rooms, int roomSize, char* savedInput)
{
  int i = 0;
  int savedIndex = -1;

  for(i = 0; i < roomSize; i++)
  {
    if(strcmp(rooms[i].roomName, savedInput) == 0)
    {
      savedIndex = i;
      break;
    }
  }

  if(savedIndex == -1)
  {
    return 0;
  }

  for(i = 0; i < rooms[savedIndex].numConnections; i++)
  {
    if(strcmp(savedInput, rooms[rooms[savedIndex].connection[i]].roomName))
    {
      return 1; /*Found Match*/
    }
  }
  return 0; /*No Match*/
}

void printWin(buildRoom* rooms, int* userPath, int tracker)
{
  printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", tracker);
  int i = 0;

  for(i = 0; i < tracker; i++)
  {
    printf("%s\n", rooms[userPath[i]].roomName);
  }
}
