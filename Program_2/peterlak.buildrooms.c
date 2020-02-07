/**********************************************************************
** * Program: Buildrooms
** * Author: Lake Peterson
** * Date: February 7, 2020
** * Description: Creates a directory to place seven created rooms
** * Input: No input is necessary in order for this program to run
** * Output: Outputs one directory and seven room files
**********************************************************************/

/**********************************************************************
** * Included Libraries
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************
** * Global Variables
**********************************************************************/

#define MAX_CONNECTIONS (int)6

/**********************************************************************
** * Buildroom Structure
**********************************************************************/

typedef struct
{
  int connection[MAX_CONNECTIONS];
  int numConnections;
  char roomName[7];
  char type;
} buildRoom;

/**********************************************************************
** * Prototypes
**********************************************************************/

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

/**********************************************************************
** * Function: main
**********************************************************************/

int main()
{
  srand(time(0));                                                               /*Creates a seed for random number generator.*/

  createRoomStructs();

  return 0;
}

/**********************************************************************
** * Function: createRoomStructs
** * Description: Fills in all variables that are specified in the struct
** * Parameters: No parameters are needed for this function
** * Pre-Conditions: Must allocate all necessary memory to be passed
** * Post-Conditions: Outlines the necessary steps to build the rooms
**********************************************************************/

buildRoom* createRoomStructs()
{
  buildRoom* rooms = malloc(7 * sizeof(buildRoom));                             /*Allocates memory for the rooms in the structure.*/
  int* selectedRooms = malloc(7 * sizeof(int));                                 /*Allocates memory for the random selected rooms.*/

  initializeStruct(rooms);
  createRooms(rooms, selectedRooms);
  createTypes(rooms);
  createConnections(rooms);
  createDirectory(rooms);
  freeMemory(rooms, selectedRooms);
}

/**********************************************************************
** * Function: initializeStruct
** * Description: Initializes values in struct
** * Parameters: buildRoom* rooms
** * Pre-Conditions: No preconditions are necessary
** * Post-Conditions: Must initilize the number of connections equal to zero
**********************************************************************/

void initializeStruct(buildRoom* rooms)
{
  int i = 0;
  int roomSize = 7;

  for(i = 0; i < roomSize; i++)                                                 /*Sets the numConnections value on all the rooms to zero*/
  {                                                                             /*to avoid any garbage values that may appear.*/
    rooms[i].numConnections = 0;
  }
}

/**********************************************************************
** * Function: randPickRooms
** * Description: Selects seven randon indices out of a total of ten
** * Parameters: int* selectedRooms
** * Pre-Conditions: No preconditions are necessary
** * Post-Conditions: Must correctly choose and return seven random indices
**********************************************************************/

int* randPickRooms(int* selectedRooms)
{
  int pick;
  int roomSize = 7;
  int i = 0;

  for(i = 0; i < roomSize; i++)                                                 /*Randomly picks an index for a possible room, if that room*/
  {                                                                             /*already exists it will regenerate a random number to be*/
    pick = randNum(0, 9);                                                       /*picked.*/

    while(checkPick(selectedRooms, roomSize, pick) == 1)
    {
      pick = randNum(0, 9);
    }

    selectedRooms[i] = pick;                                                    /*Sets the index of selected rooms equal to the random index.*/
  }

  return selectedRooms;                                                         /*Returns the array of indices to be used in selecting rooms.*/
}

/**********************************************************************
** * Function: createRooms
** * Description: Allocates the names of the rooms in the struct
** * Parameters: buildRoom* rooms, int* selectedRooms
** * Pre-Conditions: letterRooms must have a dedicated set of rooms
** * Post-Conditions: Fills the structure variable roomName with the names
**********************************************************************/

void createRooms(buildRoom* rooms, int* selectedRooms)
{
  char* letterRooms[10] = {"aRoom", "bRoom", "cRoom", "dRoom", "eRoom",         /*This is the array of possible rooms that can be picked.*/
                          "fRoom", "gRoom", "hRoom", "iRoom", "jRoom"};
  int* selection;
  int roomSize = 7;
  int i = 0;

  selection = randPickRooms(selectedRooms);

  for(i = 0; i < roomSize; i++)                                                 /*Assigns the name of the rooms, based on the random selection.*/
  {
    strcpy(rooms[i].roomName, letterRooms[selection[i]]);
  }
}

/**********************************************************************
** * Function: createTypes
** * Description: Allocates the types of the rooms in the struct
** * Parameters: buildRoom* rooms
** * Pre-Conditions: Random number generator must be seeded before
** * Post-Conditions: Fills the structure variable types with the room type
**********************************************************************/

void createTypes(buildRoom* rooms)
{
  int roomSize = 7;
  int randomNum = 0;
  int count = 0;
  int tempOne = 0;
  int tempTwo = 0;
  int i = 0;

  while(count != 3)                                                             /*Loops to ensure that the number of connections is at least three.*/
  {
    randomNum = randNum(0, 6);

    if(count == 0)                                                              /*When count is zero assign this to be the start room.*/
    {
      rooms[randomNum].type = 's';
      tempOne = randomNum;
      count++;
    }
    else if(count == 1 && randomNum != tempOne)                                 /*If this condition is met, then this is the end room.*/
    {
      tempTwo = randomNum;
      rooms[randomNum].type = 'e';
      count++;
    }
    else if(count == 2 && randomNum != tempOne && randomNum != tempTwo)         /*This condition is needed to set the rest of the rooms to be*/
    {                                                                           /*the middle rooms.*/
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

/**********************************************************************
** * Function: createConnections
** * Description: Allocates the connections of the rooms in the struct
** * Parameters: buildRoom* rooms
** * Pre-Conditions: Random number generator must be seeded before
** * Post-Conditions: Fills the structure variable *connections with the connections
**********************************************************************/

void createConnections(buildRoom* rooms)
{
  int i = 0;

  while(checkGraph(rooms) == 0)                                                 /*Checks to make sure the graph condition has not been meet, in*/
  {                                                                             /*which the is a min of 3 connections and a max of 6 connections.*/
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
      }                                                                         /*Assigns the connections as well as the number of connections.*/
      rooms[i].connection[rooms[i].numConnections] = randomConnection;
      rooms[randomConnection].connection[rooms[randomConnection].numConnections] = i;
      rooms[i].numConnections++;
      rooms[randomConnection].numConnections++;

      if(checkGraph(rooms) == 1)                                                /*If graph conditions are met then break out of the while loop.*/
      {
        break;
      }
    }
  }
}

/**********************************************************************
** * Function: randNum
** * Description: Generates a random number between specified bounds
** * Parameters: int floor, int ceiling
** * Pre-Conditions: Random number generator must be seeded before
** * Post-Conditions: Return the random number that was generated
**********************************************************************/

int randNum(int floor, int ceiling)
{
  int number;

  number = (rand() % (ceiling - floor + 1)) + floor;                            /*Based on the specified bounds it generates a random number.*/

  return number;
}

/**********************************************************************
** * Function: checkPick
** * Description: Generates true or false outcome based on room valididity
** * Parameters: int* selectedRooms, int roomSize, int pick
** * Pre-Conditions: No preconditions are necessary
** * Post-Conditions: Return the true or false based on the pick
**********************************************************************/

int checkPick(int* selectedRooms, int roomSize, int pick)
{
  int i = 0;

  for(i = 0; i < roomSize; i++)                                                 /*Check to make sure that the room pick is valid.*/
  {
    if(selectedRooms[i] == pick)
    {
      return 1; /*FALSE*/
    }
  }
  return 0; /*TRUE*/
}

/**********************************************************************
** * Function: checkGraph
** * Description: Generates true or false outcome based on number of connections
** * Parameters: buildRoom* rooms
** * Pre-Conditions: No preconditions are necessary
** * Post-Conditions: Return the true or false based on the amount of connections
**********************************************************************/

int checkGraph(buildRoom* rooms)
{
  int i = 0;
  int roomSize = 7;

  for(i = 0; i < roomSize; i++)                                                 /*Check to make sure the graph is satisfied, based on connections.*/
  {
    if(rooms[i].numConnections < 3)
    {
      return 0; /*FALSE*/
    }
  }
  return 1; /*TRUE*/
}

/**********************************************************************
** * Function: checkRoomExists
** * Description: Generates true or false outcome based on if a room exists
** * Parameters: buildRoom rooms, int randomConnection
** * Pre-Conditions: No preconditions are necessary
** * Post-Conditions: Return the true or false based on existance
**********************************************************************/

int checkRoomExists(buildRoom rooms, int randomConnection)
{
  int i = 0;

  for(i = 0; i < rooms.numConnections; i++)                                     /*Checks to see if a room exists.*/
  {
    if(rooms.connection[i] == randomConnection)
    {
      return 1;
    }
  }
  return 0;
}

/**********************************************************************
** * Function: createDirectory
** * Description: Creates a directory that room files can be placed in
** * Parameters: buildRoom* rooms
** * Pre-Conditions: C Strings must be allocated to necissary length
** * Post-Conditions: Writes a directory of the format peterlak.rooms.PID
**********************************************************************/

void createDirectory(buildRoom* rooms)
{
  int getPID = getpid();                                                        /*Retrieves a random PID when creating the directory.*/
  char stringPID[50];
  char directory[100] = "peterlak.rooms.";                                      /*Creates the name of the directory that is created.*/

  sprintf(stringPID, "%d", getPID);
  strcat(directory, stringPID);                                                 /*Concatinates the directory name.*/

  mkdir(directory, 0700);                                                       /*Sets the file permissions for the directory*/

  writeRooms(rooms, directory);
}

/**********************************************************************
** * Function: writeRooms
** * Description: Writes the rooms files inside of the created directory
** * Parameters: buildRoom* rooms, char* directory
** * Pre-Conditions: Files must be placed in correct directory
** * Post-Conditions: Writes and fills room files inside of the directory
**********************************************************************/

void writeRooms(buildRoom* rooms, char* directory)
{
  FILE* outputFiles;
  int roomSize = 7;
  char filePath[50];
  int i = 0;
  int j = 0;

  for(i = 0; i < roomSize; i++)                                                 /*Places all structure variables in independent room files, so*/
  {                                                                             /*that all files are filled accordingly.*/
    sprintf(filePath, "./%s/%s", directory, rooms[i].roomName);
    outputFiles = fopen(filePath, "w");
    fprintf(outputFiles, "ROOM NAME: %s\n", rooms[i].roomName);                 /*Places the room name.*/

    for (j = 0; j < rooms[i].numConnections; j++)                               /*Places the room connections.*/
    {
      fprintf(outputFiles, "CONNECTION %d: %s\n", j+1, rooms[rooms[i].connection[j]].roomName);
    }

    if(rooms[i].type == 's')
    {
      fprintf(outputFiles, "ROOM TYPE: START_ROOM\n");                          /*Places the room type.*/
    }
    else if(rooms[i].type == 'm')
    {
      fprintf(outputFiles, "ROOM TYPE: MID_ROOM\n");                            /*Places the room type.*/
    }
    else
    {
      fprintf(outputFiles, "ROOM TYPE: END_ROOM\n");                            /*Places the room type.*/
    }
    fclose(outputFiles);
  }
}

/**********************************************************************
** * Function: freeMemory
** * Description: Frees all memory that was allocated in the program
** * Parameters: buildRoom* rooms, int* selectedRooms
** * Pre-Conditions: All memory must be passed in
** * Post-Conditions: All memory must be deleted after the program has run
**********************************************************************/

void freeMemory(buildRoom* rooms, int* selectedRooms)
{
  free(rooms);                                                                  /*Frees all of the memory for buildRoom* rooms type*/
  free(selectedRooms);                                                          /*Frees all of the memory for int* selectedRooms type*/
}
