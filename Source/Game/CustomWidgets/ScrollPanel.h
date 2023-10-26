#pragma once
#include "UltraEngine.h"

namespace UltraEngine::Game
{
	class ScrollPanel : public Widget
	{
		virtual void UpdateLayout();
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		virtual bool ProcessEvent(const Event& e);
	public:
		std::array<shared_ptr<Widget>, 2> slider;
		shared_ptr<Widget> subpanel;
		
		virtual void SetArea(const int x, const int y);
		
		friend shared_ptr<ScrollPanel> CreateScrollPanel(const int, const int, const int, const int, shared_ptr<Widget>);
	};

	extern shared_ptr<ScrollPanel> CreateScrollPanel(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
}