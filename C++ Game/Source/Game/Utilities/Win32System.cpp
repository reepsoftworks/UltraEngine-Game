#include "UltraEngine.h"

#ifdef _WIN32
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#endif

#include "Win32System.h"

// https://learn.microsoft.com/en-au/windows/win32/shell/links?redirectedfrom=MSDN#creating-a-shortcut-and-a-folder-shortcut-to-a-file
bool CreateShortcut(const WString& path, const WString& dest, const WString& desc)
{
#if defined (_WIN32)
    auto rpath = RealPath(path).Replace("/", "\\");
    auto rdest = RealPath(dest).Replace("/", "\\");

    String s1 = rpath.ToUtf8String();
    String s2 = desc.ToUtf8String();
    String s3 = ExtractDir(rpath).Replace("/", "\\").ToUtf8String();

    LPCSTR lpszPathObj = s1.c_str();
    LPCWSTR lpszPathLink = rdest.c_str();
    LPCSTR lpszDesc = s2.c_str();

    CoInitialize(NULL);
    HRESULT hres;
    IShellLink* psl;
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (not SUCCEEDED(hres)) return false;
    IPersistFile* ppf;
    psl->SetPath(lpszPathObj);
    psl->SetWorkingDirectory(s3.c_str());
    psl->SetDescription(lpszDesc);
    hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
    if (SUCCEEDED(hres))
    {
        hres = ppf->Save(lpszPathLink, TRUE);
        ppf->Release();
    }
    psl->Release();
    return hres == S_OK;
#else
    return false;
#endif
}

WString GetPath(const UserPathEx pathid)
{
#if defined (_WIN32)
    if (pathid == PATH_PROGRAMS)
    {
        PIDLIST_ABSOLUTE pidlist;
        wchar_t path[MAX_PATH];
        HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidlist);
        BOOL b = SHGetPathFromIDListW(pidlist, path);
        return WString(path);
    }
#endif
    return L"";
}

// https://stackoverflow.com/questions/51334674/how-to-detect-windows-10-light-dark-mode-in-win32-application
bool LightThemeEnabled()
{
#if defined (_WIN32)
    // The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
    auto buffer = std::vector<char>(4);
    auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
    auto res = RegGetValueW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        L"AppsUseLightTheme",
        RRF_RT_REG_DWORD, // expected value type
        nullptr,
        buffer.data(),
        &cbData);

    if (res != ERROR_SUCCESS) {
        throw std::runtime_error("Error: error_code=" + std::to_string(res));
    }

    // convert bytes written to our buffer to an int, assuming little-endian
    auto i = int(buffer[3] << 24 |
        buffer[2] << 16 |
        buffer[1] << 8 |
        buffer[0]);

    return i == 1;
#else
    return false;
#endif
}

bool SetWindowTitlebarTheme(shared_ptr<Window> source, const TitlebarTheme theme)
{
#if defined (_WIN32)
    BOOL darkmode = (BOOL)theme;
    if (theme == TITLEBAR_SYSTEM) darkmode = !LightThemeEnabled();
    BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
        source->GetHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, // 20
        &darkmode, sizeof(darkmode)));

    return SET_IMMERSIVE_DARK_MODE_SUCCESS == darkmode;
#else
    return false;
#endif
}

void CallCMDWindow()
{
#if defined (_WIN32)
    AllocConsole();
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);
#endif
}