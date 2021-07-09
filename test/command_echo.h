#pragma once

#include "async_runtime.h"

class CommandEcho : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _CommandEchoState : public State<CommandEcho>
{
    using super = State<CommandEcho>;
    lateref<StreamSubscription<ref<String>>> _subscription;

    void didDependenceChanged() override
    {
        super::didDependenceChanged();
        _subscription = Process::of(context)->command->listen(
            [this](ref<String> command) {
                auto begin = command->find(" ");
                if (begin != std::string::npos)
                {
                    auto cmd = command->substr(0, begin);
                    if (cmd == "echo")
                    {
                        begin = command->find_first_not_of(" ", begin);
                        if (begin != std::string::npos)
                        {
                            auto argu = command->substr(begin);
                            StdoutLogger::of(context)->writeLine(argu);
                        }
                        else
                        {
                            LogError("echo with no argument");
                        }
                    }
                }
            });
    }

    void dispose() override
    {
        _subscription->cancel();
        super::dispose();
    }

    ref<Widget> build(ref<BuildContext>) override
    {
        return LeafWidget::factory();
    }
};

inline ref<State<>> CommandEcho::createState()
{
    return Object::create<_CommandEchoState>();
}
