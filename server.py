# -*- coding: utf-8 -*-
"""
Server Program Ver.0.0.3
 Time-stamp: <2017-05-13 17:43:34 akira>
"""
import sys
import datetime
import logging
import socket

def log_open(basename):
    today = datetime.datetime.today()
    logfile=today.strftime(basename + '_%Y%m%d_%H%M%S.log')
    logging.basicConfig(filename=logfile,
                        format='%(asctime)s:%(levelname)s:%(message)s',
                        level=logging.DEBUG)

def server_routine(port):
    host = socket.gethostname()
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
	if len(sys.argv) < 3:
		print 'usage: python %s port_number log_base_name' % sys.argv[0]
		quit()
	port = int(sys.argv[1])
	basename = sys.argv[2]
	log_open(port)
	server_routine(basename)
