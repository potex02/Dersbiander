#pragma once
// NOLINTBEGIN
#include "disableWarn.h"
// clang-format off
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 
    6244 6285 6385 6386 26409 26415 26418
    26429 26432 26437 26438 26440 26446 26447 
    26450 26451 26455 26457 26459 26460 26461 
    26467 26472 26473 26474 26475 26481 26482 
    26485 26490 26491 26493 26494 26495 26496 
    26497 26498 26800 26814 26818 26826)
#include <cassert>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execution>
#ifdef _WIN32 // Check if the target platform is Windows
  #ifdef _MSC_VER // Check if the compiler is MSVC
    #include <format>
  #endif
#endif
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <numbers>
#include <omp.h>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <spdlog/cfg/env.h>
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <source_location>
#include <type_traits>

// clang-format on
#include "casts.h"
// Restore warning levels.
DISABLE_WARNINGS_POP()

#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

// clang-format off
#ifdef _WIN32
     #ifdef _MSC_VER
         #ifdef USE_STD_FORMAT
              #define D_FORMAT(...) std::format(__VA_ARGS__)
         #else
              #define D_FORMAT(...) fmt::format(__VA_ARGS__)
         #endif
     #endif
#else
      #define D_FORMAT(...) fmt::format(__VA_ARGS__)
#endif
// clang-format on
static inline constexpr char CNL = '\n';
static inline constexpr char CCR = '\r';
static inline constexpr const char *CRNL = "\r\n";

#ifdef _WIN32                              // Windows
static inline constexpr auto NEWL = CRNL;  // Windows
#elif defined macintosh                    // OS 9
static inline constexpr const char *NEWL = CCR;  // Classic Mac OS
#elif defined __unix__                     // Linux and Unix-like systems
static inline constexpr const char *NEWL = CNL;  // Linux and Unix
#elif defined __APPLE__                    // macOS
static inline constexpr const char *NEWL = CNL;  // macOS
#elif defined __VMS                        // OpenVMS
static inline constexpr const char *NEWL = CRNL;  // OpenVMS
#elif defined __FreeBSD__                  // FreeBSD
static inline constexpr const char *NEWL = CNL;  // FreeBSD
#else
static inline constexpr const char *NEWL = CNL;  // Default case
#endif

#define SYSPAUSE()                                                                                                               \
    do {                                                                                                                         \
        LINFO("Press enter to exit...");                                                                                         \
        std::cin.ignore();                                                                                                       \
    } while(0);

static inline constexpr long double NINFINITY = std::numeric_limits<long double>::infinity();
static inline constexpr long double PI = std::numbers::pi_v<long double>;
static inline constexpr long double TWO_PI = 2 * PI;
static inline constexpr long double HALF_PI = PI / 2;
// NOLINTEND
