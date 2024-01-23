#pragma once

#include <chrono>
#include <iostream>

class LogDuration {
public:
    using Clock = std::chrono::steady_clock;

    LogDuration(const std::string& operation) : operation_(operation) {}

    ~LogDuration() {
        using namespace std::chrono;
        using std::literals::string_literals::operator""s;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        std::cerr << operation_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s
                  << std::endl;
    }

private:
    const Clock::time_point start_time_ = Clock::now();
    const std::string operation_;
};
