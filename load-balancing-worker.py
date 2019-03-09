#!/usr/bin/env python
#
# load-balancing-worker.py
#
# Simple worker for the load-balancing server example

import zmq

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("ipc://echo-worker.ipc")
#socket.connect("tcp://127.0.0.1:5556")

log_sock = context.socket(zmq.PUSH)
log_sock.connect("tcp://127.0.0.1:5557")

socket.send(b"READY")

while (1):
    request = socket.recv_multipart()
    print("Worker received: ", request[2])
    request[2] = b"Python REPLY"
    socket.send_multipart(request)
    log_sock.send(b"Python worker logging")
