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

#define MIN_CONNECTIONS (int)3
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
void createRooms(buildRoom*);
void createTypes(buildRoom*);
void createConnections(buildRoom*);
int randNum();
int checkGraph(buildRoom*);
int checkRoomExists(buildRoom, int);
void printRooms(buildRoom*);
void freeMemory(buildRoom*);
/* void createDirectory(); */
/* void createRoomNames(char*); */

int main()
{
  /* createDirectory(); */

  srand(time(0));

  createRoomStructs();

  return 0;
}

buildRoom* createRoomStructs()
{
  buildRoom* rooms = malloc(7 * sizeof(buildRoom));

  initializeStruct(rooms);
  createRooms(rooms);
  createTypes(rooms);
  createConnections(rooms);



  printRooms(rooms);

  freeMemory(rooms);

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

void createRooms(buildRoom* rooms)
{
  char* letterRooms[7] = {"a_room", "b_room", "c_room", "d_room", "e_room", "f_room", "g_room"};
  int roomSize = 7;
  int i = 0;

  for(i = 0; i < roomSize; i++)
  {
    strcpy(rooms[i].roomName, letterRooms[i]);
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
      while (checkRoomExists(rooms[i], randomConnection) || randomConnection == i || rooms[randomConnection].numConnections >= 6)
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

void printRooms(buildRoom* rooms)
{
  int roomSize = 7;
  int i = 0;
  int j = 0;

  printf("\n");

  for(i = 0; i < roomSize; i++)
  {
    printf("Name: %s\n", rooms[i].roomName);
    printf("Type: %c\n", rooms[i].type);
    printf("NumConnections: %d\n", rooms[i].numConnections);
    printf("Connections: ");
    for (j = 0; j < rooms[i].numConnections; j++)
    {
      printf("%s ", rooms[rooms[i].connection[j]].roomName);
    }
  printf("\n\n");
  }
}

void freeMemory(buildRoom* rooms)
{
  free(rooms);
}



























/*
void createDirectory()
{
  int getPID = getpid();
  char stringPID[50];
  char directory[100] = "peterlak.buildrooms.";

  sprintf(stringPID, "%d", getPID);
  strcat(directory, stringPID);

  mkdir(directory, 0700);

  createRoomNames(directory);
}

void createRoomNames(char* directory)
{
  char filePath[100];
  char* roomName[7] = {"a_room", "b_room", "c_room", "d_room", "e_room", "f_room", "g_room"};
  FILE* outFile;
  int roomSize = 7;
  int i;

  for(i = 0; i < roomSize; i++)
  {
    sprintf(filePath, "./%s/%s", directory, roomName[i]);
    outFile = fopen(filePath, "w");
  }

  fclose(outFile);
}
*/
