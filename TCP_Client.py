import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 서버 주소 입력
svrIP = input(('Server IP(default : 127.0.0.1) : '))
if svrIP == '':
	svrIP = '127.0.0.1' # 기본 주소

# 포트 번호 입력
port = input('port(default : 9190) : ')
if port == '':
	port = 9190	# 기본 주소
else:
	port = int(port)

sock.connect((svrIP, port))
print('Connected to {0}'.format(svrIP))

while True:
	msg = input("Sending Message : ")

	# 송신 데이터가 없으면 다시 진행
	if not msg:
		continue 
	try:	# 데이터 전송
		sock.send(msg.encode())	# 메세지 전송
	except:	# 연결이 종료됨
		print('연결이 종료되었습니다.')
		break
	
	try:
		msg = sock.recv(1024)
		if not msg:
			print('연결이 종료되었습니다.')
			break
		print('Received Message : {0}'.format(msg.decode()))

	except:	# 연결이 종료됨
		print('연결이 종료되었습니다.')
		break

sock.close()
