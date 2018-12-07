Compile the server using: gcc -o ftserver ftserver.c
Then run it using: ./ftserver <portnumber>
Example1: ./ftserver 33326

Compile and run the client using: python ftclient.py <commands>
Example1: python ftclient.py flip2 33326 -g shortfile.txt 30021
Example2: python ftclient.py flip2 33326 -l 30022

The server and client files must be located in separate directories.  Also the server directory must also contain the files 
shortfile.txt and longfile.txt.  The full contents of the server directory should be: ftserver.c, the ftserver.c executable,
shortfile.txt, and longfile.txt. There are no restrictions for the contents of the client directory, but ideally it would only
contain ftclient.c and the ftclient.c executable prior to running the program. 

If a binding error occurs, you can change either the control port number or the data port number on the client program and/or 
the control port number on the server program.  Also the client and server can't be run on different flip servers, they must be
 run on the same flip server. 
