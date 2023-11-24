#include "Tokenizer.h"
#include "Instruction.h"
#include "headers.h"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26821 26826)
#include <CLI/CLI.hpp>
DISABLE_WARNINGS_POP()
// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Dersbiander`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

static const std::string code = "variable = 42 + y + 1. + 1.0 + 1e+1 + 1E+1 + 1.1e+1 + 1.1E+1 + 1e-1 + 1E-1 + 1.1e-1 + 1.1E-1";

/* // Strassen's matrix multiplication for 2x2 matrices
template <typename T> glm::mat<2, 2, T> strassen2x2(const glm::mat<2, 2, T> &A, const glm::mat<2, 2, T> &B) {
    auto M1 = (A[0][0] + A[1][1]) * (B[0][0] + B[1][1]);
    auto M2 = (A[1][0] + A[1][1]) * B[0][0];
    auto M3 = A[0][0] * (B[0][1] - B[1][1]);
    auto M4 = A[1][1] * (B[1][0] - B[0][0]);
    auto M5 = (A[0][0] + A[0][1]) * B[1][1];
    auto M6 = (A[1][0] - A[0][0]) * (B[0][0] + B[0][1]);
    auto M7 = (A[0][1] - A[1][1]) * (B[1][0] + B[1][1]);

    return glm::mat<2, 2, T>(M1 + M4 - M5 + M7, M3 + M5, M2 + M4, M1 - M2 + M3 + M6);
}

template <typename T> glm::mat<4, 4, T> strassen4x4(const glm::mat<4, 4, T> &A, const glm::mat<4, 4, T> &B) {
    // Dividi le matrici A e B in sottomatrici 2x2
    glm::mat<2, 2, T> A11(A[0][0], A[0][1], A[1][0], A[1][1]);
    glm::mat<2, 2, T> A12(A[0][2], A[0][3], A[1][2], A[1][3]);
    glm::mat<2, 2, T> A21(A[2][0], A[2][1], A[3][0], A[3][1]);
    glm::mat<2, 2, T> A22(A[2][2], A[2][3], A[3][2], A[3][3]);

    glm::mat<2, 2, T> B11(B[0][0], B[0][1], B[1][0], B[1][1]);
    glm::mat<2, 2, T> B12(B[0][2], B[0][3], B[1][2], B[1][3]);
    glm::mat<2, 2, T> B21(B[2][0], B[2][1], B[3][0], B[3][1]);
    glm::mat<2, 2, T> B22(B[2][2], B[2][3], B[3][2], B[3][3]);

    // Applica l'algoritmo di Strassen a ciascuna sottomatrice
    glm::mat<2, 2, T> C11 = strassen2x2(A11, B11) + strassen2x2(A12, B21);
    glm::mat<2, 2, T> C12 = strassen2x2(A11, B12) + strassen2x2(A12, B22);
    glm::mat<2, 2, T> C21 = strassen2x2(A21, B11) + strassen2x2(A22, B21);
    glm::mat<2, 2, T> C22 = strassen2x2(A21, B12) + strassen2x2(A22, B22);

    // Combina le sottomatrici C in una matrice 4x4
    glm::mat<4, 4, T> C;
    C[0][0] = C11[0][0];
    C[0][1] = C11[0][1];
    C[0][2] = C12[0][0];
    C[0][3] = C12[0][1];
    C[1][0] = C11[1][0];
    C[1][1] = C11[1][1];
    C[1][2] = C12[1][0];
    C[1][3] = C12[1][1];
    C[2][0] = C21[0][0];
    C[2][1] = C21[0][1];
    C[2][2] = C22[0][0];
    C[2][3] = C22[0][1];
    C[3][0] = C21[1][0];
    C[3][1] = C21[1][1];
    C[3][2] = C22[1][0];
    C[3][3] = C22[1][1];

    return C;
}

template <typename T> glm::mat<4, 4, T> getRandomMat4(T minValue, T maxValue) {
    // Create a random number generator engine
    std::mt19937 gen(std::random_device{}());

    // Define a uniform distribution for generating random numbers within the specified range
    std::uniform_real_distribution<T> dist(minValue, maxValue);

    glm::mat<4, 4, T> randomMat4;

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            // Generate random values for each element
            randomMat4[i][j] = dist(gen);
        }
    }

    return randomMat4;
}

template <typename T, int N> glm::mat<N, N, T> coppersmithWinograd(const glm::mat<N, N, T> &A, const glm::mat<N, N, T> &B) {
    if(A.length() != N || B.length() != N) { throw std::invalid_argument("Input matrices must be NxN"); }

    glm::mat<N, N, T> C(0.0f);

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) { C[i][j] += A[i][k] * B[k][j]; }
        }
    }

    return C;
}*/

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
        [[maybe_unused]] bool time_error = false;
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
            /* auto A4x4 = getRandomMat4<double>(1.0, 4.0);
            auto B4x4 = getRandomMat4<double>(5.0, 8.0);
            auto C = getRandomMat4<long double>(1.0f, 4.0);

            Timer timerglm4x("glm4x4");
            auto result4x4 = A4x4 * B4x4;
            LINFO(timerglm4x.to_string());
            Timer timer4x("strassen4x4");
            auto result2_4x4 = strassen4x4(A4x4, B4x4);
            LINFO(timer4x.to_string());
            Timer timercoppersmithWinograd4x("coppersmithWinograd4x4x4");
            auto result3_4x4 = coppersmithWinograd(A4x4, B4x4);
            LINFO(timercoppersmithWinograd4x.to_string());
            LINFO("glm Result for 4x4 matrices:\n{}", glmp::to_string(result4x4));
            LINFO("strassen Result for 4x4 matrices:\n{}", glmp::to_string(result2_4x4));
            LINFO("coppersmithWinograd Result for 4x4 matrices:\n{}", glmp::to_string(result3_4x4));
            LINFO("long double 4x4\n{}", glmp::to_string(C));*/
            // */

            // glm::mat4 A4x4(1.0f);
            //  glm::mat4 B4x4(2.0f);
            //  glm::mat4 result4x4 = strassen4x4(A4x4, B4x4);
            //  std::cout << "Result for 4x4 matrices:\n" << glm::to_string(result4x4) << std::endl;
            LINFO("code length {}", code.length());
            Tokenizer tokenizer(code);
            Timer timer("tokenizer.tokenize()");
            std::vector<Token> tokens = tokenizer.tokenize();
            Instruction instruction(tokens);
            LINFO(timer.to_string());
            for(std::span<Token> tokenSpan(tokens); const Token &token : tokenSpan) {
                LINFO("Token {}", std::move(token.typeToString()));
            }
            LINFO("{}", instruction.validate());
        }
    } catch(const std::exception &e) {
        LERROR("Unhandled exception in main: {}", e.what());
        return EXIT_FAILURE;
    }
}
DISABLE_WARNINGS_POP()
