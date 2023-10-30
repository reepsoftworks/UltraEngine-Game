//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "CheckArgument.h"

namespace UltraEngine::Game
{
	bool CheckArgument(table table, const String& argument)
	{
		return table[argument].is_boolean() && table[argument] == true;
	}

	WString CheckArgumentString(table table, const String& argument, const WString& defaultvalue)
	{
		WString ret = defaultvalue;
		if (table[argument].is_string())
		{
			ret = table[argument];
		}
		return ret;
	}

	int CheckArgumentValue(table table, const String& argument, const int defaultvalue)
	{
		int ret = defaultvalue;
		if (table[argument].is_integer())
		{
			ret = table[argument];
		}
		return ret;
	}
}