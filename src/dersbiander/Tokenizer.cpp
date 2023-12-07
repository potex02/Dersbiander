#include "Tokenizer.h"
#include "Timer.h"
// #include "TokentizeErr.h"
#include "headers.h"
#include <compare>

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

Tokenizer::Tokenizer(const std::string &_input)
  : input(_input), inputSpan(input.c_str(), input.size()), inputSize(input.size()) {}

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
        } else if(isBrackets(currentChar)) {
            tokens.emplace_back(extractBrackets(currentChar));
        } else if(isApostrophe(currentChar)) {
            tokens.emplace_back(extractChar());
        } else if(std::isspace(currentChar)) {
            handleWhitespace(currentChar);
            continue;  // Continue the loop to get the next token
        } else {
            handleError(std::string(1, currentChar), "Unknown Character");
            std::exit(-1);  // Terminate the program with an error code
        }
    }

    tokens.emplace_back(eofTokenType, "", currentLine, currentColumn - 1);

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
// NOLINTBEGIN
bool Tokenizer::isPlusORMinus(char c) const noexcept { return c == '+' || c == '-'; }
bool Tokenizer::isOperator(char c) const noexcept {
    static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-'};
    return operators.contains(c);
}
bool Tokenizer::isOperationEqualOperator(const std::string &value) const noexcept {
    return value == "+=" || value == "-=" || value == "*=" || value == "/=";
}
bool Tokenizer::isBooleanOperator(const std::string &value) const noexcept {
    return value == "==" || value == ">=" || value == "<=" || value == "!=";
}
bool Tokenizer::isBrackets(char c) const noexcept { return c == '(' || c == ')'; }
bool Tokenizer::isLogicalOperator(const std::string &value) const noexcept { return value == "&&" || value == "||"; }
bool Tokenizer::isVarLenOperator(const std::string &val) const noexcept {
    return isOperator(val[0]) || isBrackets(val[0]) || isOperationEqualOperator(val) || isBooleanOperator(val) ||
           isLogicalOperator(val);
}

// NOLINTEND

Token Tokenizer::extractIdentifier() {
    std::string value;
    TokenType type = TokenType::IDENTIFIER;
    while(currentPosition < inputSize && (std::isalnum(inputSpan[currentPosition]) || inputSpan[currentPosition] == '_')) {
        appendCharToValue(value);
    }
    if(value == "var" || value == "const") { type = TokenType::KEYWORD_VAR; }
    return {type, value, currentLine, currentColumn - value.length()};
}

bool Tokenizer::isApostrophe(char c) const noexcept { return c == '\''; }

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
void Tokenizer::extractVarLenOperator(std::string &value) {
    while(currentPosition < inputSize && isOperator(inputSpan[currentPosition])) { appendCharToValue(value); }
}

TokenType Tokenizer::typeBySingleCharacter(char c) const {
    switch(c) {
        using enum TokenType;
    case '-':
        return MINUS_OPERATOR;
    case '=':
        return EQUAL_OPERATOR;
    case ',':
        return COMMA;
    case ':':
        return COLON;
    case '<':
    case '>':
        return BOOLEAN_OPERATOR;
    case '!':
        return NOT_OPERATOR;
    default:
        return OPERATOR;
    }
}

TokenType Tokenizer::typeByValue(const std::string &value) const {
    using enum TokenType;
    if(isOperationEqualOperator(value)) { return OPERATION_EQUAL; }
    if(isBooleanOperator(value)) { return BOOLEAN_OPERATOR; }
    if(isLogicalOperator(value)) { return LOGICAL_OPERATOR; }
    return UNKNOWN;
}

Token Tokenizer::extractOperator() {
    std::string value;
    extractVarLenOperator(value);
    TokenType type = value.size() == 1 ? typeBySingleCharacter(value[0]) : typeByValue(value);
    return {type, value, currentLine, currentColumn - value.length()};
}

Token Tokenizer::extractBrackets(char c) {
    using enum TokenType;
    TokenType type = UNKNOWN;
    if(c == '(') {
        type = OPEN_BRACKETS;
    } else if(c == ')') {
        type = CLOSED_BRACKETS;
    }
    ++currentPosition;
    ++currentColumn;
    return {type, std::string(1, c), currentLine, currentColumn - 1};
}

Token Tokenizer::extractChar() {
    std::size_t startcol = currentColumn;
    ++currentPosition;
    ++currentColumn;
    std::string value;
    while(!isApostrophe(inputSpan[currentPosition])) { appendCharToValue(value); }
    ++currentPosition;
    ++currentColumn;
    return {TokenType::CHAR, value, currentLine, currentColumn - startcol};
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

DISABLE_WARNINGS_POP()
