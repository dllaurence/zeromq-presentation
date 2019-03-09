######################################################################
# Makefile for supporting code for ZeroMQ talk
#
######################################################################


CXX = g++

CXXFLAGS += -Wall -Werror -O2 -std=c++14

LDLIBS += -lzmq -lpthread -lstdc++

EXECUTABLES += echo-pair echo-boostq echo-lockingq load-balancing-server

.PHONEY: all
all: $(EXECUTABLES)


######################################################################
# Additional manual dependencies
######################################################################


echo-lockingq.o: locking-queue.hh

load-balancing-util.o: load-balancing-util.hh

echo-pair.o: load-balancing-util.hh
echo-pair: load-balancing-util.o

load-balancing-server.o: load-balancing-util.hh
load-balancing-server: load-balancing-util.o


######################################################################
# Cleanup
######################################################################


.PHONEY: clean
clean:
	rm -rf $(EXECUTABLES) *.o

.PHONEY: spotless
spotless: clean
	rm -rf *~

