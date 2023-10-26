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
	enum MessageID
	{
		MESSAGE_ID_NULL = 0,
		MESSAGE_ID_OK,
		MESSAGE_ID_CANCEL,
		MESSAGE_ID_ABORT,
		MESSAGE_ID_RETRY,
		MESSAGE_ID_IGNORE,
		MESSAGE_ID_YES,
		MESSAGE_ID_NO,
	};

	extern const MessageID Message(const WString& title, const WString& message);
	extern const MessageID MessageAlert(const WString& title, const WString& message);
	extern const MessageID MessageError(const WString& title, const WString& message);
	extern const MessageID MessageContinue(const WString& title, const WString& message);
}