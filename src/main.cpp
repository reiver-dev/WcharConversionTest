#include <iostream>
#include <chrono>
#include <algorithm>

#include "encoding.hpp"
#include "wconvert.hpp"
#include "timer.hpp"

// grüßen
const wchar_t *TEXTW = L" \u0067\u0072\u00FC\u00DF\u0065\u006E";
const char    *TEXTN =  " \x67\x72\xC3\xBC\xC3\x9F\x65\x6E";


void setupLocale()
{
    std::locale::global(
        std::locale(
            std::locale(),
            new std::codecvt_utf8_utf16<wchar_t>()
            )
        );
}


const size_t WARMUP = 1000;
const size_t BENCH = 100000;

void test(std::wstring w, std::string n)
{
    auto wwconv = wconv::widen(n);
    auto nwconv = wconv::narrow(w);

    auto wenc = enc::widen(n);
    auto nenc = enc::narrow(w);

    auto wenc2 = enc::widen2(n);
    auto nenc2 = enc::narrow2(w);


    if (wwconv != wenc || wwconv != wenc2)
        throw std::runtime_error("Widen failed");

    if (nwconv != nenc || nwconv != nenc2)
        throw std::runtime_error("Narrow failed");

    if (wenc != w || wenc2 != w)
        throw std::runtime_error("Widen single failed");

    if (nenc != n || nenc2 != n)
        throw std::runtime_error("Narrow single failed");
}


void benchmark(std::wstring w, std::string n, size_t warmup, size_t bench)
{
    Timer t;
    double convert;
    size_t convert_wcap = 0;
    size_t convert_ncap = 0;

    double encoding;
    size_t encoding_ncap = 0;
    size_t encoding_wcap = 0;

    double encoding2;
    size_t encoding2_ncap = 0;
    size_t encoding2_wcap = 0;

    {
        std::string s1;
        std::wstring s2;

        for (size_t i = 0; i < warmup; i++) {
            s1 = wconv::narrow(w);
            s2 = wconv::widen(s1);
            s2 = wconv::widen(n);
            s1 = wconv::narrow(s2);
        }

        t.reset();

        for (size_t i = 0; i < bench; i++) {
            s1 = wconv::narrow(w);
            s2 = wconv::widen(s1);
            s2 = wconv::widen(n);
            s1 = wconv::narrow(s2);
        }

        convert = t.elapsed();

        convert_ncap = s1.capacity();
        convert_wcap = s2.capacity();
    }
    

    {
        std::string s1;
        std::wstring s2;

        for (size_t i = 0; i < warmup; i++) {
            s1 = enc::narrow(w);
            s2 = enc::widen(s1);
            s2 = enc::widen(n);
            s1 = enc::narrow(s2);
        }

        t.reset();

        for (size_t i = 0; i < bench; i++) {
            s1 = enc::narrow(w);
            s2 = enc::widen(s1);
            s2 = enc::widen(n);
            s1 = enc::narrow(s2);
        }

        encoding = t.elapsed();

        encoding_ncap = s1.capacity();
        encoding_wcap = s2.capacity();
    }

    {
        std::string s1;
        std::wstring s2;

        for (size_t i = 0; i < warmup; i++) {
            s1 = enc::narrow2(w);
            s2 = enc::widen2(s1);
            s2 = enc::widen2(n);
            s1 = enc::narrow2(s2);
        }

        t.reset();

        for (size_t i = 0; i < bench; i++) {
            s1 = enc::narrow2(w);
            s2 = enc::widen2(s1);
            s2 = enc::widen2(n);
            s1 = enc::narrow2(s2);
        }

        encoding2 = t.elapsed();

        encoding2_ncap = s1.capacity();
        encoding2_wcap = s2.capacity();
    }



    std::cout << "WStringConvert: " << convert << " n: " << convert_ncap  << " w: " << convert_wcap << std::endl;
    std::cout << "Encoding: " << encoding << " n: " << encoding_ncap  << " w: " << encoding_wcap << std::endl;
    std::cout << "Encoding2: " << encoding2 << " n: " << encoding2_ncap  << " w: " << encoding2_wcap << std::endl;
}


int main(int argc, char *argv[])
{
    setupLocale();

    std::ios_base::sync_with_stdio(false);

    std::cout.imbue(std::locale());
    std::wcout.imbue(std::locale());

    std::wstring wshort;
    std::string nshort;

    std::wstring wmid;
    std::string nmid;

    std::wstring wlong;
    std::string nlong;

    for (size_t i = 0; i < 3; ++i) {
        wshort += TEXTW;
        nshort += TEXTN;
    }

    for (size_t i = 0; i < 30; ++i) {
        wmid += TEXTW;
        nmid += TEXTN;
    }

    for (size_t i = 0; i < 300; ++i) {
        wlong += TEXTW;
        nlong += TEXTN;
    }

    test(wshort, nshort);
    test(wmid, nmid);
    test(wlong, nlong);

    std::cout << "Test succeeded!" << std::endl;

    std::cout << "Short" << std::endl;
    benchmark(wshort, nshort, WARMUP, BENCH * 10);

    std::cout << "Mid" << std::endl;
    benchmark(wmid, nmid, WARMUP, BENCH);

    std::cout << "Long" << std::endl;
    benchmark(wlong, nlong, WARMUP, BENCH);
}