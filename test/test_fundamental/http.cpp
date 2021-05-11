#define ASYNC_RUNTIME_DISABLE_CONSOLE
#include "async_runtime.h"

class HttpClientTest : public StatefulWidget
{
    ref<State<>> createState() override;

public:
    HttpClientTest(ref<String> address, ref<String> pattern, int port)
        : address(address), pattern(pattern), port(port) {}

    ref<String> address;
    ref<String> pattern;
    int port;
};

class _HttpClientTestState : public State<HttpClientTest>
{
    using super = State<HttpClientTest>;
    lateref<Http::Client> _client;

    void initState() override
    {
        super::initState();
        _client = Object::create<Http::Client>(self(), widget->address, widget->port);
        _client->get(widget->pattern)->than<void>([this](const ref<Http::Client::Result> &result) {
            LogInfo("Http Client get result with error [{}]", result->errorString());
            if (result->response != nullptr)
            {
                LogInfo(std::endl
                        << "Http Version: " << result->response->version << std::endl
                        << "Http Status: " << result->response->status << std::endl
                        << "Http Body: " << result->response->body << std::endl);
            }
            _client->get("/exit");
        });
    }

    void dispose() override
    {
        _client->dispose();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<>> HttpClientTest::createState()
{
    return Object::create<_HttpClientTestState>();
}

class HttpServerTest : public StatefulWidget
{
    ref<State<>> createState() override;
};

static const int port = 12345;
static finalref<String> pattern = "/test";
static finalref<String> localhost = "localhost";

class _HttpTestState : public State<HttpServerTest>
{
    using super = State<HttpServerTest>;
    lateref<Http::Server> _server;

    void initState() override
    {
        super::initState();
        _server = Object::create<Http::Server>(self());
        _server->onGet(pattern, [this](const Http::Request &request, Http::Response &response) {
                   LogInfo("HttpServer Running on thread: " << ThreadPool::thisThreadName << std::endl // Http:Server callback will work on state's thread, don't worry about thread problem
                                                            << "Body: Hello World!" << std::endl
                                                            << "Content-Type: text/plain" << std::endl);
                   response.set_content("Hello World!", "text/plain");
               })
            ->onGet("/exit", [this](const Http::Request &request, Http::Response &response) {
                LogInfo("System exit");
                response.set_content("System exit", "text/plain");
                Process::of(context)->exit(0);
            })
            ->onPost("/post", [](const Http::Request &request, Http::Response &response) { response.set_content("onPost", "text/plain"); })
            ->onDelete("/delete", [](const Http::Request &request, Http::Response &response) { response.set_content("onDelete", "text/plain"); })
            ->listen(localhost, port);
    }

    void dispose() override
    {
        _server->dispose();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return Object::create<HttpClientTest>(
            localhost, /* address */
            pattern,   /* pattern */
            port       /* port */
        );
    }
};

inline ref<State<>> HttpServerTest::createState()
{
    return Object::create<_HttpTestState>();
}

int main()
{
    return runApp(Object::create<HttpServerTest>());
}