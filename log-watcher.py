#!/usr/bin/env python
#
# log-watcher.py
#
# Simple log watcher for the load-balancing server example


import sys
import zmq

from util import *


context = zmq.Context()

sub = context.socket(zmq.SUB)
sub.connect(log_broadcast_ep)

if len(sys.argv) > 1:
    for prefix in sys.argv:
        sub.setsockopt(zmq.SUBSCRIBE, prefix.encode("ascii"))
else:
    sub.setsockopt(zmq.SUBSCRIBE, b"")

while (1):
    message = sub.recv_multipart()
    print(message[0].decode("utf-8"), ": ", message[1].decode("utf-8"))
