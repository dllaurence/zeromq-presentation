#pragma once

/**********************************************************************
 * load-balancing-util.hh
 *
 * Shared code for the echo client-server example.
 *
 **********************************************************************/


#include <string>
#include <zmq.hpp>


 // "Well known" endpoints used by the load-balancing-server nodes
extern const char* client_inproc_ep;
extern const char* client_ipc_ep;
extern const char* client_tcp_ep;

extern const char* worker_inproc_ep;
extern const char* worker_ipc_ep;
extern const char* worker_tcp_ep;

extern const char* log_collect_ep;
extern const char* log_broadcast_ep;


// Facilities to work with std::string rather than zmq::message_t

std::string s_recv (zmq::socket_t & socket); // Receive one frame

bool s_send(zmq::socket_t & socket, const std::string & string); // Send final frame

// Sends string as 0MQ string, as multipart non-terminal
bool s_sendmore (zmq::socket_t & socket, const std::string & string); // Send non-final frame

