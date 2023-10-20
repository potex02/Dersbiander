#include "Tokenizer.h"
#include "headers.h"
#include <CLI/CLI.hpp>
// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Dersbiander`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

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
        std::string code = "42 + y,";
        Tokenizer tokenizer(code);
        std::vector<Token> tokens = tokenizer.tokenize();
        for(const Token &token : tokens) { LINFO("Token {}", token.toString()); }
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
    SYSPAUSE()
}
