#pragma once

#include <chrono>

class Timer {
public:

    Timer() : m_point(clock::now()) {}

    void reset() { m_point = clock::now(); }

    double elapsed() const
    {
        auto div = clock::now() - m_point;
        return std::chrono::duration_cast<second_t>(div).count();
    }

private:
    using clock = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock> m_point;
};
