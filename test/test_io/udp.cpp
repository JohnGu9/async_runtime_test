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
    const auto server0 = Udp::from(reinterpret_cast<const struct sockaddr *>(&s0), 0);
    const auto server1 = Udp::from(reinterpret_cast<const struct sockaddr *>(&s1), 0);

    server0->startRecv()->listen([server0](const ref<RecvMessage> &message) //
                                 {                                          //
                                     const auto server = server0;           // copy ref from heap to stack
                                                                            // in Windows, it does not automatically lock the resource inside lambda while lambda function is execing
                                                                            // if the function make its own ref count to 0 while function is execing, you will lose all resource inside lambda

                                     server->close();
                                     std::cout << "Message recv by server0 - " << message->content << std::endl;
                                 });

    server1->startRecv()->listen([server1](const ref<RecvMessage> &message) //
                                 {                                          //
                                     const auto server = server1;           // copy ref from heap to stack
                                     std::cout << "Message recv by server1 - " << message->content << std::endl;
                                     server->stopRecv();
                                     server->send("Message send from server1", reinterpret_cast<const struct sockaddr *>(&s0))
                                         ->then<int>([server](const int &status) //
                                                     {                           //
                                                         std::cout << "Send status from server1 - " << status << std::endl;
                                                         server->close();
                                                         return 0;
                                                     });
                                 });

    server0->send("Message send from server0", reinterpret_cast<const struct sockaddr *>(&s1))
        ->then<int>([](const int &status) //
                    {                     //
                        std::cout << "Send status from server0 - " << status << std::endl;
                        return 0;
                    });
}
