//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "MessageBox.h"

namespace UltraEngine::Game
{
	const MessageID Message(const WString& title, const WString& message)
	{
#if defined (_WIN32)
		HWND hwnd = NULL;
		auto window = ActiveWindow();
		if (window) hwnd = window->GetHandle();

		return (MessageID)::MessageBoxW(hwnd, message.c_str(), title.c_str(), MB_OK);
#else
		Notify(message, title);
		return MESSAGE_ID_NULL;
#endif
		
	}

	const MessageID MessageAlert(const WString& title, const WString& message)
	{
#if defined (_WIN32)
		HWND hwnd = NULL;
		auto window = ActiveWindow();
		if (window) hwnd = window->GetHandle();

		return (MessageID)::MessageBoxW(hwnd, message.c_str(), title.c_str(), MB_ICONWARNING | MB_OK);
#else
		Notify(message, title, true);
		return MESSAGE_ID_NULL;
#endif
	}

	const MessageID MessageError(const WString& title, const WString& message)
	{
#if defined (_WIN32)
		HWND hwnd = NULL;
		auto window = ActiveWindow();
		if (window) hwnd = window->GetHandle();

		return (MessageID)::MessageBoxW(hwnd, message.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
#else
		Notify(message, title, true);
		return MESSAGE_ID_NULL;
#endif
	}

	const MessageID MessageContinue(const WString& title, const WString& message)
	{
#if defined (_WIN32)
		HWND hwnd = NULL;
		auto window = ActiveWindow();
		if (window) hwnd = window->GetHandle();

		MessageID msgboxID = (MessageID)MessageBoxW(
			hwnd,
			message.c_str(), title.c_str(),
			MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
		);
		return msgboxID;
#else
		int i = Proceed(message, title);
		MessageID id = MESSAGE_ID_NULL;
		if (i == 1)
			id = MESSAGE_ID_YES;
		else if (i == -1)
			id = MESSAGE_ID_NO;
		else
			id = MESSAGE_ID_CANCEL;

		return id;
#endif		
	}
}