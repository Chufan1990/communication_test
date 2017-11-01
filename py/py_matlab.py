import socket
import time

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind(('127.0.0.1',54377))
sock.listen(5)
# a=[x for x in range(40960,41160)]
a = 12341234
print("start server")
connection, address = sock.accept()
while True:
    # a=[x + 1 for x in a]
    a = a + 1
    s=str(a)
    # connection, address = sock.accept()
    print("client ip is:",address)
    buf = connection.recv(40960)
    connection.send(bytes(s, encoding = "utf8"))
    print(s)
    time.sleep(1)
connection.close()
sock.close()