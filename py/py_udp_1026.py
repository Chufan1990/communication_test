import time
from socket import *

for pings in range(10):
    clientSocket = socket(AF_INET, SOCK_DGRAM)
    clientSocket.settimeout(1)
    message = '1010'
    addr = ("127.0.0.1", 25000)

    start = time.time()
    clientSocket.sendto(bytes(message,encoding = "utf8"), addr)
    try:
        data, server = clientSocket.recvfrom(1024)
        end = time.time()
        elapsed = end - start
        print(data, pings, elapsed)
    except timeout:
        print('REQUEST TIMED OUT')