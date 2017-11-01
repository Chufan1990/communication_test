import socket
import sys

HOST, PORT = "localhost", 54377

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((HOST, PORT))
for x in range(0, 10000):
    print("Step 1")
    s.send(b'Hello')
    print("Step 2")
    print(str(s.recv(1000)))
    print(x)