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
	extern bool CheckArgument(table table, const String& argument);
	extern WString CheckArgumentString(table table, const String& argument, const WString& defaultvalue = "");
	extern int CheckArgumentValue(table table, const String& argument, const int defaultvalue);
}