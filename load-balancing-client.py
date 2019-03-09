#!/usr/bin/env python
#
# load-balancing-client.py
#
# Simple client for the load-balancing server example

import zmq

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("ipc://echo-client.ipc")
#socket.connect("tcp://127.0.0.1:5555")

log_sock = context.socket(zmq.PUSH)
log_sock.connect("tcp://127.0.0.1:5557")

while (1):
    socket.send(b"Python REQUEST")
    message = socket.recv()
    #log_sock.send(b"Python received: '{0}'".format(message))
    log_sock.send(b"Python client logging")
    print("Received: ", message)
