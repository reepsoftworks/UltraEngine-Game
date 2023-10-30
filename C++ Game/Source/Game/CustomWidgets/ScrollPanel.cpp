#include "UltraEngine.h"
#include "ScrollPanel.h"

namespace UltraEngine::Game
{
	shared_ptr<ScrollPanel> CreateScrollPanel(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		const int sliderwidth = 16;
		auto widget = std::make_shared<ScrollPanel>();
		widget->Initialize("", x, y, width, height, parent, 0);
		widget->subpanel = CreatePanel(0, 0, widget->size.x, widget->size.y, widget, PANEL_BORDER);
		widget->subpanel->SetLayout(1, 1, 1, 1);
		widget->subpanel->SetColor(0, 0, 0, 0);
		widget->UpdateLayout();
		widget->slider[1] = CreateSlider(widget->size.x - sliderwidth, 0, sliderwidth, widget->size.y, widget, SLIDER_SCROLLBAR | SLIDER_VERTICAL);
		widget->slider[1]->SetHidden(true);
		widget->slider[1]->SetLayout(0,1,1,1);
		ListenEvent(EVENT_NONE, widget->slider[1], ScrollPanel::EventCallback, widget);
		return widget;
	}

	void ScrollPanel::SetArea(const int width, const int height)
	{
		if (subpanel->size.x == width and subpanel->size.y == height) return;
		if (height > size.y)
		{
			slider[1]->SetHidden(false);
			slider[1]->SetRange(size.y,height);
			subpanel->SetShape(0,int(-slider[1]->GetValue()), size.x - slider[1]->size.x, height);
		}
		else
		{
			slider[1]->SetHidden(true);
			slider[1]->SetValue(0);
			subpanel->SetShape(0,0,size.x,size.y);
		}
	}

	void ScrollPanel::UpdateLayout() 
	{
		Widget::UpdateLayout();

	}

	bool ScrollPanel::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<ScrollPanel>();
		return elem->ProcessEvent(e);
	}

	bool ScrollPanel::ProcessEvent(const Event& e)
	{
		switch (e.id)
		{
		case EVENT_WIDGETACTION:
			if (e.source == slider[1])
			{
				subpanel->SetShape(0, -e.data, subpanel->size.x, subpanel->size.y);
				EmitEvent(EVENT_WIDGETACTION, Self(), 0, subpanel->position.x, subpanel->position.y, subpanel->size.x, subpanel->size.y);
			}
			break;
		}
		return true;
	}
}