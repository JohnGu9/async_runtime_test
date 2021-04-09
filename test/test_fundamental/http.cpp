#include <iostream>
#include "async_runtime.h"
#include "async_runtime/fundamental/http.h"

class HttpTest : public StatefulWidget
{
    ref<State<>> createState() override;
};

static const int port = 12345;
static finalref<String> pattern = "/test";
static finalref<String> localhost = "localhost";

class _HttpTestState : public State<HttpTest>
{
    using super = State<HttpTest>;

    lateref<Http::Server> _server;

    void initState() override
    {
        super::initState();
        _server = Object::create<Http::Server>(this);
        _server->onGet(pattern, [](const Http::Request &request, Http::Response &response) {
            response.set_content("Hello World!", "text/plain");
        });
        _server->listen(localhost, port);
    }

    void dispose() override
    {
        _server->dispose();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override;
};

class HttpClientTest : public StatefulWidget
{
public:
    HttpClientTest(ref<String> url, int port, ref<String> pattern)
        : url(url), port(port), pattern(pattern){};

    ref<String> url;
    int port;
    ref<String> pattern;
    ref<State<>> createState() override;
};

class _HttpClientTestState : public State<HttpClientTest>
{
    using super = State<HttpClientTest>;
    lateref<Http::Client> _client;

    void initState() override
    {
        super::initState();
        auto widget = this->getWidget();
        _client = Object::create<Http::Client>(this, widget->url, widget->port);
        _client->get(widget->pattern)
            // async api Future::than
            ->than<void>([this](const ref<Http::Client::Result> &res) {
                debug_print("Get result: " << std::endl
                                           << "Error: " << res->errorString()
                                           << std::endl);
                if (res->response != nullptr)
                {
                    debug_print("HttpResponse" << std::endl
                                               << "HttpStatus: " << res->response->status << std::endl
                                               << "HttpVersion: " << res->response->version << std::endl
                                               << "HttpLocation: " << res->response->location << std::endl
                                               << "HttpBody: " << res->response->body << std::endl);
                }
                else
                {
                    debug_print("No HttpResponse" << std::endl);
                }
                RootInheritedWidget::of(this->context)->exit();
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

inline ref<State<>> HttpTest::createState()
{
    return Object::create<_HttpTestState>();
}

inline ref<Widget> _HttpTestState::build(ref<BuildContext>)
{
    return Object::create<HttpClientTest>(localhost, port, pattern);
}

int main()
{
    runApp(Object::create<HttpTest>());
    return EXIT_SUCCESS;
}