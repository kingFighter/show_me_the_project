import socket

HOST, PORT = '', 8888
listenSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listenSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listenSocket.bind((HOST, PORT))
listenSocket.listen(1)

print('Serving the HTTP ON PORT %s...' % PORT)
while True:
    clientConn, clintAddr = listenSocket.accept()
    request  = clientConn.recv(1024)
    print(request)

    httpResponse = '''
HTTP/1.1 200 OK

hello, world!
'''
    clientConn.sendall(httpResponse.encode())
    clientConn.close()