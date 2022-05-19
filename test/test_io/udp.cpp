#include "async_runtime/io/udp.h"
#include <iostream>
extern "C"
{
#include <uv.h>
}

void task();
int main()
{
    EventLoop::run(task);
    return 0;
}

static struct sockaddr_in s0;
static struct sockaddr_in s1;
void task()
{
    assert(uv_ip4_addr("127.0.0.1", 10001, &s0) == 0);
    assert(uv_ip4_addr("127.0.0.1", 10002, &s1) == 0);
    const auto server0 = Udp::from(reinterpret_cast<sockaddr *>(&s0), 0);
    const auto server1 = Udp::from(reinterpret_cast<sockaddr *>(&s1), 0);

    server0->startRecv()->listen([server0](const ref<RecvMessage> &message) { //
        server0->close();
        std::cout << "Message recv by server0 - " << message->content << std::endl;
    });

    server1->startRecv()->listen([server1](const ref<RecvMessage> &message) { //
        std::cout << "Message recv by server1 - " << message->content << std::endl;
        server1->stopRecv();
        server1->send("Message send from server1", reinterpret_cast<sockaddr *>(&s0))
            ->then<int>([server1](const int &status) { //
                std::cout << "Send status from server1 - " << status << std::endl;
                server1->close();
                return 0;
            });
    });

    server0->send("Message send from server0", reinterpret_cast<const struct sockaddr *>(&s1))
        ->then<int>([](const int &status) { //
            std::cout << "Send status from server0 - " << status << std::endl;
            return 0;
        });
}
