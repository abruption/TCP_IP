import socket

# 숫자에 대한 영어 사전
table = {'1':'one', '2':'two', '3':'three', '4':'four', '5':'five', '6':'six', '7':'seven', '8':'eight', '9':'nine', '10':'ten'}

s = socket.socket()	# AF_INET, SOCK_STREAM
address = ('', 9190)
s.bind(address)
s.listen(1)
print('Waiting...')
c_socket, c_addr = s.accept()
print('Connection from : {0}'.format(c_addr))

while True:
	data = c_socket.recv(1024).decode()	# 요청 수신
	try:
		resp = table[data]	# 데이터를 Key로 사용하여 Value를 가져온다
	except:
		c_socket.send('Try Again'.encode())	# 오류가 있을 때
	else:
		c_socket.send(resp.encode())	# 변환 값을 전송
