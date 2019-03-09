/**********************************************************************
 * echo-pair.cc
 *
 * Echo server implemented with a channel made of ZeroMQ PAIR sockets.
 *
 **********************************************************************/


#include <thread>
#include <iostream>

#include "util.hh"


// inproc doesn't need any worker threads
const size_t io_threads = 0;

// Hard-coded to cut the code to be bare minimum
const size_t num_messages = 100000;


void server_main(zmq::context_t& context, const char* url)
{
    // We do *NOT* create a second context--in this case, it contains
    // the queues used to communicate.
    zmq::socket_t sock(context, ZMQ_PAIR);

    // Using "connect" because the connection already exists
    sock.connect(url);

    s_send(sock, ""); // We're ready

    // Process messages
    std::string msg;
    for (size_t i=0; i < num_messages; ++i) {
        msg = s_recv(sock);
        s_send(sock, msg);
    }
}


int main()
{
    zmq::context_t context(io_threads);
    zmq::socket_t  sock(context, ZMQ_PAIR);

    const char* url = client_inproc_ep;
    sock.bind(url); // for inproc bind() must happen first

    std::thread server_thread(server_main, std::ref(context), url);

    // Wait until the server thread is ready
    s_recv(sock);

    // Timing data
    auto start = std::chrono::system_clock::now();

    // Send messages synchronously
    std::string msg;
    for (size_t i=0; i < num_messages; ++i) {
        s_send(sock, "The British are coming!  The British are coming!");
        msg = s_recv(sock);
    }

    auto end = std::chrono::system_clock::now();


    std::chrono::duration<double> duration = end - start;
    double elapsed_seconds = duration.count();

    std::cout << "Processed " << num_messages << " messages in "
        << elapsed_seconds << "s" << std::endl;
    std::cout << "(" << num_messages / elapsed_seconds << " msgs/sec)" << std::endl;

    server_thread.join();

    return 0;
}
