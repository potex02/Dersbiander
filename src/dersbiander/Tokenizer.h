#include "headers.h"

enum class TokenType : int { IDENTIFIER, INTEGER, OPERATOR, KEYWORD, EOFT, ERROR };

struct Token {
    TokenType type;
    std::string value;
    std::size_t line{};
    std::size_t column{};

    [[nodiscard]] inline std::string typeToString() const noexcept {
        switch(type) {
            using enum TokenType;
        case IDENTIFIER:
            return "IDENTIFIER";
        case INTEGER:
            return "INTEGER";
        case OPERATOR:
            return "OPERATOR";
        case KEYWORD:
            return "KEYWORD";
        case EOFT:
            return "EOFT";
        case ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    [[nodiscard]] inline std::string toString() const noexcept {
        return D_FORMAT("type: {}, value: {}, line {}, column {}", typeToString(), value, line, column);
    }
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input) : input(input) { inputSize = input.size(); }

    Token getNextToken() {
        if(currentPosition >= inputSize) { return {TokenType::EOFT, "", currentLine, currentColumn}; }

        char currentChar = input[currentPosition];
        if(std::isalpha(currentChar)) {
            return extractIdentifier();
        } else if(std::isdigit(currentChar)) {
            return extractInteger();
        } else if(isOperator(currentChar)) {
            return extractOperator();
        } else if(std::isspace(currentChar)) {
            handleWhitespace(currentChar);
            return getNextToken();
        } else {
            // Handle unknown or invalid characters as an error
            return handleError("Unknown character", currentLine, currentColumn);
        }
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token;
        currentLine = 1;
        currentColumn = 1;

        while((token = getNextToken()).type != TokenType::EOFT) { tokens.emplace_back(token); }

        return tokens;
    }

private:
    std::string input;
    std::size_t currentPosition = 0;
    std::size_t inputSize;
    std::size_t currentLine = 1;
    std::size_t currentColumn = 1;

    [[nodiscard]] inline bool isOperator(char c) const {
        // Add logic to recognize specific operators
        // Example:
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    Token extractIdentifier() {
        std::string value;
        while(currentPosition < inputSize && (std::isalnum(input[currentPosition]) || input[currentPosition] == '_')) {
            value += input[currentPosition];
            ++currentPosition;
            ++currentColumn;
        }
        return {TokenType::IDENTIFIER, value, currentLine, currentColumn - value.length()};
    }

    Token extractInteger() {
        std::string value;
        while(currentPosition < inputSize && std::isdigit(input[currentPosition])) {
            value += input[currentPosition];
            currentPosition++;
            currentColumn++;
        }
        return {TokenType::INTEGER, value, currentLine, currentColumn - value.length()};
    }

    Token extractOperator() {
        auto value = input.substr(currentPosition, 1);
        currentPosition++;
        currentColumn++;
        return {TokenType::OPERATOR, value, currentLine, currentColumn - 1};
    }

    void handleWhitespace(char currentChar) {
        if(currentChar == '\n') {
            currentLine++;
            currentColumn = 1;
        } else {
            currentColumn++;
        }
        currentPosition++;
    }

    Token handleError(const std::string &message, std::size_t line, std::size_t column) {
        // You can customize error handling as needed
        return {TokenType::ERROR, message, line, column};
    }
};
