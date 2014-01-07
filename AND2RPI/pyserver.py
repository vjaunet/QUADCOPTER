import socket
import json
from time import *

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('0.0.0.0',7000))

start = time()

#chksum calculation
def chksum(str):
  c = 0
  for a in str:
    c = ((c + ord(a)) << 1)% 256
  return c

#main loop
def main():
  while True:
    # wait for UDP packet
    data,addr= sock.recvfrom(1024)
    data = data.replace("'", "\"");

    # parse it
    p = json.loads(data)

    # if control packet, send to ardupilot
    if p['type'] == 'rcinput':
      str = "%d,%d,%d,%d" % (p['roll'], -p['pitch'], p['thr'], p['yaw'])

      #calc checksum
      chk = chksum(str)

      #concatenate msg and chksum
      output = "%s*%2x\r" % (str, chk)
      print output

main()
