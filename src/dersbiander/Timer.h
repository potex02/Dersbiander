#pragma once
#include "headers.h"

// On GCC < 4.8, the following define is often missing. Due to the
// fact that this library only uses sleep_for, this should be safe
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5 && __GNUC_MINOR__ < 8
#define _GLIBCXX_USE_NANOSLEEP
#endif

#include <iostream>

inline static constexpr long double MICROSENCONDSFACTOR = 1000.0;
inline static constexpr long double MILLISENCONDSFACTOR = 1'000'000.0;
inline static constexpr long double SENCONDSFACTOR = 1'000'000'000.0;
DISABLE_WARNINGS_PUSH(6005 26447 26455 26496)
class Timer {
protected:
    /// This is a typedef to make clocks easier to use
    using clock = std::chrono::high_resolution_clock;

    /// This typedef is for points in time
    using time_point = std::chrono::time_point<clock>;

    /// This is the type of a printing function, you can make your own
    using time_print_t = std::function<std::string(std::string, std::string)>;

    /// This is the title of the timer
    std::string title_;

    /// This is the function that is used to format most of the timing message
    time_print_t time_print_;

    /// This is the starting point (when the timer was created)
    time_point start_;

    /// This is the number of times cycles (print divides by this number)
    std::size_t cycles{1};

public:
    /// Standard print function, this one is set by default
    static std::string Simple(const std::string &title, const std::string &time) { return D_FORMAT("{}: {}", title, time); }

    static std::string Big(const std::string &title, const std::string &time) {
        return D_FORMAT("-----------------------------------------\n| {} | Time = {}\n-----------------------------------------",
                        title, time);
    }

public:
    /// Standard constructor, can set title and print function
    explicit Timer(std::string title = "Timer", const time_print_t &time_print = Simple)
      : title_(std::move(title)), time_print_(time_print), start_(clock::now()) {}

    Timer(const Timer &other) = delete;              // Delete copy constructor
    Timer &operator=(const Timer &other) = delete;   // Delete copy assignment operator
    Timer(const Timer &&other) = delete;             // Delete move constructor
    Timer &operator=(const Timer &&other) = delete;  // Delete move assignment operator

    /// Time a function by running it multiple times. Target time is the len to target.
    std::string time_it(std::function<void()> f, long double target_time = 1) {
        const time_point start = start_;
        long double total_time = NAN;

        start_ = clock::now();
        std::size_t n = 0;
        do {
            f();
            std::chrono::duration<long double> elapsed = clock::now() - start_;
            total_time = elapsed.count();
        } while(n++ < 100u && total_time < target_time);

        // std::string out = make_time_str(total_time / static_cast<long double>(n)) + " for " + std::to_string(n) + " tries";
        std::string out = D_FORMAT("{} for {} tries", make_time_str(total_time / static_cast<long double>(n)), std::to_string(n));
        start_ = start;
        return out;
    }

    inline long double make_time() const noexcept {
        const std::chrono::duration<long double, std::nano> elapsed = clock::now() - start_;
        return elapsed.count();
    }

    std::tuple<long double, long double, long double, long double, std::string, std::string, std::string, std::string>
    make_named_times(long double time) const {
        auto secondsTime = time / SENCONDSFACTOR;
        auto millisTime = time / MILLISENCONDSFACTOR;
        auto microTime = time / MICROSENCONDSFACTOR;
        std::tuple<long double, long double, long double, long double, std::string, std::string, std::string, std::string>
            myTuple;

        // Example initialization
        std::get<0>(myTuple) = secondsTime;
        std::get<1>(myTuple) = millisTime;
        std::get<2>(myTuple) = microTime;
        std::get<3>(myTuple) = time;
        std::get<4>(myTuple) = "s";
        std::get<5>(myTuple) = "ms";
        std::get<6>(myTuple) = "us";
        std::get<7>(myTuple) = "ns";

        return myTuple;
    }

    std::tuple<long double, long double, long double, long double, std::string, std::string, std::string, std::string>
    multi_time() const {
        const long double time = make_time();
        return make_named_times(time);
    }

    std::pair<long double, std::string> make_named_time(long double time) const {
        auto myTuple = make_named_times(time);
        // Accessing values
        auto ld1 = std::get<0>(myTuple);  // nano
        auto ld2 = std::get<1>(myTuple);  // micro
        auto ld3 = std::get<2>(myTuple);  // milli
        auto ld4 = std::get<3>(myTuple);  // seconds
        auto str1 = std::get<4>(myTuple);
        auto str2 = std::get<5>(myTuple);
        auto str3 = std::get<6>(myTuple);
        auto str4 = std::get<7>(myTuple);

        if(ld1 > 1)
            return std::make_pair(ld1, str1);
        else if(ld2 > 1)
            return std::make_pair(ld2, str2);
        else if(ld3 > 1)
            return std::make_pair(ld3, str3);
        else
            return std::make_pair(ld4, str4);
    }

    /// This formats the numerical value for the time string
    inline std::string make_time_str() const {  // NOLINT(modernize-use-nodiscard)
        const long double time = make_time() / static_cast<long double>(cycles);
        return make_time_str(time);
    }

    // LCOV_EXCL_START
    /// This prints out a time string from a time
    std::string make_time_str(long double time) const {  // NOLINT(modernize-use-nodiscard)
        auto [titme, stime] = make_named_time(time);
        return D_FORMAT("{:.f} {}", titme, stime);
    }
    // LCOV_EXCL_STOP

    /// This is the main function, it creates a string
    std::string to_string() const noexcept { return time_print_(title_, make_time_str()); }  // NOLINT(modernize-use-nodiscard)

    /// Division sets the number of cycles to divide by (no graphical change)
    Timer &operator/(std::size_t val) noexcept {
        cycles = val;
        return *this;
    }
};

/// This class prints out the time upon destruction
class AutoTimer : public Timer {
public:
    /// Reimplementing the constructor is required in GCC 4.7
    explicit AutoTimer(const std::string &title = "Timer", const time_print_t &time_print = Simple) : Timer(title, time_print) {}
    // GCC 4.7 does not support using inheriting constructors.
    AutoTimer(const AutoTimer &other) = delete;              // Delete copy constructor
    AutoTimer &operator=(const AutoTimer &other) = delete;   // Delete copy assignment operator
    AutoTimer(const AutoTimer &&other) = delete;             // Delete move constructor
    AutoTimer &operator=(const AutoTimer &&other) = delete;  // Delete move assignment operator

    /// This destructor prints the string
    ~AutoTimer() { LINFO(to_string()); }
};

/// This prints out the time if shifted into a std::cout like stream.
inline std::ostream &operator<<(std::ostream &in, const Timer &timer) { return in << timer.to_string(); }
DISABLE_WARNINGS_POP()
