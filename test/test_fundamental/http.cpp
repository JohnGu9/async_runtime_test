#include <iostream>
#include "async_runtime.h"
#include "async_runtime/fundamental/http.h"

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
        _server = Object::create<Http::Server>(this);
        _server->onGet(pattern, [](const Http::Request &request, Http::Response &response) {
                   debug_print("HttpServer Running on thread: " << ThreadPool::thisThreadName << std::endl // Http:Server callback will work on state's thread, don't worry about thread problem
                                                                << "Body: Hello World!" << std::endl
                                                                << "Content-Type: text/plain" << std::endl);
                   response.set_content("Hello World!", "text/plain");
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

    ref<Widget> build(ref<BuildContext>) override;
};

inline ref<State<>> HttpServerTest::createState()
{
    return Object::create<_HttpTestState>();
}

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

    void onReturn(const ref<Http::Client::Result> &res)
    {
        assert(res->error == httplib::Success);
        debug_print("Http Client: " << std::endl
                                    << "Error: " << res->errorString()
                                    << std::endl);
        if (res->response != nullptr)
        {
            debug_print("HttpResponse" << std::endl
                                       << "Status: " << res->response->status << std::endl
                                       << "Version: " << res->response->version << std::endl
                                       << "Location: " << res->response->location << std::endl
                                       << "Body: " << res->response->body << std::endl
                                       << "Content-Type: " << res->response->get_header_value("Content-Type") << std::endl);
        }
        else
        {
            debug_print("No HttpResponse" << std::endl);
        }
        Process::of(context)->exit();
    }

    void initState() override
    {
        super::initState();
        auto widget = this->getWidget();
        _client = Object::create<Http::Client>(this, widget->url, widget->port);
        _client->get(widget->pattern)
            // async api Future::than
            ->than<void>([this](const ref<Http::Client::Result> &res) { onReturn(res); });
    }

    void didWidgetUpdated(ref<StatefulWidget> oldWidget) override
    {
        super::didWidgetUpdated(oldWidget);
        auto widget = this->getWidget();
        auto old = oldWidget->covariant<HttpClientTest>();
        if (widget->url != old->url || widget->port != old->port)
        {
            _client->dispose();
            _client->get(widget->pattern)
                // async api Future::than
                ->than<void>([this](const ref<Http::Client::Result> &res) { onReturn(res); });
        }
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

inline ref<Widget> _HttpTestState::build(ref<BuildContext>)
{
    return Object::create<HttpClientTest>(localhost, port, pattern);
}

int main()
{
    return runApp(Object::create<HttpServerTest>());
}