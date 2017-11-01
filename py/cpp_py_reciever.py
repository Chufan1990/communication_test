import socket
import time

HOST = ''
PORT = 54377
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
a = [x for x in range(1,100)]
while True:
    # conn, addr = s.accept()
    data = conn.recv(1024)
    print(data)
    a = [x + 1 for x in a]
    string = str(a)
    conn.sendall(bytes(string,encoding = "utf8"))
    # print(s)
    time.sleep(1)
conn.close()
socket.close()