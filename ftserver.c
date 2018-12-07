/*
Using code from OSU's operating systems class: client.c  
This program sets up and creates the client.  It uses the host name and port number that was entered in on the command line 
at run time to connect to the corresponding server. It uses the struct sockaddr_in serveraddress to set up the socket.  It 
also uses a do/while loop to prompt the user for input and place it into a buffer.  The client user can type "/quit" to close
the client side of the socket connection.  The sendmessage and receivemessage functions are used to receive and send messages.

I also used code from https://unix.stackexchange.com/questions/96226/delete-first-line-of-a-file in order to get the code for the 
"tail -n +4 directory.txt > directory2.txt" command in unix.

I also used code from https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c to get code for finding the
size of a file using fseek, ftell, and rewind.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h> 

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

/*This function creates the socket for the server to listen on using the socket, bind, and listen functions.  It also
returns the listenSocketFD integer back to main.*/
int startup(int listenSocketFD, struct sockaddr_in serverAddress)
{
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections
	return listenSocketFD;
}
/*This function handles the request of the client by establishing a socket connection after the client has initiated contact.
It uses the accept function which returns an integer for the socket connection.  The handleRequest function then returns 
this integer to main.*/
int handleRequest(int establishedConnectionFD, int listenSocketFD, struct sockaddr_in clientAddress, int sizeOfClientInfo)
{
	establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
	
	if (establishedConnectionFD < 0) error("ERROR on accept");
	return establishedConnectionFD;
}
int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	int socketFD, charsWritten;
	char buffer[100];
	char buffer2[100];
	char buffer3[100];
	char buffer4[100];
	char buffer5[10000];
	char buffer6[100];
	
	char buffer7[1030000];
	char buffer11[1000];
	struct sockaddr_in serverAddress, clientAddress;
	FILE * f;
	struct hostent* serverHostInfo;
	
	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args
	
	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process
	listenSocketFD = 0;
	
	// Set up the socket
	listenSocketFD = startup(listenSocketFD, serverAddress);	
	
    
	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	char * buf = (char *) malloc((sizeof(char)) * 70000 + 1);
		memset(buf, '\0', sizeof(buf));
	char * buf2 = (char *) malloc((sizeof(char)) * 70000 + 1);
		memset(buf2, '\0', sizeof(buf2));
		
	
	//The following do/while loop iterates until the user kills the process or a segmentation fault occurs
	do{
		
	establishedConnectionFD = handleRequest(establishedConnectionFD, listenSocketFD, clientAddress, sizeOfClientInfo);
	

	//Get the plaintext message from the client and display it
	
	
	memset(buffer, '\0', sizeof(buffer));	
	charsRead = recv(establishedConnectionFD, buffer, 5, 0); // Receives the flip server name from the client
	if (charsRead < 0) error("ERROR reading from socket");
	printf("Connection from %s\n", buffer); 
	
	memset(buffer2, '\0', sizeof(buffer2));	
	charsRead = recv(establishedConnectionFD, buffer2, 2, 0); // Receives the -l or -g command from the client
	if (charsRead < 0) error("ERROR reading from socket");
	
	memset(buffer3, '\0', sizeof(buffer3));	
	charsRead = recv(establishedConnectionFD, buffer3, 5, 0); // Receives the data port number from the client
	if (charsRead < 0) error("ERROR reading from socket");
	
	memset(buffer4, '\0', sizeof(buffer4));	
	charsRead = recv(establishedConnectionFD, buffer4, 100, 0); // Receives the filename from the client if there is one
	if (charsRead < 0) error("ERROR reading from socket");
	printf("File \"%s\" requested on port %s.\n", buffer4, buffer3);
  
	memset(buffer7, '\0', sizeof(buffer7));	
	
	int x = 0;
	int sz = 0;
	int sz2 = 0;
	
	//The following section creates the hostname by concatenating the strings of the flip server from the client and 
	//the suffix .engr.oregonstate.edu.  It also places the hostname into buffer8.
	char buffer8[100];
	char hostnamez[100] = ".engr.oregonstate.edu";
	strcpy(buffer8, buffer);
	strcat(buffer8, hostnamez);
	
	//The following section creates the data port socket connection from the server to the client.  It uses the data port
	//number from buffer3 and the hostname from buffer 8.
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(buffer3); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(buffer8); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); 
	
	
	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");
	int j, m;
	
	/*The following if statement runs if the command '-l' was sent from the client.  It uses the system call to place the 
	contents of the directory into a text file called directory.txt using a linux command.  Then it uses another linux 
	command to truncate the file to exclude the directory text file, the ftserver file, and the ftserver executable file. 
	It then reads the data from directory file into buffer 7 and places the output data for the server into buffer5.  Then
	it removes the directory text files using a linux command. */
	memset(buffer5, '\0', sizeof(buffer5));
	if(buffer2[1] == 'l')
	{
		x = 3;

		j = system("ls > directory.txt");
		j = system("tail -n +4 directory.txt > directory2.txt");

	FILE * g = fopen("directory2.txt", "r");	
	fread(buffer7, 1000, 1, g);
	fclose(g);	
			
	strcpy(buffer5, "Receiving directory structure from ");
	strcat(buffer5, buffer);
	strcat(buffer5, ":");
	strcat(buffer5, buffer3);
	strcat(buffer5, "\n");
	
	j = system("rm -f directory.txt");
	j = system("rm -f directory2.txt");
	}
	/*The following section runs if the command '-g' was sent from the client.  It uses system call to find the filename that
	was sent in buffer4 using a linux command. It creates a new file and if there's data in the file, then the file that was
	requested by the client is present in the server's directory. The new file is removed using a linux command whether or not
	it has data in it. If it has data, then the file name that was stored in buffer4 will be opened and the size of the file is 
	calculated and stored.  The value x is changed depending on the file name and saved for later use.  If it's empty then the 
	server's directory doesn't contain that file and an error message is copied into buffer7 to be sent to the client and an 
	error message is printed out on the server.	*/
	memset(buffer6, '\0', sizeof(buffer6)); 
	if(buffer2[1] == 'g')
	{
		strcpy(buffer6, "find . -name ");
		strcat(buffer6, buffer4);
		strcat(buffer6, "> newfile.txt");

		m = system(buffer6);
		memset(buffer6, '\0', sizeof(buffer6));
		
		FILE * h = fopen("newfile.txt", "r");
		int size;
		if (h != NULL) 
		{
		fseek (h, 0, SEEK_END);
		size = ftell(h);

			if (0 == size) {
				x = 19;
				printf("File not found. Sending error message to %s:%s\n", buffer, buffer3);
				strcpy(buffer5, buffer);
				strcat(buffer5, ":");
				strcat(buffer5, buffer3);
				strcat(buffer5, " says FILE NOT FOUND");
				fclose(h);
				m = system("rm -f newfile.txt");
			}
			else{
				m = system("rm -f newfile.txt");

				strcpy(buffer5, "Receiving ");
				strcat(buffer5, buffer4);
				strcat(buffer5, " from ");
				strcat(buffer5, buffer);
				strcat(buffer5, ":");
				strcat(buffer5, buffer3);
				strcat(buffer5, "\n");

				if(strcmp(buffer4,"longfile.txt") == 0)
				{
					x = 1;
					FILE * v = fopen(buffer4, "r");
					fseek(v, 0, SEEK_END);
					sz = ftell(v);
					rewind(v);					
				}
				else
				{
					x = 2;
					FILE * b = fopen(buffer4, "r");
					fseek(b, 0, SEEK_END);
					sz2 = ftell(b);
					rewind(b);					
				}
			}
		}					
	}
	
		//This section sends the message that was placed in buffer5 to the client over the data port socket.
		charsWritten = send(socketFD, buffer5, strlen(buffer5), 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");//If charsWritten is less then 0 an error is displayed
		if (charsWritten < strlen(buffer5)) printf("CLIENT: WARNING: Not all data written to socket!\n");
    
	//If x is 19, that means the file the client requested wasn't in the server's directory, so there's no file data to send 
	//and the data port socket must be closed.
	if(x == 19)	
		close(socketFD);
	
	/*If x is 1, then the file longfile.txt was requested by the client.  So the buffer that is used is much larger than
	the buffer that is used in shortfile.txt.  Here, the size of the file is divided by 1000000 in order to get the number of 
	iterations that the for loop should run.  Then the extra data is calculated by using the modulus operator.  The amount 
	of data that fread places into buffer7 changes depending upon the iteration that the for loop is in.  The data in buffer7 
	is then sent over the data port socket to the client and then the socket is closed. */
	if(x == 1)
	{
		FILE * v = fopen(buffer4, "r");
		int y = 0;
		int totalchars = 0;
		int iterations = sz/1000000; 
		int extra = sz % 1000000;
		
		for(y = 0; y < iterations+1; y++)
		{
			if(y <= iterations-1)
			{
				fread(buffer7, 1000000, 1, v);
			}
			else	
			{
				memset(buffer7, '\0', sizeof(buffer7));
				fread(buffer7, extra, 1, v);
			}
			
		charsWritten = send(socketFD, buffer7, strlen(buffer7), 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");//If charsWritten is less then 0 an error is displayed
		if (charsWritten < strlen(buffer7)) printf("CLIENT: WARNING: Not all data written to socket!\n");
		}
		close(socketFD);
	}
	/*If x is 2, then the file shortfile.txt was requested by the client.  So the buffer that is used is much smaller than
	the buffer that is used in longfile.txt.  Here, the size of the file is divided by 1000 in order to get the number of 
	iterations that the for loop should run.  Then the extra data is calculated by using the modulus operator.  The amount 
	of data that fread places into buffer7 changes depending upon the iteration that the for loop is in.  The data in buffer7 
	is then sent over the data port socket to the client, and then the socket is closed. */
	if(x == 2)
	{
		FILE * fp = fopen(buffer4, "r");
		int y = 0;
		int totalchars = 0;
		
		int iterations = sz2/1000; 
		int extra = sz2 % 1000;
		for(y = 0; y < iterations+1; y++)
		{
			if(y <= iterations-1)
			{
				fread(buffer7, 1000, 1, fp);
			}
			else
			{
				memset(buffer7, '\0', sizeof(buffer7));
				fread(buffer7, extra, 1, fp);
			}
		
		charsWritten = send(socketFD, buffer7, strlen(buffer7), 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");//If charsWritten is less then 0 an error is displayed
		if (charsWritten < strlen(buffer7)) printf("CLIENT: WARNING: Not all data written to socket!\n");
		}
		close(socketFD);
	}
	/*If x is 3, then the directory list command was sent.  In this case, buffer7 already contains the data that it needs to 
	send, so the data is sent over to the client using the data port socket.*/
	if(x == 3)
	{
		charsWritten = send(socketFD, buffer7, strlen(buffer7), 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");//If charsWritten is less then 0 an error is displayed
		if (charsWritten < strlen(buffer7)) printf("CLIENT: WARNING: Not all data written to socket!\n");
		close(socketFD);
	}
	
	}while(1);
	//The following statements flush out stderr and stdout
	fflush(stderr);
	fflush(stdout);
	

	return 0; 
}
