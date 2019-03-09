#!/usr/bin/env python
#
# log-server.py
#
# Simple log server for the load-balancing server example


import zmq

from util import *


context = zmq.Context()

collect = context.socket(zmq.PULL)
collect.bind(log_collect_ep)

broadcast = context.socket(zmq.PUB)
broadcast.bind(log_broadcast_ep)

while (1):
    message = collect.recv_multipart()
    print("Received: ", message)
    broadcast.send_multipart(message)
