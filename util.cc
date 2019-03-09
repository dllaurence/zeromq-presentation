/**********************************************************************
 * load-balancing-util.cc
 *
 * Shared code for the echo client-server example.
 *
 **********************************************************************/


#include "util.hh"


/**********************************************************************
 * "Well known" endpoints used by the load-balancing-server nodes
 *
 **********************************************************************/


const char* client_inproc_ep = "inproc://echo-client";
const char* client_ipc_ep = "ipc://echo-client.ipc";
const char* client_tcp_ep = "tcp://127.0.0.1:5555";


const char* worker_inproc_ep = "inproc://echo-worker";
const char* worker_ipc_ep = "ipc://echo-worker.ipc";
const char* worker_tcp_ep = "tcp://127.0.0.1:5556";

const char* log_ep = "tcp://127.0.0.1:5557";


/**********************************************************************
 * s_recv
 *
 * Receive 0MQ string from socket and convert into string
 * (from zhelpers.hpp)
 *
 **********************************************************************/


std::string
s_recv (zmq::socket_t & socket)
{
    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char*>(message.data()), message.size());
}


/**********************************************************************
 * s_send
 *
 * Convert string to 0MQ string and send to socket
 * (from zhelpers.hpp)
 *
 **********************************************************************/


//  Convert string to 0MQ string and send to socket
bool s_send (zmq::socket_t & socket, const std::string & string)
{
    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message);
    return (rc);
}


/**********************************************************************
 * s_sendmore
 *
 *  Sends string as 0MQ string, as multipart non-terminal
 * (from zhelpers.hpp)
 *
 **********************************************************************/


//  Sends string as 0MQ string, as multipart non-terminal
bool s_sendmore (zmq::socket_t & socket, const std::string & string)
{
    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message, ZMQ_SNDMORE);
    return (rc);
}

