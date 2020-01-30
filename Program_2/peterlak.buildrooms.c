/**********************************************************************
** * Program:
** * Author: Lake Peterson
** * Date:
** * Description:
** * Input:
** * Output:
**********************************************************************/

#include <stdio.h>
#include <string.h>

#define MIN_CONNECTIONS (int)3
#define MAX_CONNECTIONS (int)6

void createRoomStructs();
// void createDirectory();
// void createRoomNames(char*);

typedef struct
{
  //struct buildRoom* connection[MAX_CONNECTIONS];
  int numConnections;
  char roomName[6];
  char type;
}buildRoom;

int main()
{
  // createDirectory();

  createRoomStructs();


  return 0;
}

void createRoomStructs()
{
    // struct buildRoom* room[7];
    // char* letterRooms[7] = {"a_room", "b_room", "c_room", "d_room", "e_room", "f_room", "g_room"};
    // int roomSize = 7;
    // int i;

    // printf("HRERERER");



    buildRoom rooms[7];
    strcpy(rooms[0].roomName, "Crowther");
    printf("%s\n", rooms[0].roomName);


    //printf("%s\n", roomList[0]->roomName);



    //
    // printf("%s\n", room[0]->roomName);

    // for(i = 0; i < roomSize; i++)
    // {
    //   strcpy(names[0]->roomNames, rooms[i]);
    // }


    // for(i = 0; i < roomSize; i++)
    // {
    //
    //
    //
    //
    //   sprintf(filePath, "./%s/%s", directory, roomName[i]);
    //   outFile = fopen(filePath, "w");
    // }

    // struct Room* roomList[7];
    // strcpy(roomList[0]->name, "Crowther");
    // strcpy(roomList[1]->name, "Dungeon");
    // strcpy(roomList[2]->name, "PLUGH");
    // strcpy(roomList[3]->name, "PLOVER");
    // strcpy(roomList[4]->name, "twisty");
    // strcpy(roomList[5]->name, "XYZZY");
    // strcpy(roomList[6]->name, "Zork");

}


// void createDirectory()
// {
//   int getPID = getpid();
//   char stringPID[50];
//   char directory[100] = "peterlak.buildrooms.";
//
//   sprintf(stringPID, "%d", getPID);
//   strcat(directory, stringPID);
//
//   mkdir(directory, 0700);
//
//   createRoomNames(directory);
// }
//
// void createRoomNames(char* directory)
// {
//   char filePath[100];
//   char* roomName[7] = {"a_room", "b_room", "c_room", "d_room", "e_room", "f_room", "g_room"};
//   FILE* outFile;
//   int roomSize = 7;
//   int i;
//
//   for(i = 0; i < roomSize; i++)
//   {
//     sprintf(filePath, "./%s/%s", directory, roomName[i]);
//     outFile = fopen(filePath, "w");
//   }
//
//   fclose(outFile);
// }
