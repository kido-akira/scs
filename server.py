# -*- coding: utf-8 -*-
"""
Server Program Ver.0.0.3
 Time-stamp: <2017-05-13 17:43:34 akira>
"""
import datetime
import logging
import socket

def log_open():
    today = datetime.datetime.today()
    logging.basicConfig(filename=today.strftime('access_%Y%m%d_%H%M%S.log'),
                        format='%(asctime)s:%(levelname)s:%(message)s',
                        level=logging.DEBUG)

def server_routine():
    host = socket.gethostname()
    port = 60606
    serversock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serversock.bind((host, port))
    serversock.listen(10)
    logging.info('Server Start on %s port %d' % (host, port))
    while True:
        (clientsock, clientaddr) = serversock.accept()
        message = clientsock.recv(1024)
        logging.info(clientaddr[0] + "," + message)
        clientsock.send('OK')
        clientsock.close()
        items = message.split(",")
        if items[2] == 'Stop Server':
            break
    logging.info('Server End')

if __name__ == '__main__':
    log_open()
    server_routine()
