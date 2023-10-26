#include "UltraEngine.h"
#include "NumberSlider.h"
#include "Spinner.h"

namespace UltraEngine::Game
{
	shared_ptr<NumberSlider> CreateNumberSlider(const int min, const int max, const int defaultvalue, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto widget = std::make_shared<NumberSlider>();
		widget->panel = CreatePanel(x, y, width, height, parent);
		widget->slider = CreateSlider(0, 0, width - 50, height, widget->panel, SLIDER_TRACKBAR);
		widget->slider->SetRange(min, max);
		ListenEvent(EVENT_WIDGETACTION, widget->slider, NumberSlider::EventCallback, widget);
		widget->textbox = CreateSpinner(widget->slider->GetPosition().x + widget->slider->GetSize().x + 16, widget->slider->GetPosition().y, 34, height, widget->panel, SPINNER_INTEGER | SPINNER_NOSTEPPER);

		// Apply default values.
		auto spinner = widget->textbox->As<Spinner>();
		if (defaultvalue >= min)
		{
			spinner->SetValue(defaultvalue);
			widget->slider->SetValue(defaultvalue);
		}
		else
		{
			spinner->SetValue(min);
			widget->slider->SetValue(min);
		}
			
		spinner->SetRange(min, max);
		widget->m_min = min;
		widget->m_max = max;
		ListenEvent(EVENT_WIDGETACTION, spinner, NumberSlider::EventCallback, widget);
		return widget;
	}

	void NumberSlider::SetLayout(const int left, const int right, const int top, const int bottom)
	{
		panel->SetLayout(left, right, top, bottom);
	}

	void NumberSlider::SetValue(const double value)
	{
		auto spinner = textbox->As<Spinner>();
		if (value >= m_min)
		{
			spinner->SetValue(value);
			slider->SetValue(value);
		}
		else
		{
			spinner->SetValue(m_min);
			slider->SetValue(m_min);
		}
	}

	double NumberSlider::GetValue()
	{
		auto spinner = textbox->As<Spinner>();
		float ret = 0.0f;
		if (spinner != NULL)
		{
			ret = (float)spinner->GetValue();
		}
		return ret;
	}

	bool NumberSlider::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<NumberSlider>();
		return elem->ProcessEvent(e);
	}

	bool NumberSlider::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_WIDGETACTION)
		{
			auto spinner = textbox->As<Spinner>();
			if (e.source == slider)
			{
				spinner->SetValue(slider->GetValue());
			}
			else if (e.source == spinner)
			{
				slider->SetValue(spinner->GetValue());
			}
		}

		return true;
	}
}