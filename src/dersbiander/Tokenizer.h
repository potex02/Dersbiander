#pragma once
#include "Timer.h"
#include "Token.h"
#include <span>
#include <string>
#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input);

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
    [[nodiscard]] inline bool isPlusORMinus(char c) const noexcept;  // NOLINT(*-identifier-length)
    [[nodiscard]] inline bool isOperator(char c) const noexcept; // NOLINT(*-identifier-length)
    Token extractIdentifier();
    void extractDigits(std::string &value);
    void extractExponent(std::string &value);
    Token extractnumber();
    Token extractOperator();
    void handleWhitespace(char currentChar) noexcept;
};
