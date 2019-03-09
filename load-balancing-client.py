#!/usr/bin/env python
#
# load-balancing-client.py
#
# Simple client for the load-balancing server example


import zmq

from util import *


context = zmq.Context()
sock = context.socket(zmq.REQ)
sock.connect(client_tcp_ep)

log_sock = context.socket(zmq.PUSH)
log_sock.connect(log_collect_ep)

while True:
    sock.send(b"Python REQUEST")
    reply = sock.recv()
    #log_sock.send(b"Python received: '{0}'".format(reply))
    log_sock.send_multipart([b"client-python", b"Python client logging"])
    print("Received: ", reply)
