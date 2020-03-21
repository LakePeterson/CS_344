/**********************************************************************
** * Program: opt_enc
** * Author: Lake Peterson
** * Date: March 13, 2020
** * Description: Performs the client side encryption
** * Input: Reads in the encrypted message from the server
** * Output: Outputs the encrypted message into a destination of choice
**********************************************************************/

/**********************************************************************
** * Included Libraries
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**********************************************************************
** * Global Variables
**********************************************************************/

#define BUFFER (int)100000

/**********************************************************************
** * Prototypes
**********************************************************************/

void error(const char*);

/**********************************************************************
** * Function: main
**********************************************************************/

int main(int argc, char const *argv[])
{
  int socketFD;
  int portNumber;
  int charsWritten;
  int charsRead;
  long int messageLength;
  long int keyLength;
  char messageLengthC[BUFFER];
  char keyLengthC[BUFFER];
  char buffer[BUFFER];
  char message[BUFFER];
  char key[BUFFER];
  FILE* findFileSize;
  FILE* findKeySize;
  FILE* openFile;
  FILE* openKey;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	if(argc != 4)                                                                 // Check usage & args
  {
    fprintf(stderr,"USAGE: %s hostname port\n", argv[0]);
    exit(0);
  }
                                                                                // Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));                   // Clear out the address struct
	portNumber = atoi(argv[3]);                                                   // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET;                                           // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber);                                   // Store the port number
	serverHostInfo = gethostbyname("localhost");                                  // Convert the machine name into a special form of address

  if(serverHostInfo == NULL)                                                    // Ensure that a host has been specified
  {
    fprintf(stderr, "CLIENT: ERROR, no such host\n");
    exit(0);
  }

  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	socketFD = socket(AF_INET, SOCK_STREAM, 0);                                   // Set up the socket and create the socket
	if(socketFD < 0)
  {
    error("CLIENT: ERROR opening socket");
  }

	if(connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect to server and connect socket to address
  {
		error("CLIENT: ERROR connecting");
  }

  openFile = fopen(argv[1], "r");                                               // Open the file provided from the client

  if(openFile == NULL)                                                          // Make sure the file was opened successfully
  {
    error("Failed");
  }
  fgets(message, BUFFER, openFile);                                             // Puts the file into the message variable

  openKey = fopen(argv[2], "r");                                                // Open the key provided from the client

  if(openKey == NULL)                                                           // Make sure the key was opened successfully
  {
    error("Failed");
  }
  fgets(key, BUFFER, openKey);                                                  // Puts the key into the key variable

  findFileSize = fopen(argv[1], "r+");                                          // Find the file size in order determine when when to stop recieving
  fseek(findFileSize, 0L, SEEK_END);
  messageLength = ftell(findFileSize);
  fclose(findFileSize);                                                         // Close the file once finished

  findKeySize = fopen(argv[2], "r+");                                           // Find the key file size in order determine when when to stop recieving
  fseek(findKeySize, 0L, SEEK_END);
  keyLength = ftell(findKeySize);
  fclose(findKeySize);                                                          // Close the key file once finished

  sprintf(messageLengthC, "%d", messageLength);                                 // Convert the message length into a character
  sprintf(keyLengthC, "%d", keyLength);                                         // Convert the key length into a character

  /*****************************************************************SEND THE MESSAGE LENGTH*****************************************************************/

  charsWritten = send(socketFD, messageLengthC, strlen(messageLengthC), 0);     // Send message to server and write to the server

	if(charsWritten < 0)                                                          // Checks for an error when writing to the socket
  {
    error("CLIENT: ERROR writing to socket");
  }
	if(charsWritten < strlen(messageLengthC))                                     // Checks to make sure all the data has transferred across socket
  {
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }

	memset(buffer, '\0', BUFFER);                                                 // Get return message from server and clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, BUFFER, 0);                                // Read data from the socket, leaving \0 at end

  if(buffer[0] != 'x')                                                          // Checks for a valid connection between the sockets
  {
    fprintf(stderr, "ERROR Not A Valid Connection");
    exit(2);
  }

	if(charsRead < 0)                                                             // Checks for an error when reading from the socket
  {
    error("CLIENT: ERROR reading from socket");
  }

  /*******************************************************************SEND THE KEY LENGTH*******************************************************************/

  charsWritten = send(socketFD, keyLengthC, strlen(keyLengthC), 0);             // Send message to server and write to the server

	if(charsWritten < 0)                                                          // Checks for an error when writing to the socket
  {
    error("CLIENT: ERROR writing to socket");
  }
	if(charsWritten < strlen(keyLengthC))                                         // Checks to make sure all the data has transferred across socket
  {
    fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");
  }

  memset(buffer, '\0', BUFFER);                                                 // Get return message from server and clear out the buffer again for reuse
  charsRead = recv(socketFD, buffer, BUFFER, 0);                                // Read data from the socket, leaving \0 at end

  if(charsRead < 0)                                                             // Checks for an error when reading from the socket
  {
    error("CLIENT: ERROR reading from socket");
  }

  /*******************************************************************SEND THE MESSAGE********************************************************************/

  charsWritten = send(socketFD, message, strlen(message), 0);                   // Send message to server and write to the server

	if(charsWritten < 0)                                                          // Checks for an error when writing to the socket
  {
    error("CLIENT: ERROR writing to socket");
  }
	if(charsWritten < strlen(message))                                            // Checks to make sure all the data has transferred across socket
  {
    fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");
  }

  memset(buffer, '\0', BUFFER);                                                 // Get return message from server and clear out the buffer again for reuse
  charsRead = recv(socketFD, buffer, BUFFER, 0);                                // Read data from the socket, leaving \0 at end

  if(charsRead < 0)                                                             // Checks for an error when reading from the socket
  {
    error("CLIENT: ERROR reading from socket");
  }

  /*********************************************************************SEND THE KEY**********************************************************************/

  charsWritten = send(socketFD, key, strlen(key), 0);                           // Send message to server and write to the server

	if(charsWritten < 0)                                                          // Checks for an error when writing to the socket
  {
    error("CLIENT: ERROR writing to socket");
  }
	if(charsWritten < strlen(key))                                                // Checks to make sure all the data has transferred across socket
  {
    fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");
  }

  memset(buffer, '\0', BUFFER);                                                 // Get return message from server and clear out the buffer again for reuse
  charsRead = recv(socketFD, buffer, BUFFER, 0);                                // Read data from the socket, leaving \0 at end

  if(charsRead < 0)                                                             // Checks for an error when reading from the socket
  {
    error("CLIENT: ERROR reading from socket");
  }

  /*****************************************************************SEND THE ENC MESSAGE******************************************************************/

  memset(buffer, '\0', BUFFER);                                                 // Get return message from server and clear out the buffer again for reuse
  charsRead = recv(socketFD, buffer, BUFFER, 0);                                // Read data from the socket, leaving \0 at end

  if(charsRead < 0)                                                             // Checks for an error when reading from the socket
  {
    error("CLIENT: ERROR reading from socket");
  }

  printf("%s", buffer);                                                         // Adds read data to the destination of choice
  messageLength -= strlen(buffer);                                              // Decrements the length

  while(messageLength != 0)                                                     // Loop until all data has been read correctly
  {
    if(strlen(buffer) == 0)                                                     // Check for when there is nothing in the file
    {
      break;
    }
    else
    {
      memset(buffer, '\0', BUFFER);                                             // Clear out the buffer again for reuse
      charsRead = recv(socketFD, buffer, BUFFER, 0);                            // Read data from the socket, leaving \0 at end

      if(charsRead < 0)                                                         // Checks for an error when reading from the socket
      {
        error("CLIENT: ERROR reading from socket");
      }
      messageLength -= strlen(buffer);                                          // Decrements the length
      printf("%s", buffer);                                                     // Adds read data to the destination of choice
    }
  }

  close(socketFD);                                                              // Close the socket

  return 0;
}

/**********************************************************************
** * Function: error
** * Description: Prints out an error message
** * Parameters: const char* message
** * Pre-Conditions: User must indicate what error message they want
** * Post-Conditions: The exit status must be one
**********************************************************************/

void error(const char* message)
{
  perror(message);                                                              // Error function used for reporting issues
  exit(1);
}
