
import socket
import sys
from thread import *

#Set the socket config as TCP 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#Bind to port from command line argument
try:
    s.bind(('', int(sys.argv[1])))
except:
    print 'Bind failed.'
    sys.exit()
 
#start listening on port for requests
s.listen(10)


#loop that is listening for new connections
while 1: 
	conn, addr = s.accept()
        
	tempy = 1
	while tempy ==1:

			#If no data, client ended connection
			data = conn.recv(1024)
			if not data:
				print("connection closed by client")	
				tempy = 0
				#get data from user
			else:
				print('{}').format(data)
				#get user input and ensure not too long	
				text = raw_input()
				
				while   len(text) > 500:
					print("Input too long")
					text = raw_input()
					#exit if user enters \quit
				if text == "\quit":
					conn.close()
					tempy = 0
				else:
					#send input 
					text="server>"+text
					conn.send(text)
