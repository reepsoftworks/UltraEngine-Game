#pragma once
#include "UltraEngine.h"

using namespace UltraEngine;

enum UserPathEx
{
    PATH_PROGRAMS // start menu programs path
};

enum TitlebarTheme
{
    TITLEBAR_DEFAULT,
    TITLEBAR_DARK,
    TITLEBAR_SYSTEM
};

extern bool CreateShortcut(const WString& path, const WString& dest, const WString& desc = "");
extern WString GetPath(const UserPathEx pathid);
extern bool LightThemeEnabled();
extern bool SetWindowTitlebarTheme(shared_ptr<Window> source, const TitlebarTheme theme);
extern void CallCMDWindow();