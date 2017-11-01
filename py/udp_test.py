import socket
import time

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind(('10.120.10.92',54377))
buf, addr = sock.recvfrom(53000)
# print(addr)
a=[x for x in range(1,5)]
while True:
    print(addr)
    a=[x + 1 for x in a]
    s=str(a)
    sock.sendto(bytes(s, encoding = "utf8") ,addr)
    # print(bytes(s, encoding = "utf8"))
    # print(s)
    time.sleep(0.0001)
sock.close()