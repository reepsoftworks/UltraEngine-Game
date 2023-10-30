//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#pragma once
#include "UltraEngine.h"

namespace UltraEngine::Game
{
	class ResoultionComboBox : public Widget
	{
		std::vector<std::shared_ptr<UltraEngine::Widget>> combobox;
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		virtual bool ProcessEvent(const Event& e);

	public:
		virtual void Enable();
		virtual void Disable();
		virtual bool GetDisabled();

		virtual void SetLayout(const int left, const int right, const int top, const int bottom);
		void ShowDisplayBox(const int displayindex);
		iVec2 GetResoultion(const int displayindex);
		const bool ForceSelection(const int displayindex, const UltraEngine::WString& item);
		friend shared_ptr<ResoultionComboBox> CreateResoultionComboBox(const int, const int, const int, const int, shared_ptr<Widget>);
	};

	extern shared_ptr<ResoultionComboBox> CreateResoultionComboBox(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
}