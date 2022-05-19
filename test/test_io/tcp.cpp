#include "async_runtime/io/tcp.h"
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

    auto server0 = Tcp::from(reinterpret_cast<sockaddr *>(&s0), 0);
    auto server1 = Tcp::from(reinterpret_cast<sockaddr *>(&s1), 0);
    auto server0RecvConfirm = Object::create<Completer<int>>();
    auto server1RecvConfirm = Object::create<Completer<int>>();
    auto allRecConfirm = server0RecvConfirm->then<int>([server0RecvConfirm, server1RecvConfirm] { //
        ref<Future<int>> future = server1RecvConfirm;
        return future;
    }); // @TODO: add [Future::wait] and [Future::race] api

    allRecConfirm->then<int>([server0, server1] { //
        server0->close();
        server1->close();
        return 0;
    });

    auto stream = server0->listen();
    stream->listen([server0RecvConfirm, allRecConfirm](ref<Tcp::Connection> connection) { //
        connection->startRead()
            ->listen([server0RecvConfirm](ref<String> message) { //
                server0RecvConfirm->complete(0);
                std::cout << "server0 recv message - " << message << std::endl;
            });
        allRecConfirm->then<int>([connection] { //
            return connection->shutdown();
        });
        connection->write("Message send from server0")
            ->then<int>([](const int &status) { //
                std::cout << "server0 send message status - " << status << std::endl;
                return 0;
            });
    });

    server1->connect(reinterpret_cast<sockaddr *>(&s0))
        ->then<int>([allRecConfirm, server1RecvConfirm](ref<Tcp::Connection> connection) { //
            connection->startRead()
                ->listen([connection, server1RecvConfirm](ref<String> message) { //
                    server1RecvConfirm->complete(0);
                    std::cout << "server0 recv message - " << message << std::endl;
                });
            allRecConfirm->then<int>([connection] { //
                return connection->shutdown();
            });
            connection->write("Message send from server1")->then<int>([](const int &status) { //
                std::cout << "server1 send message status - " << status << std::endl;
                return 0;
            });

            return 0;
        });
}
