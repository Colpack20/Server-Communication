'''

This program creates and connects to the server using the makeRequest function.  It closes that socket once it's finished 
sending arguments.  It then creates a server socket while in the initiateContact function and then waits for the client to 
connect to it on the corresponding port. It then receives the data from a message or file using the recv function and, if
it's receiving a file, it writes that data to a new file and prints out a file transfer complete message when it's done.   
  
'''
 
import socket
import sys
from thread import *

'''The following section takes looks at argument #3 from the command line and depending on if it is -l or -g, the port number
is found in either the fourth argument or the fifth argument.  Also counter is set to either 1 or 2 depending on what the
third argument is and saved for later. The control port number is always the second argument. ''' 
HOST = ''   # Symbolic name meaning all available interfaces
counter = 0
if sys.argv[3] == "-l":
	PORT = int(sys.argv[4])
	counter = 1
if sys.argv[3] == "-g":
	PORT = int(sys.argv[5])
	counter = 2
CONTROLPORT = int(sys.argv[2])

'''
This function starts up the server's socket and then waits for the client to connect to it before messages are send and
received from the server. It creates the socket using the socket function and places it into the variable s.  Next it
binds the socket to the local host and port using the bind function on s.  An error code will print if it can't bind, 
otherwise it'll print a completion message.  Then it uses the listen function on s to wait for a client to connect to it.
Finally, it returns the s variable.  
'''
def initiateContact(): 
	
	makeRequest()
	
	t = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#Bind socket to local host and port
	try:
		t.bind((HOST, PORT))
	#If the socket fails to bind, an error message will print	
	except socket.error as msg:
		print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
		sys.exit()
 
	#Start listening on socket
	t.listen(10)

	return t
''' 
The function makeRequest creates the control port socket using the socket function.  It then connects to the server and
sends it the commands that were sent to the client from the command line usings the sys function.  Depending on what arguments
are sent, counter is either 1 or 2 and the corresponding commands are sent to the server. Then, the control socket is closed.
'''
def makeRequest():
	#Create the control port socket
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	s.connect((HOST, CONTROLPORT))
	#The flip server name is always sent out
	s.send(sys.argv[1])
	#If counter is 1, then the -l command and data port number are sent
	if counter == 1:
		s.send(sys.argv[3])
		s.send(sys.argv[4])
	#If counter is 2, then the -g command, file name, and the data port number are sent
	if counter == 2:
		s.send(sys.argv[3])
		s.send(sys.argv[5])
		s.send(sys.argv[4])
	#close the socket
	s.close()
	
'''
The receiveFile function receives messages and files from the server. It also writes that data from the files it receives 
into it's own similarly titled files.  
'''
def receiveFile(conn):
	
	#filesize = 0
	message = 0	
	
	#This section receives the first message from the server and prints it out.  
	messge = conn.recv(1000)
	print messge
	
	'''This section runs if the -g command is used and the filename is shortfile.txt. Because of the shortfile's size, a 
	for loop is iterated 24 times. The recv function receives 43776 bytes at a time, so it only has to iterate twice, but it
	could potentially handle larger file sizes. A new file is opened and created using the name shortfile.txt on the first 
	iteration. Then data is written to it using the write function, and then it's closed.  The file is then appended to on
	later iterations.  When the data is less than the size of the buffer, 43776, and the message variable is still at zero, 
	the file transfer message will print, since all the file data had been sent successfully. '''
	if counter == 2:
		if sys.argv[4] == "shortfile.txt":
			for num in range (1,25):
				name = conn.recv(43776)
				#filesize = filesize + len(name)
				if num == 1:
					f= open("shortfile.txt","w+")
					f.write(name)
					f.close()
				if num > 1:
					f= open("shortfile.txt", "a")
					f.write(name)
					f.close()
					
				if (len(name) < 43776) and (message == 0):
					print "File transfer complete."
					message = 1
	'''This section runs if the -g command is used and the filename is longfile.txt. Because of the longfile's size, a for loop 
	is iterated 24 times. The recv function receives 43776 bytes at a time, so it has to iterate 2354 times to get the entire 
	size of the longfile. It could potentially handle smaller sizes too. A new file is opened and created using the name 
	longfile.txt on the first iteration. Then data is written to it using the write function, and then it's closed.  The file is 
	then appended to on later iterations.  When the data is less than the size of the buffer, 43776, and the message variable 
	is still at zero, the file transfer message will print, since all the file data had been sent successfully. '''
	if counter == 2:
		if sys.argv[4] == "longfile.txt":
			for num in range(1,2355):
				name = conn.recv(43776)
				#filesize = filesize + len(name)
				if num == 1:
					f= open("longfile.txt","w+")
					f.write(name)
					f.close()
				if num > 1:
					f= open("longfile.txt", "a")
					f.write(name)
					f.close()
				if (len(name) < 43776) and (message == 0):
					print "File transfer complete."
					message = 1
					
    #close socket
	conn.close()

#Start the client and server	
t = initiateContact() 

#wait to accept a connection - blocking call
conn, addr = t.accept()
     
#start receiving and downloading the file data over the dataport connection
receiveFile(conn)

#close the socket
t.close()
