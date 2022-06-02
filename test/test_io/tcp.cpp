#include "async_runtime/io/tcp.h"
#include <assert.h>
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
#define SERVER0_MESSAGE "Message send from server0"
#define SERVER1_MESSAGE "Message send from server1"
void task()
{
    assert(uv_ip4_addr("127.0.0.1", 10001, &s0) == 0);
    assert(uv_ip4_addr("127.0.0.1", 10002, &s1) == 0);

    auto server0 = Tcp::from(reinterpret_cast<sockaddr *>(&s0), 0);
    auto server1 = Tcp::from(reinterpret_cast<sockaddr *>(&s1), 0);
    auto server0RecvConfirm = Object::create<Completer<int>>();
    auto server1RecvConfirm = Object::create<Completer<int>>();
    auto allRecConfirm = Future<>::wait<int, int>(server0RecvConfirm, server1RecvConfirm);
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
                assert(message == SERVER1_MESSAGE);
                std::cout << "server0 recv message - " << message << std::endl;
            });
        connection->write(SERVER0_MESSAGE)
            ->then<int>([](const int &status) { //
                assert(status == 0);
                std::cout << "server0 send message status - " << status << std::endl;
                return 0;
            });
        allRecConfirm->then<int>([connection] { //
            return connection->shutdown();
        });
    });

    server1->connect(reinterpret_cast<sockaddr *>(&s0))
        ->then<int>([allRecConfirm, server1RecvConfirm](ref<Tcp::Connection> connection) { //
            connection->startRead()
                ->listen([connection, server1RecvConfirm](ref<String> message) { //
                    server1RecvConfirm->complete(0);
                    assert(message == SERVER0_MESSAGE);
                    std::cout << "server0 recv message - " << message << std::endl;
                });
            connection->write(SERVER1_MESSAGE)->then<int>([](const int &status) { //
                assert(status == 0);
                std::cout << "server1 send message status - " << status << std::endl;
                return 0;
            });
            allRecConfirm->then<int>([connection] { //
                return connection->shutdown();
            });

            return 0;
        });
}
