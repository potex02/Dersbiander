#include "Tokenizer.h"
#include "Timer.h"
#include "TokentizeErr.h"
#include "headers.h"
#include <compare>



Tokenizer::Tokenizer(const std::string &input) : input(input), inputSpan(input.c_str(), input.size()), inputSize(input.size()) {}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    while(currentPosition < inputSize) {
        const char currentChar = inputSpan[currentPosition];

        if(std::isalpha(currentChar)) {
            tokens.emplace_back(extractIdentifier());
        } else if(std::isdigit(currentChar)) {
            tokens.emplace_back(extractnumber());
        } else if(isOperator(currentChar)) {
            tokens.emplace_back(extractOperator());
        } else if(std::isspace(currentChar)) {
            handleWhitespace(currentChar);
            continue;  // Continue the loop to get the next token
        } else {
            handleError(std::string(1, currentChar), "Unknown Character");
            std::exit(-1);  // Terminate the program with an error code
        }
    }

    tokens.emplace_back(TokenType::EOFT, "", currentLine, currentColumn - 1);

    return tokens;
}

void Tokenizer::handleError(const std::string &values, const std::string &errorMsg) {
    Timer timer(errorMsg);
    std::ostringstream errorMessage;
    errorMessage << D_FORMAT("{} '{}' (line {}, column {}):\n", errorMsg, values, currentLine, currentColumn);

    errorMessage << "Context:" << NEWL;

    std::size_t lineStart = currentPosition;
    std::size_t lineEnd = currentPosition;

    while(lineStart > 0 && inputSpan[lineStart - 1] != CNL) { --lineStart; }
    while(lineEnd < inputSize && inputSpan[lineEnd] != CNL) { ++lineEnd; }
    errorMessage << std::string_view(input.data() + lineStart, lineEnd - lineStart) << NEWL;

    errorMessage << std::string(currentPosition - lineStart, ' ') << std::string(values.length(), '^') << NEWL;
    auto time = timer.to_string();
    LERROR("{}{}", std::move(errorMessage).str(), std::move(time));
}

void Tokenizer::appendCharToValue(std::string &value) {
    value += inputSpan[currentPosition];
    ++currentPosition;
    ++currentColumn;
}

bool Tokenizer::isPlusORMinus(char c) const noexcept { return (c == '+' || c == '-'); }

bool Tokenizer::isOperator(char c) const noexcept { return (isPlusORMinus(c) || c == '*' || c == '/' || c == '='); }

Token Tokenizer::extractIdentifier() {
    std::string value;
    while(currentPosition < inputSize && (std::isalnum(inputSpan[currentPosition]) || inputSpan[currentPosition] == '_')) {
        appendCharToValue(value);
    }
    return {TokenType::IDENTIFIER, value, currentLine, currentColumn - value.length()};
}

void Tokenizer::extractDigits(std::string &value) {
    while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) { appendCharToValue(value); }
}

void Tokenizer::extractExponent(std::string &value) {
    if(currentPosition < inputSize && isPlusORMinus(inputSpan[currentPosition])) { appendCharToValue(value); }
    extractDigits(value);
}

Token Tokenizer::extractnumber() {
    std::string value;
    extractDigits(value);

    if(currentPosition < inputSize && inputSpan[currentPosition] == PNT) {
        appendCharToValue(value);
        extractDigits(value);

        if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == ECR) {
            appendCharToValue(value);
            extractExponent(value);
        }
        return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
    }

    if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == ECR) {
        appendCharToValue(value);
        extractExponent(value);
        return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
    }
    return {TokenType::INTEGER, value, currentLine, currentColumn - value.length()};
}

Token Tokenizer::extractOperator() {
    auto value = input.substr(currentPosition, 1);
    ++currentPosition;
    ++currentColumn;
    return {TokenType::OPERATOR, value, currentLine, currentColumn - 1};
}

void Tokenizer::handleWhitespace(char currentChar) noexcept {
    if(currentChar == CNL) {
        ++currentLine;
        currentColumn = 1;
    } else {
        ++currentColumn;
    }
    ++currentPosition;
}
