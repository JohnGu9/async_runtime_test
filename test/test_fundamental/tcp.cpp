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
    auto server0RecvComfirm = Object::create<Completer<int>>();
    auto server1RecvComfirm = Object::create<Completer<int>>();
    auto allRecComfirm = server0RecvComfirm
                             ->then<int>([server0RecvComfirm, server1RecvComfirm](const int &) //
                                         {                                                     //
                                             ref<Future<int>> future = server1RecvComfirm;
                                             return future;
                                         }); // @TODO: add [Future::wait] and [Future::race] api

    allRecComfirm->then<int>([server0, server1](const int &) //
                             {                               //
                                 server0->close();
                                 server1->close();
                                 return 0;
                             });

    auto stream = server0->listen();
    stream->listen([server0RecvComfirm, allRecComfirm](ref<Tcp::Connection> connection) //
                   {                                                                    //
                       connection->startRead()
                           ->listen([server0RecvComfirm](ref<String> message) //
                                    {                                         //
                                        server0RecvComfirm->complete(0);
                                        std::cout << "server0 recv message - " << message << std::endl;
                                    });

                       allRecComfirm->then<int>([connection](const int &) //
                                                {                         //
                                                    return connection->shutdown();
                                                });
                       connection->write("Message send from server0")
                           ->then<int>([](const int &status) //
                                       {                     //
                                           std::cout << "server0 send message status - " << status << std::endl;
                                           return 0;
                                       });
                   });

    server1->connect(reinterpret_cast<const struct sockaddr *>(&s0))
        ->then<int>([allRecComfirm, server1RecvComfirm](ref<Tcp::Connection> connection) //
                    {                                                                    //
                        connection->startRead()
                            ->listen([connection, server1RecvComfirm](ref<String> message) //
                                     {                                                     //
                                         server1RecvComfirm->complete(0);
                                         std::cout << "server0 recv message - " << message << std::endl;
                                     });

                        allRecComfirm->then<int>([connection](const int &) //
                                                 {                         //
                                                     return connection->shutdown();
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