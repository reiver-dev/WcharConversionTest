#pragma once

#include <string>

namespace enc
{

std::string narrow(const std::wstring& str);
std::wstring widen(const std::string& str);

std::string narrow2(const std::wstring& str);
std::wstring widen2(const std::string& str);

}