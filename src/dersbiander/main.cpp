#include "Instruction.h"
#include "Tokenizer.h"
#include "headers.h"

#define ONLY_TOKEN_TYPE

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826)

#include <CLI/CLI.hpp>

DISABLE_WARNINGS_POP()
// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Dersbiander`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

static const std::array<std::string, 14> code = {
    "var variable: type",
    "var num1, num2: type = 1 + 1, variable * -5",
    "variable = 42 * -y + 1. + 1.0 + 1e+1 + 1E+1 + 1.1e+1 + 1.1E+1 + 1e-1 + 1E-1 + 1.1e-1 + 1.1E-1",
    "",
    "true false",
    "variable == -4",
    "variable < -4",
    "variable = -4",
    "variable += -4",
    "variable -= -4",
    "variable *= -4",
    "variable /= -4",
    "variable && -4",
    "!variable"};

DISABLE_WARNINGS_PUSH(26461 26821)

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv) {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    const auto console = spdlog::stdout_color_mt(R"(console)");
    spdlog::set_default_logger(console);
    try {
        CLI::App app{D_FORMAT("{} version {}", Dersbiander::cmake::project_name, Dersbiander::cmake::project_version)};

        bool show_version = false;
        bool run_code_from_console = false;
        //[[maybe_unused]] bool time_error = false;
        std::optional<std::string> input;
        app.add_option("-i,--input", input, "A message to print back out");
        app.add_flag("--version", show_version, "Show version information");
        app.add_flag("--jit", run_code_from_console, "Run code from the console");

        CLI11_PARSE(app, argc, argv)
        if(show_version) {
            LINFO("{} version {}", Dersbiander::cmake::project_name, Dersbiander::cmake::project_version);
            return EXIT_SUCCESS;
        }

        if(run_code_from_console) {
            // if(input.has_value()) {
            // } else {
            // }
        } else {
            Timer tim("tokenizer total time");
            for(std::string str : code) {
                LINFO("code {}", str);
                LINFO("code length {}", str.length());
                Tokenizer tokenizer(str);
                Timer timer("tokenizer.tokenize()");
                std::vector<Token> tokens = tokenizer.tokenize();
                LINFO(timer.to_string());
                Instruction instruction(tokens);
                for(std::span<Token> tokenSpan(tokens); const Token &token : tokenSpan) {
#ifdef ONLY_TOKEN_TYPE
                    LINFO("Token {}", token.typeToString());
#else
                    LINFO("{}", token.toString());
#endif  // ONLY_TOKEN_TYPE
                }
                Timer time("instruction.validate()");
                const std::string valiadation = instruction.validate();
                LINFO(time.to_string());
                LINFO("{}", valiadation);
            }
            LINFO(tim.to_string());
        }
    } catch(const std::exception &e) {
        LERROR("Unhandled exception in main: {}", e.what());
        return EXIT_FAILURE;
    }
}

DISABLE_WARNINGS_POP()
