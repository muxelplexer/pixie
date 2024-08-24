#pragma once

namespace ui
{
    struct widget
    {
        virtual ~widget() = default;
        virtual void render() const = 0;
        virtual void update() = 0;
    };
}
