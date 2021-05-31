from socket import *

port = 9190
BUFSIZE = 1024

sock = socket(AF_INET, SOCK_STREAM)
sock.bind(('', port)) # 자신의 주소를 사용
sock.listen(1) 	# 최대 대기 클라이언트 수
print("Waiting for clients...")

# 클라이언트의 연결 요청을 받는다
c_sock, (r_host, r_port) = sock.accept()
print("Connected By ", r_host, r_port)

while True:
	# 상대방 메세지 수신
	data = c_sock.recv(BUFSIZE)
	if data.decode() == 'a':
		break
	print("Received Message : ", data.decode())
	
	# 수신 메세지를 다시 전송
	c_sock.send(data)

c_sock.close()
