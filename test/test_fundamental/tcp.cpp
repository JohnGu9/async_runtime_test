#include "async_runtime/fundamental/tcp.h"
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

    auto server0 = Tcp::from(reinterpret_cast<const struct sockaddr *>(&s0), 0);
    auto server1 = Tcp::from(reinterpret_cast<const struct sockaddr *>(&s1), 0);

    auto stream = server0->listen();
    stream->listen([](ref<Tcp::Connection> connection) //
                   {                                   //
                       connection->startRead()
                           ->listen([](ref<String> message) //
                                    {                       //
                                        std::cout << "server0 recv message - " << message << std::endl;
                                    });
                       connection->write("Message send from server0")
                           ->then<int>([](const int &status) //
                                       {                     //
                                           std::cout << "server0 send message status - " << status << std::endl;
                                           return 0;
                                       });
                   });

    server1->connect(reinterpret_cast<const struct sockaddr *>(&s0))
        ->then<int>([server0](ref<Tcp::Connection> connection) //
                    {                                          //
                        connection->startRead()
                            ->listen([server0, connection](ref<String> message) //
                                     {                                          //
                                         server0->close();
                                         connection->shutdown();
                                         std::cout << "server0 recv message - " << message << std::endl;
                                     });
                        connection->write("Message send from server1")
                            ->then<int>([](const int &status) //
                                        {                     //
                                            std::cout << "server1 send message status - " << status << std::endl;
                                            return 0;
                                        });

                        return 0;
                    });
}