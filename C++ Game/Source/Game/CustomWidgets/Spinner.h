#pragma once
#include "UltraEngine.h"
#include <limits>

namespace UltraEngine::Game
{
    enum SpinnerStyle
    {
        SPINNER_DEFAULT = 0,
        SPINNER_INTEGER = 0,
        SPINNER_FLOAT = 1,
        SPINNER_NOSTEPPER = 2
    };
    inline SpinnerStyle operator|(SpinnerStyle a, SpinnerStyle b)
    {
        return static_cast<SpinnerStyle>(static_cast<int>(a) | static_cast<int>(b));
    };

    class Spinner : public Widget
    {
        int decimalplaces;
        shared_ptr<Widget> textfield;
        shared_ptr<Widget> slider;
        bool usefrange;
        static bool EventCallback(const Event& e, shared_ptr<Object> extra);
        static bool Filter(const WString& text, shared_ptr<Object> extra);
    public:
        virtual void SetColor(const float r, const float g, const float b, const float a = 1.0f, const WidgetColor index = WIDGETCOLOR_BACKGROUND, const int deltatime = 0);
        virtual void SetFontScale(const float scale);
        virtual WString GetText();
        virtual void SetRange(const double min, const double max);
        virtual void SetValue(const double value);
        virtual void SetColor(const float r, const float g, const float b, const float a, const WidgetColor index = WIDGETCOLOR_BACKGROUND);
        friend shared_ptr<Widget> CreateSpinner(const int, const int, const int, const int, shared_ptr<Widget>, const SpinnerStyle);
    };

    extern shared_ptr<Widget> CreateSpinner(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const SpinnerStyle style = SPINNER_DEFAULT);
}