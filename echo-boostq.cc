/**********************************************************************
 * echo-boostq.cc
 *
 * Echo server implemented with a channel made with a pair of lock-free
 * Boost queues.
 *
 **********************************************************************/


#include <thread>
#include <iostream>
#include <cstring>

#include <boost/lockfree/queue.hpp>


// Hard-coded to cut the code to a bare minimum
const size_t num_messages = 1000000;


typedef boost::lockfree::queue<char*> Queue;


void server_main(Queue& request_q, Queue& reply_q)
{
    // Send so the client knows we're ready
    char* msg = nullptr;
    reply_q.push(msg);

    // Process messages
    for (size_t i=0; i < num_messages; ++i) {
        request_q.pop(msg);
        reply_q.push(msg);
    }
}


int main()
{
    Queue request_q(100);
    Queue reply_q(100);

    std::thread echo_server_thread(server_main,
            std::ref(request_q), std::ref(reply_q));

    // Wait until the server thread is ready
    char* msg;
    reply_q.pop(msg);

    // Timing data
    auto start = std::chrono::system_clock::now();

    // Send messages synchronously
    for (size_t i=0; i < num_messages; ++i) {
        // TODO: don't re-allocate so much?
        msg = strdup("The British are coming!  The British are coming!");
        assert(msg);
        request_q.push(msg);

        reply_q.pop(msg);
        free(msg);
        msg = nullptr;
    }

    auto end = std::chrono::system_clock::now();


    std::chrono::duration<double> duration = end - start;
    double elapsed_seconds = duration.count();

    std::cout << "Processed " << num_messages << " messages in "
        << elapsed_seconds << "s" << std::endl;
    std::cout << "(" << num_messages / elapsed_seconds << " msgs/sec)" << std::endl;

    echo_server_thread.join();

    return 0;
}
