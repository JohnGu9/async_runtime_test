#pragma once

#include "async_runtime.h"

class CommandEcho : public StatefulWidget
{
    ref<State<>> createState() override;
};

class _CommandEchoState : public State<CommandEcho>
{
    using super = State<CommandEcho>;
    Function<void(ref<List<ref<String>>>)> onCommand;

    void initState() override
    {
        super::initState();
        onCommand = [this](ref<List<ref<String>>> arguments) {
            if (arguments[0] == "echo")
            {
                std::stringstream ss;
                for (size_t i = 1; i < arguments->size(); i++)
                    ss << arguments[i] << " ";
                StdoutLogger::of(context)->writeLine(ss.str());
            }
        };
    }

    void didDependenceChanged() override
    {
        super::didDependenceChanged();
        Process::of(context)->command->addListener(onCommand);
    }

    void dispose() override
    {
        Process::of(context)->command->removeListener(onCommand);
        onCommand = nullptr;
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
