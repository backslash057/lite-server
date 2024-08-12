import socket


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect(("127.0.0.1", 5050))

while True:
	try:
		sock.send(input('>').encode('utf8'))
	except KeyboardInterrupt: pass

sock.close()
