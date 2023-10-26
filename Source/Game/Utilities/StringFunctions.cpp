//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "StringFunctions.h"

namespace UltraEngine::Game
{
	String QuoteString(const String& s)
	{
		return "\"" + s + "\"";
	}

	String UnQuoteString(const String& s)
	{
		auto ret = s;
		auto len = s.length();
		if (ret[0] == '"' && ret[len - 1] == '"')
		{
			ret = ret.Left((int)ret.length() - 1);
			ret = ret.Right((int)ret.length() - 1);
		}
		return ret;
	}

	String EncyptString(const String& key, const String& token)
	{
		String filepart = key.Lower();
		while (filepart.length() < 32)
		{
			filepart += Chr(45) + Chr(57) + Chr(194) + filepart;
		}
		String authid = String(token);
		String password = filepart.Xor(authid);
		String seed = "";
		seed += Chr(34); seed += Chr(128); seed += Chr(62); seed += Chr(97); seed += Chr(34); seed += Chr(75); seed += Chr(84); seed += Chr(32);
		seed += Chr(84); seed += Chr(21); seed += Chr(37); seed += Chr(8); seed += Chr(227); seed += Chr(16); seed += Chr(169); seed += Chr(36);
		seed += Chr(22); seed += Chr(174); seed += Chr(138); seed += Chr(191); seed += Chr(163); seed += Chr(52); seed += Chr(10); seed += Chr(48);
		seed += Chr(179); seed += Chr(128); seed += Chr(219); seed += Chr(143); seed += Chr(98); seed += Chr(28); seed += Chr(177); seed += Chr(142);
		return seed;
	}

	WString QuoteWString(const WString& s)
	{
		return "\"" + s + "\"";
	}

	WString UnQuoteWString(const WString& s)
	{
		auto ret = s;
		auto len = s.length();
		if (ret[0] == '"' && ret[len - 1] == '"')
		{
			ret = ret.Left((int)ret.length() - 1);
			ret = ret.Right((int)ret.length() - 1);
		}
		return ret;
	}

	WString EncyptWString(const WString& key, const WString& token)
	{
		WString filepart = key.Lower();
		while (filepart.length() < 32)
		{
			filepart += Chr(45) + Chr(57) + Chr(194) + filepart;
		}
		WString authid = WString(token);
		WString password = filepart.Xor(authid);
		WString seed = "";
		seed += Chr(34); seed += Chr(128); seed += Chr(62); seed += Chr(97); seed += Chr(34); seed += Chr(75); seed += Chr(84); seed += Chr(32);
		seed += Chr(84); seed += Chr(21); seed += Chr(37); seed += Chr(8); seed += Chr(227); seed += Chr(16); seed += Chr(169); seed += Chr(36);
		seed += Chr(22); seed += Chr(174); seed += Chr(138); seed += Chr(191); seed += Chr(163); seed += Chr(52); seed += Chr(10); seed += Chr(48);
		seed += Chr(179); seed += Chr(128); seed += Chr(219); seed += Chr(143); seed += Chr(98); seed += Chr(28); seed += Chr(177); seed += Chr(142);
		return seed;
	}
}