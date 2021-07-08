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
                if (begin != String::npos)
                {
                    auto cmd = command->substr(0, begin);
                    if (cmd == "echo")
                    {
                        begin = command->find_first_not_of(" ", begin);
                        if (begin != String::npos)
                        {
                            auto argument = command->substr(begin);
                            StdoutLogger::of(context)->writeLine(argument);
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
