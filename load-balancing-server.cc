/**********************************************************************
 * load-balancing-server.cc
 *
 * A server for any number of workers and clients.
 *
 **********************************************************************/


#include <thread>
#include <queue>
#include <iostream>

//#include "zhelpers.hh"

#include "load-balancing-util.hh"


/**********************************************************************
 * Parameters
 *
 **********************************************************************/


const int num_io_threads = 1;
const int num_worker_threads = 1;
const int num_client_threads = 1;

std::thread worker_thread_list[num_worker_threads];
std::thread client_thread_list[num_client_threads];


/**********************************************************************
 * worker_main
 *
 **********************************************************************/


void worker_main(zmq::context_t& context, const char* endpoint)
{
    zmq::socket_t worker_sock(context, ZMQ_REQ);
    worker_sock.connect(endpoint);

    s_send(worker_sock, "READY");
    std::cout << "Worker sent 'READY'" << std::endl;

    for (;;) {
        // Decode incoming frames
        std::string client_addr = s_recv(worker_sock);
        std::string empty = s_recv(worker_sock);
        assert(empty.size() == 0);
        std::string request = s_recv(worker_sock);
        std::cout << "Worker received request: " << request << "'" << std::endl;

        // Assemble outgoing frames
        s_sendmore(worker_sock, client_addr);
        s_sendmore(worker_sock, "");
        s_send    (worker_sock, "REPLY");
        std::cout << "Worker sent 'REPLY'" << std::endl;
    }
}


/**********************************************************************
 * client_main
 *
 **********************************************************************/


void client_main(zmq::context_t& context, const char* endpoint)
{
    zmq::socket_t sock(context, ZMQ_REQ);
    sock.connect(endpoint);

    for (;;) {
        s_send(sock, "C++ REQUEST");
        std::cout << "Client sent 'C++ REQUEST'" << std::endl;

        std::string reply = s_recv(sock);
        std::cout << "Client received reply '" << reply << "'" << std::endl;
    }
}


/**********************************************************************
 * main
 *
 **********************************************************************/


int main(int argc, char *argv[])
{
    zmq::context_t context(num_io_threads);

    // Strict error checking on ROUTER sockets for debugging purposes
    const int router_mandatory_val = 1;

    // Create endpoints
    zmq::socket_t client_sock(context, ZMQ_ROUTER);
    client_sock.setsockopt(ZMQ_ROUTER_MANDATORY, &router_mandatory_val, sizeof(router_mandatory_val));
    client_sock.bind(client_inproc_ep);
    client_sock.bind(client_ipc_ep);
    client_sock.bind(client_tcp_ep);

    zmq::socket_t worker_sock(context, ZMQ_ROUTER);
    worker_sock.setsockopt(ZMQ_ROUTER_MANDATORY, &router_mandatory_val, sizeof(router_mandatory_val));
    worker_sock.bind(worker_inproc_ep);
    worker_sock.bind(worker_ipc_ep);
    worker_sock.bind(worker_tcp_ep);

    // Start internal worker threads
    for (auto& worker_thread: worker_thread_list) {
        worker_thread = std::thread(
                worker_main,
                std::ref(context),
                worker_inproc_ep
                );
    }

    // Start internal client threads
    for (auto& client_thread: client_thread_list) {
        client_thread = std::thread(
                client_main,
                std::ref(context),
                client_inproc_ep
                );
    }

    std::queue<std::string> worker_queue;

    // Main processing loop
    for (;;) {

        zmq::pollitem_t sockets[] = {        // Initialize each loop
            {worker_sock, 0, ZMQ_POLLIN, 0}, // First so we can poll workers only
            {client_sock, 0, ZMQ_POLLIN, 0}
        };

        if (worker_queue.size() > 0) { // If workers are available also poll clients
            zmq::poll(sockets, 2, -1);
        }
        else {
            zmq::poll(sockets, 1, -1);
        }

        if (sockets[0].revents & ZMQ_POLLIN) {      // If we have a ready worker

            // Worker reply is [worker addr][empty][client addr][empty][reply]

            worker_queue.push(s_recv(worker_sock)); // First frame is a worker address

            std::string empty = s_recv(worker_sock); // Discard empty
            assert(empty.size() == 0); // Internal failure if not an empty frame

            std::string client_addr = s_recv(worker_sock);

            // TODO: try simple != comparison rather than this abomination
            if (client_addr.compare("READY") != 0) { // If an actual reply
                std::string empty = s_recv(worker_sock);
                assert(empty.size() == 0);
                std::string reply = s_recv(worker_sock);
                std::cout << "Server has a worker message: '" << reply
                    << "'" << std::endl;

                // Assemble client-side reply frames
                s_sendmore(client_sock, client_addr);
                s_sendmore(client_sock, "");
                s_send    (client_sock, reply);
                std::cout << "Server relayed reply '" << reply << "'" << std::endl;
            }
            else {
                std::cout << "Server has a worker message: '" << client_addr
                    << "'" << std::endl;
            }
        }

        if (sockets[1].revents & ZMQ_POLLIN) {      // If we have a ready client

            // Client request is [client address][empty][request]

            // Extract client request frames
            std::string client_addr = s_recv(client_sock);
            std::string empty = s_recv(client_sock);
            assert(empty.size() == 0);
            std::string request = s_recv(client_sock);
            std::cout << "Server has a client request: '" << request
                << "'" << std::endl;

            // Construct worker request frames
            std::string worker_addr = worker_queue.front();
            worker_queue.pop();

            s_sendmore(worker_sock, worker_addr);
            s_sendmore(worker_sock, "");
            s_sendmore(worker_sock, client_addr);
            s_sendmore(worker_sock, "");
            s_send    (worker_sock, request);
            std::cout << "Server relayed request '" << request << "'" << std::endl;
        }
    }

    for (auto& worker_thread: worker_thread_list) {
        worker_thread.join();
    }

    return 0;
}
