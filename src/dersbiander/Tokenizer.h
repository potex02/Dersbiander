#pragma once

#include "Timer.h"
#include "Token.h"
#include <span>
#include <string>
#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input);

    // delete copy constructor and copy assignment operator
    Tokenizer(const Tokenizer &) = delete;
    Tokenizer(Tokenizer &&) = delete;
    Tokenizer &operator=(const Tokenizer &) = delete;
    Tokenizer &operator=(Tokenizer &&) = delete;

    std::vector<Token> tokenize();

    void handleError(const std::string &values, const std::string &errorMsg);

private:
    std::string input;
    std::span<const char> inputSpan;
    std::size_t currentPosition = 0;
    std::size_t inputSize = 0;
    std::size_t currentLine = 1;
    std::size_t currentColumn = 1;

    inline void appendCharToValue(std::string &value);

    // NOLINTNEXTLINE(clang-diagnostic-*)
    [[nodiscard]] inline bool isPlusORMinus(char c) const noexcept;  // NOLINT(*-identifier-length)
    [[nodiscard]] inline bool isOperator(char c) const noexcept;     // NOLINT(*-identifier-length) NOLINT(functionStatic)
    Token extractIdentifier();
    void extractDigits(std::string &value);
    void extractExponent(std::string &value);
    Token extractnumber();
    Token extractOperator();
    void handleWhitespace(char currentChar) noexcept;
};
