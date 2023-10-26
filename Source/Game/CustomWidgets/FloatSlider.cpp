#include "UltraEngine.h"
#include "FloatSlider.h"
#include "Spinner.h"

namespace UltraEngine::Game
{
	shared_ptr<FloatSlider> CreateFloatSlider(const float min, const float max, const float defaultvalue, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto widget = std::make_shared<FloatSlider>();
		widget->panel = CreatePanel(x, y, width, height, parent);
		widget->slider = CreateSlider(0, 0, width - 50, height, widget->panel, SLIDER_TRACKBAR);
		widget->slider->SetRange(min, max);
		ListenEvent(EVENT_WIDGETACTION, widget->slider, FloatSlider::EventCallback, widget);
		widget->textbox = CreateSpinner(widget->slider->GetPosition().x + widget->slider->GetSize().x + 16, widget->slider->GetPosition().y, 60, height, widget->panel, SPINNER_FLOAT| SPINNER_NOSTEPPER);

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
		ListenEvent(EVENT_WIDGETACTION, spinner, FloatSlider::EventCallback, widget);
		return widget;
	}

	void FloatSlider::SetLayout(const int left, const int right, const int top, const int bottom)
	{
		panel->SetLayout(left, right, top, bottom);
	}

	void FloatSlider::SetValue(const double value)
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

	double FloatSlider::GetValue()
	{
		auto spinner = textbox->As<Spinner>();
		float ret = 0.0f;
		if (spinner != NULL)
		{
			ret = (float)spinner->GetValue();
		}
		return ret;
	}

	bool FloatSlider::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<FloatSlider>();
		return elem->ProcessEvent(e);
	}

	bool FloatSlider::ProcessEvent(const Event& e)
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