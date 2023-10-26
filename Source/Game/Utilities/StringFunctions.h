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
	extern String QuoteString(const String& s);
	extern String UnQuoteString(const String& s);
	extern String EncyptString(const String& key, const String& token);

	extern WString QuoteWString(const WString& s);
	extern WString UnQuoteWString(const WString& s);
	extern WString EncyptWString(const WString& key, const WString& token);
}