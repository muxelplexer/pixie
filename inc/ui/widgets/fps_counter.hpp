#pragma once
#include "ui/widget.hpp"

namespace ui::widgets
{
    class fps_counter : public widget
    {
    public:
        virtual ~fps_counter() = default;

        void update() override;
        void render() const override;
    private:
        float delta{0};
        float time{0};
    };
}
