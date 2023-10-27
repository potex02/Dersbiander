#include "Tokenizer.h"
#include "headers.h"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826)
#include <CLI/CLI.hpp>
DISABLE_WARNINGS_POP()
// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Dersbiander`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

static const std::string code = "42 + y + 1. + 1.0 + 1e+1 + 1E+1 + 1.1e+1 + 1.1E+1 + 1e-1 + 1E-1 + 1.1e-1 + 1.1E-1";

DISABLE_WARNINGS_PUSH(26821)
// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    const auto console = spdlog::stdout_color_mt(R"(console)");
    spdlog::set_default_logger(console);
    try {
        CLI::App app{D_FORMAT("{} version {}", Dersbiander::cmake::project_name, Dersbiander::cmake::project_version)};
        /*
        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        bool is_turn_based = false;
        auto *turn_based = app.add_flag("--turn_based", is_turn_based);

        bool is_loop_based = false;
        auto *loop_based = app.add_flag("--loop_based", is_loop_based);

        turn_based->excludes(loop_based);
        loop_based->excludes(turn_based);


        CLI11_PARSE(app, argc, argv);
        if (show_version) {
          return EXIT_SUCCESS;
        }
        */

        LINFO("{} version {}", Dersbiander::cmake::project_name, Dersbiander::cmake::project_version);
        LINFO("code legt {}", code.length());

        Tokenizer tokenizer(code);
        Timer timer("tokenizer.tokenize()");
        std::vector<Token> tokens = tokenizer.tokenize();
        LINFO(timer.to_string());
        for(std::span<Token> tokenSpan(tokens); const Token &token : tokenSpan) { LINFO("Token {}", token.toString()); }
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
    SYSPAUSE()
}
DISABLE_WARNINGS_POP()
