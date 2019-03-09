#!/usr/bin/env python
#
# load-balancing-worker.py
#
# Simple worker for the load-balancing server example


import zmq

from util import *


context = zmq.Context()
sock = context.socket(zmq.REQ)
sock.connect(worker_ipc_ep)

log_sock = context.socket(zmq.PUSH)
log_sock.connect(log_collect_ep)

sock.send(b"READY")

while True:
    request = sock.recv_multipart()
    print("Worker received: ", request[2])
    request[2] = b"Python REPLY"
    sock.send_multipart(request)
    log_sock.send_multipart([b"worker-python", b"Python worker logging"])
