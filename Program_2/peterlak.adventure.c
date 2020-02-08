/**********************************************************************
** * Program: Adventure
** * Author: Lake Peterson
** * Date: February 7, 2020
** * Description: Creates a game for the user to move through rooms
** * Input: Requires rooms files to be created and have access to info
** * Output: Lets the user know when they have completed and won the game
**********************************************************************/

/**********************************************************************
** * Included Libraries
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h>

/**********************************************************************
** * Global Variables
**********************************************************************/

#define MAX_CONNECTIONS (int)6
#define BUFFER_SIZE (int)1000
#define BUFFER_INPUT (int)100
#define TIME_HOLD (int)10000

/**********************************************************************
** * Change pthread_mutex_t Variable Name
**********************************************************************/

pthread_mutex_t mutexThread;

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
void* createTimeFile();
void outputTimeFile();
void mutexThreading();
void freeMemory(buildRoom*, char*);

/**********************************************************************
** * Function: main
**********************************************************************/

int main()
{
  createGame();

  return 0;
}

/**********************************************************************
** * Function: createGame
** * Description: Creates the general path for the game to be played
** * Parameters: No parameters are needed for this function
** * Pre-Conditions: Must allocate all necessary memory to be passed
** * Post-Conditions: Outlines the necessary steps to create the game
**********************************************************************/

void createGame()
{
  buildRoom* rooms = malloc(7 * sizeof(buildRoom));                             /*Allocates memory for the rooms in the structure.*/
  char* currentRoom = malloc(10 * sizeof(char));                                /*Allocates memory for the random selected rooms.*/

  createRoomStructs(rooms);
  gameInterface(rooms, currentRoom);
  freeMemory(rooms, currentRoom);
}

/**********************************************************************
** * Function: createRoomStructs
** * Description: Fills in all variables that are specified in the struct
** * Parameters: buildRoom* rooms
** * Pre-Conditions: The directory and must be created before hand
** * Post-Conditions: Correctly fills in all attributes contained in the struct
**********************************************************************/

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
  sprintf(directoryPath, "./%s/", recentDir);                                   /*Creates the path that is used to access all of the room files.*/

  directory = opendir(directoryPath);

  while((file = readdir(directory)) != NULL)                                    /*Maintains a loop until their is no more files to go through.*/
  {
    if(file->d_name[0] != '.')
    {
      sprintf(filePath, "%s%s", directoryPath, file->d_name);                   /*Accesses a specific room file, in order to retrieve all of the*/
      openFile = fopen(filePath, "r");                                          /*necessary information to play the game.*/

      while(fgets(fileLine, BUFFER_SIZE, openFile) != NULL)
      {
        sscanf(fileLine, "%*[^:]: %[^]%*[^\n]", storeVal);                      /*Deletes unwanted characters per line.*/
        if(fileLine[5] != 'T' && fileLine[0] != 'C')                            /*Determines if the information is a room name.*/
        {
          rooms[nameIndex].numConnections = 0;
          strcpy(rooms[nameIndex].roomName, storeVal);
          nameIndex++;
        }
        else if(fileLine[5] == 'T' && fileLine[0] != 'C')                       /*Determines if the information is a room type.*/
        {
          rooms[typeIndex].type = storeVal[0];
          typeIndex++;
        }
      }
      fclose(openFile);
    }
  }

  directory = opendir(directoryPath);

  while((file = readdir(directory)) != NULL)                                    /*Maintains a loop until their is no more files to go through.*/
  {
    if(file->d_name[0] != '.')
    {
      sprintf(filePath, "%s%s", directoryPath, file->d_name);                   /*Accesses a specific room file, in order to retrieve all of the*/
      openFile = fopen(filePath, "r");                                          /*necessary information to play the game.*/

      while(fgets(fileLine, BUFFER_SIZE, openFile) != NULL)
      {
        sscanf(fileLine, "%*[^:]: %[^]%*[^\n]", storeVal);                      /*Deletes unwanted characters per line.*/
        if(fileLine[0] == 'C')                                                  /*Determines if the information is a room connection.*/
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

/**********************************************************************
** * Function: findsRecentDir
** * Description: Finds the most recent directory that has been created
** * Parameters: No parameters are needed for this function
** * Pre-Conditions: A directory must exist
** * Post-Conditions: Finds the directory that is the most recent
**********************************************************************/

char* findsRecentDir()
{
  DIR* directory;
  struct stat buffer;
  struct dirent* folder;
  long recentTime = -1;
  char* recentFolder;

  directory = opendir(".");                                                     /*Opens the directory.*/

  while((folder = readdir(directory)) != NULL)                                  /*Loops until it goes through all of the directories.*/
  {
    stat(folder->d_name, &buffer);                                              /*Looks for only directories that meet the assignments requirments.*/

    if(folder->d_name[14] == '.' && folder->d_name[15] != 'c' && buffer.st_mtime >= recentTime)
    {
      recentTime = buffer.st_mtime;                                             /*When finding the most recent it updates the time and directory.*/
      recentFolder = folder->d_name;                                            /*to indicate the most recent version.*/
    }
  }
  closedir(directory);

  return recentFolder;
}

/**********************************************************************
** * Function: indexFinder
** * Description: Finds the index that the chesen value is at
** * Parameters: buildRoom* rooms, char* storeVal, int roomSize
** * Pre-Conditions: Must have access the the users input
** * Post-Conditions: Return the specific index that matches the users input
**********************************************************************/

int indexFinder(buildRoom* rooms, char* storeVal, int roomSize)
{
  int i = 0;

  for(i = 0; i < roomSize; i++)                                                 /*Loops through until a match has been found, when the match.*/
  {                                                                             /*has been found it return the index.*/
    if(strcmp(rooms[i].roomName, storeVal) == 0)
    {
      return i;
    }
  }
  return 0;
}

/**********************************************************************
** * Function: gameInterface
** * Description: Displays the UI
** * Parameters: buildRoom* rooms, char* room
** * Pre-Conditions: Must have access to the rooms
** * Post-Conditions: Diplays UI and also deals with mutex threading
**********************************************************************/

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

  strcpy(currentRoom, findRoom(rooms, roomSize, room));                         /*Copies the current room to be the start room.*/

  while(checkWin == 0)                                                          /*Continues looping until all win requirements have been met.*/
  {
    while(checkInput == 0)                                                      /*Continues looping until user inputs valid input.*/
    {
      printf("CURRENT ROOM: %s\n", currentRoom);                                /*Prints out the current room to the user.*/
      findConnections(rooms, roomSize, currentRoom);
      printf("WHERE TO? >");                                                    /*Asks for user input.*/
      fgets(userInput, BUFFER_INPUT, stdin);
      sscanf(userInput, "%s", savedInput);

      if(strcmp(savedInput, "time") == 0 || strcmp(savedInput, "Time") == 0)    /*If user input is time, then the current time is displayed.*/
      {
        mutexThreading();
        outputTimeFile();
        printf("\n");
      }
      else if(checkConnections(rooms, roomSize, savedInput) == 1)               /*If user input is valid, move to the next room.*/
      {
        checkInput = 1;
        strcpy(currentRoom, savedInput);
        savedIndex = findUserIndex(rooms, roomSize, savedInput);
        userPath[tracker] = savedIndex;
        tracker++;
        printf("\n");
      }
      else                                                                      /*If user input is not valid show the error message.*/
      {
        printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
      }
    }
    if(rooms[savedIndex].type == 'E')                                           /*If user hits the end room, let them know they have won.*/
    {
      printWin(rooms, userPath, tracker);
      checkWin = 1;
    }
    else                                                                        /*Resets second while loop, in order to try and meet the checkWin*/
    {                                                                           /*requirements.*/
      checkInput = 0;
    }
  }
}

/**********************************************************************
** * Function: findRoom
** * Description: Finds the initial room to start at
** * Parameters: buildRoom* rooms, int roomSize, char* room
** * Pre-Conditions: Must have access to the rooms
** * Post-Conditions: Copies the start room into the current room
**********************************************************************/

char* findRoom(buildRoom* rooms, int roomSize, char* room)
{
  int i = 0;

  for(i = 0; i < roomSize; i++)                                                 /*Loops until it find the room type with 's' and returns that room.*/
  {
    if(rooms[i].type == 'S')
    {
      strcpy(room, rooms[i].roomName);                                          /*Copies into current room.*/
    }
  }

  return room;
}

/**********************************************************************
** * Function: findConnections
** * Description: Finds all the connections attached to a specific room
** * Parameters: buildRoom* rooms, int roomSize, char* currentRoom
** * Pre-Conditions: Must have access to the rooms and current room
** * Post-Conditions: Searches for and prints out all connections
**********************************************************************/

void findConnections(buildRoom* rooms, int roomSize, char* currentRoom)
{
  int i = 0;
  int j = 0;

  for(i = 0; i < roomSize; i++)                                                 /*Loops through all of the possible connections.*/
  {
    if(strcmp(currentRoom, rooms[i].roomName) == 0)                             /*Compares the current room to the other seven rooms.*/
    {
      printf("POSSIBLE CONNECTIONS: ");
      for (j = 0; j < rooms[i].numConnections; ++j)
      {
        printf("%s", rooms[rooms[i].connection[j]].roomName);                   /*If conditions are met the connected rooms are printed out.*/
        if(j < (rooms[i].numConnections - 1))
        {
          printf(", ");
        }
      }
      printf(".\n");
    }
  }
}

/**********************************************************************
** * Function: findUserIndex
** * Description: Finds the index of the room that the user chose
** * Parameters: buildRoom* rooms, int roomSize, char* savedInput
** * Pre-Conditions: Must have access to the rooms
** * Post-Conditions: Returns the index that the users choice is at
**********************************************************************/

int findUserIndex(buildRoom* rooms, int roomSize, char* savedInput)
{
  int i = 0;
  int savedIndex = -1;                                                          /*Initilize to -1 in order to avoid any conflicts.*/

  for(i = 0; i < roomSize; i++)                                                 /*Loops through until it finds the correct index.*/
  {
    if(strcmp(rooms[i].roomName, savedInput) == 0)
    {
      savedIndex = i;
    }
  }
  return savedIndex;
}

/**********************************************************************
** * Function: checkConnections
** * Description: Checks all the connections to ensure a match is found
** * Parameters: buildRoom* rooms, int roomSize, char* savedInput
** * Pre-Conditions: Must have access to the rooms
** * Post-Conditions: Returns one if the room looking for exists
**********************************************************************/

int checkConnections(buildRoom* rooms, int roomSize, char* savedInput)
{
  int i = 0;
  int savedIndex = -1;                                                          /*Initilize to -1 in order to avoid any conflicts.*/

  for(i = 0; i < roomSize; i++)                                                 /*Loops through until it finds the correct index.*/
  {
    if(strcmp(rooms[i].roomName, savedInput) == 0)
    {
      savedIndex = i;
      break;
    }
  }

  if(savedIndex == -1)                                                          /*If the saved index is not found return zero.*/
  {
    return 0;
  }

  for(i = 0; i < rooms[savedIndex].numConnections; i++)                         /*Loops through until the users input is found.*/
  {
    if(strcmp(savedInput, rooms[rooms[savedIndex].connection[i]].roomName))
    {
      return 1; /*Found Match*/
    }
  }
  return 0; /*No Match*/
}

/**********************************************************************
** * Function: printWin
** * Description: Prints out the win statement to the user
** * Parameters: buildRoom* rooms, int* userPath, int tracker
** * Pre-Conditions: Must have access to rooms and the history tracker
** * Post-Conditions: Prints out the win
**********************************************************************/

void printWin(buildRoom* rooms, int* userPath, int tracker)
{
  printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");                    /*Prints out the win statement to the user.*/
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", tracker);
  int i = 0;

  for(i = 0; i < tracker; i++)                                                  /*Prints out the users path to reach the win.*/
  {
    printf("%s\n", rooms[userPath[i]].roomName);
  }
}

/**********************************************************************
** * Function: createTimeFile
** * Description: Creates a file that holds the current time
** * Parameters: No parameters are needed for this function
** * Pre-Conditions: Must be able to successfully create a file
** * Post-Conditions: Must successfully put the current time in the file
** * References: "Thread Creation and Termination:." POSIX thread (pthread) libraries, YoLinux,
                  secondary.oslis.org/cite-sources/mla-secondary-citation. Accessed 7 Feb. 2020.
**********************************************************************/

void* createTimeFile()
{
  struct tm* timeTracker;
  time_t currentTime;
  FILE* createFile;
  char space[BUFFER_INPUT];

  createFile = fopen("currentTime.txt", "w+");                                  /*Opens a currentTime file.*/

  currentTime = time(0);
  timeTracker = localtime(&currentTime);
  strftime(space, BUFFER_INPUT, "%Y-%m-%d %H:%M:%S", timeTracker);              /*Writes the current time to the file.*/
  fputs(space, createFile);
  fclose(createFile);
}

/**********************************************************************
** * Function:
** * Description:
** * Parameters: No parameters are needed for this function
** * Pre-Conditions:
** * Post-Conditions:
** * References: "Thread Creation and Termination:." POSIX thread (pthread) libraries, YoLinux,
                  secondary.oslis.org/cite-sources/mla-secondary-citation. Accessed 7 Feb. 2020.
**********************************************************************/

void outputTimeFile()
{
  FILE* outputFile;
  char space[BUFFER_INPUT];

  outputFile = fopen("currentTime.txt", "r+");                                  /*Opens the file to be displayed to the user.*/

  fgets(space, 100, outputFile);
  printf("\n%s\n", space);
  fclose(outputFile);
}

/**********************************************************************
** * Function:
** * Description:
** * Parameters: No parameters are needed for this function
** * Pre-Conditions:
** * Post-Conditions:
** * References: "Thread Creation and Termination:." POSIX thread (pthread) libraries, YoLinux,
                  secondary.oslis.org/cite-sources/mla-secondary-citation. Accessed 7 Feb. 2020.
**********************************************************************/

void mutexThreading()
{
     pthread_t thread;
     int retrieveTime;

     pthread_mutex_init(&mutexThread, NULL);                                    /*Initializes the mutex thread.*/
     pthread_mutex_lock(&mutexThread);                                          /*Locks the mutex thread.*/
     retrieveTime = pthread_create(&thread, NULL, createTimeFile, NULL);
     pthread_mutex_unlock(&mutexThread);                                        /*Unlocks the mutex thread.*/
     pthread_mutex_destroy(&mutexThread);                                       /*Destroys the mutex thread.*/
     usleep(TIME_HOLD);                                                         /*Delays how much time is needed to read, that way the the*/
}                                                                               /*time can be displayed.*/

/**********************************************************************
** * Function: freeMemory
** * Description: Frees all memory that was allocated in the program
** * Parameters: buildRoom* rooms, int* selectedRooms
** * Pre-Conditions: All memory must be passed in
** * Post-Conditions: All memory must be deleted after the program has run
**********************************************************************/

void freeMemory(buildRoom* rooms, char* currentRoom)
{
  free(rooms);                                                                  /*Frees all of the memory for buildRoom* rooms type*/
  free(currentRoom);                                                            /*Frees all of the memory for char* currentRoom type*/
}
