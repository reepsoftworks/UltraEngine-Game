#include "UltraEngine.h"
#include "Spinner.h"

namespace UltraEngine::Game
{
	void Spinner::SetRange(const double min, const double max)
	{
		Widget::SetRange(min, max);
		if (slider)
		{
			if ((style & SPINNER_FLOAT) == 0) slider->SetRange(min, max);
		}
		usefrange = true;
	}

	WString Spinner::GetText()
	{
		return textfield->text;
	}

	void Spinner::SetFontScale(const float scale)
	{
		textfield->SetFontScale(scale);
		if (slider) slider->SetFontScale(scale);
	}

	void Spinner::SetColor(const float r, const float g, const float b, const float a, const WidgetColor index, const int deltatime)
	{
		textfield->SetColor(r, g, b, a, index, deltatime);
		if (slider) slider->SetColor(r, g, b, a, index, deltatime);
	}

	void Spinner::SetValue(const double value)
	{
		Widget::SetValue(value);
		WString s;
		if ((style & SPINNER_FLOAT) != 0)
		{
			s = WString(GetValue());
			if (s.Find("e-") != -1) s = "0.0";
			if (s.Find(".") == -1) s += L".0";
		}
		else
		{
			s = WString(Round(GetValue()));
		}
		textfield->SetText(s);
	}

	void Spinner::SetColor(const float r, const float g, const float b, const float a, const WidgetColor index)
	{
		textfield->SetColor(r, g, b, a, index);
		if (slider) slider->SetColor(r, g, b, a, index);
	}

	bool Spinner::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto spinner = extra->As<Spinner>();
		if (e.id == EVENT_WIDGETACTION)
		{
			if (e.source == spinner->slider and spinner->slider != NULL)
			{
				if ((spinner->style & SPINNER_FLOAT) == 0)
				{
					spinner->slidervalue = e.data;
					spinner->textfield->SetText(e.data);
					EmitEvent(EVENT_WIDGETACTION, spinner, e.data);
				}
				else
				{
					auto v = spinner->GetValue();
					if (double(Round(v)) == v)
					{
						v += double(e.data);
						spinner->SetValue(v);
					}
					else
					{
						if (e.data == 1) spinner->SetValue(Ceil(v));
						if (e.data == -1) spinner->SetValue(Floor(v));
					}
					if (v != spinner->GetValue()) EmitEvent(EVENT_WIDGETACTION, spinner);
					spinner->slider->SetValue(0);
				}
			}
			if (e.source == spinner->textfield)
			{
				auto s = spinner->textfield->text;
				if ((spinner->style & SPINNER_FLOAT) != 0)
				{
					double f = s.ToFloat();
					f = Clamp(f, spinner->range[0], spinner->range[1]);
					spinner->slidervalue = f;
					auto s = e.text;
					s = WString(f);
					if (s.Find("e-") != -1) s = "0.0";
					if (s.Find(L".") == -1) s += L".0";
					if (s != e.text) spinner->textfield->SetText(s);
					EmitEvent(EVENT_WIDGETACTION, spinner);
				}
				else
				{
					const auto i = s.ToInt();
					if (i != spinner->GetValue())
					{
						spinner->SetValue((double)i);
						EmitEvent(EVENT_WIDGETACTION, spinner, e.data);
					}
				}
			}
		}
		else if (e.id == EVENT_WIDGETGAINFOCUS or e.id == EVENT_WIDGETLOSEFOCUS)
		{
			if (e.source == spinner->textfield or (spinner->slider != NULL and e.source == spinner->slider))
			{
				EmitEvent(e.id, spinner);
				return false;
			}
		}
		return true;
	}

	bool Spinner::Filter(const WString& text, shared_ptr<Object> extra)
	{
		auto spinner = extra->As<Spinner>();
		if (text == L"0" or text == L"1" or text == L"2" or text == L"3" or text == L"4" or text == L"5"
			or text == L"6" or text == L"7" or text == L"8" or text == L"9") return true;
		if (spinner->range.x < 0.0 and text == L"-") return true;
		if ((spinner->style & SPINNER_FLOAT) != 0 and text == L".") return true;
		return false;
	}

	shared_ptr<Widget> CreateSpinner(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const SpinnerStyle style)
	{
		int sw = 1;
		auto spinner = std::make_shared<Spinner>();
		spinner->Initialize("", x, y, width, height, parent, style);
		if ((style & SPINNER_NOSTEPPER) == 0)
		{
			sw = 20;
			spinner->slider = CreateSlider(width - sw, 0, sw, height, spinner, SLIDER_STEPPER | SLIDER_VERTICAL);
			spinner->slider->SetLayout(0, 1, 1, 1);
			spinner->slider->SetRange(-1, 1);
		}
		spinner->textfield = CreateTextField(0, 0, width - sw + 1, height, spinner);
		spinner->textfield->SetLayout(1, 1, 1, 1);
		spinner->textfield->As<TextField>()->SetFilter(Spinner::Filter, spinner);
		spinner->usefrange = false;
		spinner->SetRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
		if ((style & SPINNER_FLOAT) == 0)
		{
			spinner->textfield->SetText("0");
		}
		else
		{
			spinner->textfield->SetText("0.0");
		}
		ListenEvent(EVENT_NONE, spinner->textfield, Spinner::EventCallback, spinner);
		ListenEvent(EVENT_NONE, spinner->slider, Spinner::EventCallback, spinner);
		return spinner;
	}
}