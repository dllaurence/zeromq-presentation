#!/usr/bin/env python
#
# load-balancing-logger.py
#
# Simple log server for the load-balancing server example

import zmq

context = zmq.Context()
sock = context.socket(zmq.PULL)
sock.bind("tcp://127.0.0.1:5557")

while (1):
    message = sock.recv()
    print("Received: ", message)
