#pragma once
#include "UltraEngine.h"

namespace UltraEngine::Game
{
	class FloatSlider : public Widget
	{
		shared_ptr<Widget> panel;
		shared_ptr<Widget> slider;
		shared_ptr<Widget> textbox;
		float m_min, m_max;
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		virtual bool ProcessEvent(const Event& e);

	public:
		virtual void SetLayout(const int left, const int right, const int top, const int bottom);
		virtual void SetValue(const double value);
		virtual double GetValue();
		friend shared_ptr<FloatSlider> CreateFloatSlider(const float min, const float max, const float defaultvalue, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	};

	extern shared_ptr<FloatSlider> CreateFloatSlider(const float min, const float max, const float defaultvalue, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
}