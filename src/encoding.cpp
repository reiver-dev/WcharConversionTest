#include "encoding.hpp"

#include <memory>
#include <locale>
#include <codecvt>

using cvt_type = std::codecvt<wchar_t, char, std::mbstate_t>;

static const cvt_type &current_codecvt()
{
    return std::use_facet<cvt_type>(std::locale());
}

static const size_t CONVERT_STATIC_BUFFER_SIZE = 256;

static wchar_t *widen_impl(const char *begin, const char *end,
                           wchar_t *bufbegin, wchar_t *bufend,
                           const cvt_type& cvt)
{
    std::mbstate_t state = std::mbstate_t();

    const char *inp;
    wchar_t *bufp;

    auto result = cvt.in(state, begin, end, inp, bufbegin, bufend, bufp); 
    if (result == cvt_type::error) {
        throw std::range_error("bad encoding");
    }

    return bufp;
}

static char *narrow_impl(const wchar_t *begin, const wchar_t *end,
                         char *bufbegin, char *bufend,
                         const cvt_type& cvt)
{
    std::mbstate_t state = std::mbstate_t();

    const wchar_t *inp;
    char *bufp;

    auto result = cvt.out(state, begin, end, inp, bufbegin, bufend, bufp); 
    if (result == cvt_type::error) {
        throw std::range_error("bad encoding");
    }

    return bufp;
}

void widen_base(const char *str, size_t len, std::wstring &output)
{
    auto &cvt = current_codecvt();
    const size_t buflen = len;

    if (buflen < CONVERT_STATIC_BUFFER_SIZE) {
        wchar_t buffer[CONVERT_STATIC_BUFFER_SIZE];
        wchar_t *end = widen_impl(str, str + len, buffer, buffer + CONVERT_STATIC_BUFFER_SIZE, cvt);
        output.append(buffer, end);
    } else {
        auto buffer = std::make_unique<wchar_t[]>(buflen);
        wchar_t *end = widen_impl(str, str + len, buffer.get(), buffer.get() + buflen, cvt);
        output.append(buffer.get(), end);
    }
}


void narrow_base(const wchar_t *str, size_t len, std::string &output)
{
    auto &cvt = current_codecvt();
    const size_t buflen = len * cvt.max_length() + 4;

    if (buflen < CONVERT_STATIC_BUFFER_SIZE) {
        char buffer[CONVERT_STATIC_BUFFER_SIZE];
        char *end = narrow_impl(str, str + len, buffer, buffer + CONVERT_STATIC_BUFFER_SIZE, cvt);
        output.append(buffer, end);
    } else {
        auto buffer = std::make_unique<char[]>(buflen);
        char *end = narrow_impl(str, str + len, buffer.get(), buffer.get() + buflen, cvt);
        output.append(buffer.get(), end);
    }
}


void widen_base2(const char *str, size_t len, std::wstring &output)
{
    auto &cvt = current_codecvt();
    const size_t buflen = len;
    const size_t point = 0;

    output.assign(buflen, '\0');

    wchar_t *end = widen_impl(str, str + len, &output[point], &output[buflen], cvt);

    auto last = end - &output[point];
    output.erase(last);
}

void narrow_base2(const wchar_t *str, size_t len, std::string &output)
{
    auto &cvt = current_codecvt();
    const size_t buflen = len * cvt.max_length() + 4;
    const size_t point = 0;

    output.assign(buflen, '\0');

    char *end = narrow_impl(str, str + len, &output[point], &output[buflen], cvt);

    auto last = end - &output[point];
    output.erase(last);
}

std::string enc::narrow(const std::wstring& str)
{
    std::string result;
    if (!str.empty())
        narrow_base(str.data(), str.length(), result);
    return result;
}


std::wstring enc::widen(const std::string& str)
{
    std::wstring result;
    if (!str.empty())
        widen_base(str.data(), str.length(), result);
    return result;
}

std::string enc::narrow2(const std::wstring & str)
{

    std::string result;
    if (!str.empty())
        narrow_base2(str.data(), str.length(), result);
    return result;
}

std::wstring enc::widen2(const std::string & str)
{
    std::wstring result;
    if (!str.empty())
        widen_base2(str.data(), str.length(), result);
    return result;
}
