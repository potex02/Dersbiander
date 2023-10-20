#include "TokentizeErr.h"
#include "headers.h"

enum class TokenType : int { IDENTIFIER, INTEGER, OPERATOR, KEYWORD, EOFT, ERROR, UNKNOWN };

struct Token {
    TokenType type{TokenType::UNKNOWN};
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

    [[nodiscard]] inline std::string toString() const {
        return D_FORMAT("type: {}, value: {}, line {}, column {}", typeToString(), value, line, column);
    }
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input) : input(input) {}

    Token getNextToken() {
        if(currentPosition >= inputSize) { return {TokenType::EOFT, "", currentLine, currentColumn}; }

        const char currentChar = input.at(currentPosition);
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
            handleError(currentChar, "Unknown Character");
            std::exit(-1);  // Terminate the program with an error code
        }
    }

    void handleError(const char currentChar, const std::string &errorMsg) {
        std::stringstream errorMessage;
        auto values = std::string(1, currentChar);
        errorMessage << D_FORMAT("{} '{}' (line {}, column {}):\n", errorMsg, values, currentLine, currentColumn);

        // Add context information to the error message
        errorMessage << "Context:" << NEWL;

        std::size_t lineStart = currentPosition;
        std::size_t lineEnd = currentPosition;

        while(lineStart > 0 && input[lineStart - 1] != CNL) { lineStart--; }
        while(lineEnd < inputSize && input[lineEnd] != CNL) { lineEnd++; }
        errorMessage << input.substr(lineStart, lineEnd - lineStart) << NEWL;

        // Include a marker pointing to the position of the error
        errorMessage << std::string(currentPosition - lineStart, ' ') << std::string(values.length(), '^') << NEWL;

        LERROR(errorMessage.str());
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token;
        currentLine = 1;
        currentColumn = 1;

        const auto eofTokenType = TokenType::EOFT;
        while((token = getNextToken()).type != eofTokenType) { tokens.emplace_back(token); }

        return tokens;
    }

private:
    std::string input;
    std::size_t currentPosition = 0;
    std::size_t inputSize = input.size();
    std::size_t currentLine = 1;
    std::size_t currentColumn = 1;

    [[nodiscard]] inline bool isOperator(char c) const noexcept {
        // Add logic to recognize specific operators
        // Example:
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    Token extractIdentifier() {
        std::string value;
        while(currentPosition < inputSize && (std::isalnum(input.at(currentPosition)) || input.at(currentPosition) == '_')) {
            value += input.at(currentPosition);
            ++currentPosition;
            ++currentColumn;
        }
        return {TokenType::IDENTIFIER, value, currentLine, currentColumn - value.length()};
    }

    Token extractInteger() {
        std::string value;
        while(currentPosition < inputSize && std::isdigit(input.at(currentPosition))) {
            value += input.at(currentPosition);
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

    void handleWhitespace(char currentChar) noexcept {
        if(currentChar == '\n') {
            currentLine++;
            currentColumn = 1;
        } else {
            currentColumn++;
        }
        currentPosition++;
    }
};
