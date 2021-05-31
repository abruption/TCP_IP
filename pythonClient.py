import socket

port = 9190
address = ("localhost", port)
BUFSIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

while True:
	msg = input("Message to Send : ")
	s.send(msg.encode()) 	# Send a message to server
	r_msg = s.recv(BUFSIZE)	# Receive message from server
	if not r_msg:
		break
	print("Received Message : {0}".format(r_msg.decode()))
