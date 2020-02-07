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

#define MAX_CONNECTIONS (int)6

typedef struct
{
  int connection[MAX_CONNECTIONS];
  int numConnections;
  char roomName[7];
  char type;
} buildRoom;

buildRoom* createRoomStructs();
void initializeStruct(buildRoom*);
int* randPickRooms(int*);
void createRooms(buildRoom*, int*);
void createTypes(buildRoom*);
void createConnections(buildRoom*);
int randNum();
int checkGraph(buildRoom*);
int checkRoomExists(buildRoom, int);
void createDirectory(buildRoom*);
void writeRooms(buildRoom*, char*);
void freeMemory(buildRoom*, int*);

int main()
{
  srand(time(0));

  createRoomStructs();

  return 0;
}

buildRoom* createRoomStructs()
{
  buildRoom* rooms = malloc(7 * sizeof(buildRoom));
  int* selectedRooms = malloc(7 * sizeof(int));

  initializeStruct(rooms);
  createRooms(rooms, selectedRooms);
  createTypes(rooms);
  createConnections(rooms);
  createDirectory(rooms);
  freeMemory(rooms, selectedRooms);
}

void initializeStruct(buildRoom* rooms)
{
  int i = 0;
  int roomSize = 7;

  for(i = 0; i < roomSize; i++)
  {
    rooms[i].numConnections = 0;
  }
}

int* randPickRooms(int* selectedRooms)
{
  int pick;
  int roomSize = 7;
  int i = 0;

  for(i = 0; i < roomSize; i++)
  {
    pick = randNum(0, 9);

    while(checkPick(selectedRooms, roomSize, pick) == 1)
    {
      pick = randNum(0, 9);
    }

    selectedRooms[i] = pick;
  }

  return selectedRooms;
}

void createRooms(buildRoom* rooms, int* selectedRooms)
{
  char* letterRooms[10] = {"aRoom", "bRoom", "cRoom", "dRoom", "eRoom", "fRoom", "gRoom", "hRoom", "iRoom", "jRoom"};
  int* selection;
  int roomSize = 7;
  int i = 0;

  selection = randPickRooms(selectedRooms);

  for(i = 0; i < roomSize; i++)
  {
    strcpy(rooms[i].roomName, letterRooms[selection[i]]);
  }
}

void createTypes(buildRoom* rooms)
{
  int roomSize = 7;
  int randomNum = 0;
  int count = 0;
  int tempOne = 0;
  int tempTwo = 0;
  int i = 0;

  while(count != 3)
  {
    randomNum = randNum(0, 6);

    if(count == 0)
    {
      rooms[randomNum].type = 's';
      tempOne = randomNum;
      count++;
    }
    else if(count == 1 && randomNum != tempOne)
    {
      tempTwo = randomNum;
      rooms[randomNum].type = 'e';
      count++;
    }
    else if(count == 2 && randomNum != tempOne && randomNum != tempTwo)
    {
      for(i = 0; i < roomSize; i++)
      {
        if(rooms[i].type != 'e' && rooms[i].type != 's')
        {
          rooms[i].type = 'm';
        }
      }
      count++;
    }
  }
}

void createConnections(buildRoom* rooms)
{
  int i = 0;

  while(checkGraph(rooms) == 0)
  {
    for(i = 0; i < 7; i++)
    {
      int randomConnection = randNum(0, 6);
      while(checkRoomExists(rooms[i], randomConnection) || randomConnection == i || rooms[randomConnection].numConnections >= 6)
      {
        if(rooms[i].numConnections >= 6)
        {
          break;
        }
        randomConnection = randNum(0, 6);
      }
      rooms[i].connection[rooms[i].numConnections] = randomConnection;
      rooms[randomConnection].connection[rooms[randomConnection].numConnections] = i;
      rooms[i].numConnections++;
      rooms[randomConnection].numConnections++;

      if(checkGraph(rooms) == 1)
      {
        break;
      }
    }
  }
}

int randNum(int floor, int ceiling)
{
  int number;

  number = (rand() % (ceiling - floor + 1)) + floor;

  return number;
}

int checkPick(int* selectedRooms, int roomSize, int pick)
{
  int i = 0;

  for(i = 0; i < roomSize; i++)
  {
    if(selectedRooms[i] == pick)
    {
      return 1; /*FALSE*/
    }
  }
  return 0; /*TRUE*/
}

int checkGraph(buildRoom* rooms)
{
  int i = 0;
  int roomSize = 7;

  for(i = 0; i < roomSize; i++)
  {
    if(rooms[i].numConnections < 3)
    {
      return 0; /*FALSE*/
    }
  }
  return 1; /*TRUE*/
}

int checkRoomExists(buildRoom rooms, int randomConnection)
{
  int i = 0;

  for(i = 0; i < rooms.numConnections; i++)
  {
    if(rooms.connection[i] == randomConnection)
    {
      return 1;
    }
  }
  return 0;
}

void createDirectory(buildRoom* rooms)
{
  int getPID = getpid();
  char stringPID[50];
  char directory[100] = "peterlak.rooms.";

  sprintf(stringPID, "%d", getPID);
  strcat(directory, stringPID);

  mkdir(directory, 0700);

  writeRooms(rooms, directory);
}

void writeRooms(buildRoom* rooms, char* directory)
{
  FILE* outputFiles;
  int roomSize = 7;
  char filePath[50];
  int i = 0;
  int j = 0;

  for(i = 0; i < roomSize; i++)
  {
    sprintf(filePath, "./%s/%s", directory, rooms[i].roomName);
    outputFiles = fopen(filePath, "w");
    fprintf(outputFiles, "ROOM NAME: %s\n", rooms[i].roomName);

    for (j = 0; j < rooms[i].numConnections; j++)
    {
      fprintf(outputFiles, "CONNECTION %d: %s\n", j+1, rooms[rooms[i].connection[j]].roomName);
    }

    if(rooms[i].type == 's')
    {
      fprintf(outputFiles, "ROOM TYPE: START_ROOM\n");
    }
    else if(rooms[i].type == 'm')
    {
      fprintf(outputFiles, "ROOM TYPE: MID_ROOM\n");
    }
    else
    {
      fprintf(outputFiles, "ROOM TYPE: END_ROOM\n");
    }
    fclose(outputFiles);
  }
}

void freeMemory(buildRoom* rooms, int* selectedRooms)
{
  free(rooms);
  free(selectedRooms);
}
