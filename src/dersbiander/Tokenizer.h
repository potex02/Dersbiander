#include "Timer.h"
#include "TokentizeErr.h"
#include "headers.h"
#include <compare>

enum class TokenType : int { IDENTIFIER, INTEGER, DOUBLE, OPERATOR, KEYWORD, EOFT, ERROR, UNKNOWN };

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
        case DOUBLE:
            return "DOUBLE";
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

    auto operator<=>(const Token &other) const = default;
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input)
      : input(input), inputSpan(input.c_str(), input.size()), inputSize(input.size()) {}

    Token getNextToken() {
        if(currentPosition >= inputSize) { return {TokenType::EOFT, "", currentLine, currentColumn}; }

        const char currentChar = inputSpan[currentPosition];
        if(std::isalpha(currentChar)) {
            return extractIdentifier();
        } else if(std::isdigit(currentChar)) {
            return extractnumber();
        } else if(isOperator(currentChar)) {
            return extractOperator();
        } else if(std::isspace(currentChar)) {
            handleWhitespace(currentChar);
            return getNextToken();
        } else {
            // Handle unknown or invalid characters as an error
            handleError(std::string(1, currentChar), "Unknown Character");
            std::exit(-1);  // Terminate the program with an error code
        }
    }

    void handleError(const std::string &values, const std::string &errorMsg) {
        std::stringstream errorMessage;
        errorMessage << D_FORMAT("{} '{}' (line {}, column {}):\n", errorMsg, values, currentLine, currentColumn);

        // Add context information to the error message
        errorMessage << "Context:" << NEWL;

        std::size_t lineStart = currentPosition;
        std::size_t lineEnd = currentPosition;

        while(lineStart > 0 && inputSpan[lineStart - 1] != CNL) { lineStart--; }
        while(lineEnd < inputSize && inputSpan[lineEnd] != CNL) { lineEnd++; }
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
    std::span<const char> inputSpan;
    std::size_t currentPosition = 0;
    std::size_t inputSize = 0;
    std::size_t currentLine = 1;
    std::size_t currentColumn = 1;

    // Funzione per aggiungere un carattere al valore e incrementare posizione e colonna corrente
    void appendCharToValue(std::string &value, const char &character) {
        value += character;
        ++currentPosition;
        ++currentColumn;
    }

    [[nodiscard]] inline bool isOperator(char c) const noexcept {
        // Add logic to recognize specific operators
        // Example:
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    Token extractIdentifier() {
        std::string value;
        while(currentPosition < inputSize && (std::isalnum(inputSpan[currentPosition]) || inputSpan[currentPosition] == '_')) {
            appendCharToValue(value, inputSpan[currentPosition]);
        }
        return {TokenType::IDENTIFIER, value, currentLine, currentColumn - value.length()};
    }

    Token extractnumber() {
        std::string value;

        // Handle the digits before the decimal point
        while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) {
            appendCharToValue(value, inputSpan[currentPosition]);
        }

        // Check for a decimal point
        if(currentPosition < inputSize && inputSpan[currentPosition] == '.') {
            appendCharToValue(value, inputSpan[currentPosition]);

            // Handle digits after the decimal point (optional)
            while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) {
                appendCharToValue(value, inputSpan[currentPosition]);
            }

            // Check for an exponent (optional)
            if(currentPosition < inputSize && (std::toupper(inputSpan[currentPosition]) == 'E')) {
                appendCharToValue(value, inputSpan[currentPosition]);

                // Check for the sign of the exponent (optional)
                if(currentPosition < inputSize) {
                    auto c = inputSpan[currentPosition];
                    if(c == '+' || c == '-') { appendCharToValue(value, c); }
                }

                // Handle digits in the exponent (optional)
                while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) {
                    appendCharToValue(value, inputSpan[currentPosition]);
                }
            }
            return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
        }

        // Check for an exponent without a decimal point (e.g., 1e+1)
        if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == 'E') {
            appendCharToValue(value, inputSpan[currentPosition]);

            // Check for the sign of the exponent (optional)
            if(currentPosition < inputSize) {
                auto c = inputSpan[currentPosition];
                if(c == '+' || c == '-') { appendCharToValue(value, c); }
            }

            // Handle digits in the exponent (optional)
            while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) {
                appendCharToValue(value, inputSpan[currentPosition]);
            }
            return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
        }
        // If none of the above conditions match, it's a regular integer
        return {TokenType::INTEGER, value, currentLine, currentColumn - value.length()};
    }

    Token extractOperator() {
        auto value = input.substr(currentPosition, 1);
        currentPosition++;
        currentColumn++;
        return {TokenType::OPERATOR, value, currentLine, currentColumn - 1};
    }

    void handleWhitespace(char currentChar) noexcept {
        if(currentChar == CNL) {
            currentLine++;
            currentColumn = 1;
        } else {
            currentColumn++;
        }
        currentPosition++;
    }
};
