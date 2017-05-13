# -*- coding: utf-8 -*-
"""
Client Program Ver.0.0.3
 Time-stamp: <2017-05-13 17:20:31 akira>
"""
import socket
from os import getpid
from getpass import getuser

def query(message):
    rc   = False
    host = '192.168.10.37'
    port = 60606
    pid  = getpid()
    user = getuser()
    clientsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        clientsock.settimeout(10) # 10 seconds
        clientsock.connect((host, port))
        clientsock.send("%d,%s,%s" % (pid,user,message))
        answer = clientsock.recv(1024)
        if answer == 'OK':
            rc = True
    except:
        print 'Connection failed.'
    finally:
        clientsock.close()
    return rc

if __name__ == '__main__':
    if query('Client Start'):
        print 'Authentication was successful.'
        query('Client End')
    else:
        print 'Authentication failed.'
