#pragma once

#include <string>
#include <codecvt>

namespace wconv
{

inline const std::string narrow(const std::wstring& s)
{
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(s);
}

inline const std::wstring widen(const std::string& s)
{
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(s);
}


}
