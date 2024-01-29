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

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)
#define LOG_DURATION(operation) LogDuration UNIQUE_VAR_NAME_PROFILE(operation)
