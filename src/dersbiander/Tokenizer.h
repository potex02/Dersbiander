#include "Timer.h"
#include "TokentizeErr.h"
#include "headers.h"
#include <compare>

enum class TokenType : int { IDENTIFIER, INTEGER, DOUBLE, OPERATOR, KEYWORD, EOFT, ERROR, UNKNOWN };

/* inline static const std::unordered_map<const char, TokenType> charToTokenTypeMap{
    {'+', TokenType::OPERATOR}, {'-', TokenType::OPERATOR}, {'*', TokenType::OPERATOR}, {'/', TokenType::OPERATOR},
    {'=', TokenType::OPERATOR}, {'(', TokenType::OPERATOR}, {')', TokenType::OPERATOR},
    // Add more mappings as needed
};*/

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
        return D_FORMAT("type: {}, value: '{}', line {}, column {}", typeToString(), value, line, column);
    }

    auto operator<=>(const Token &other) const = default;
};

static inline constexpr TokenType eofTokenType = TokenType::EOFT;

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input)
      : input(input), inputSpan(input.c_str(), input.size()), inputSize(input.size()) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        // while((token = getNextToken()).type != eofTokenType) { tokens.emplace_back(token); }
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
                // Continue the loop to get the next token
                continue;
            } else {
                // Handle unknown or invalid characters as an error
                handleError(std::string(1, currentChar), "Unknown Character");
                std::exit(-1);  // Terminate the program with an error code
            }
        }

        // Return an EOFT token when the input is fully processed
        tokens.emplace_back(TokenType::EOFT, "", currentLine, currentColumn - 1);

        return tokens;
    }

    void handleError(const std::string &values, const std::string &errorMsg) {
        std::stringstream errorMessage;
        errorMessage << D_FORMAT("{} '{}' (line {}, column {}):\n", errorMsg, values, currentLine, currentColumn);

        // Add context information to the error message
        errorMessage << "Context:" << NEWL;

        std::size_t lineStart = currentPosition;
        std::size_t lineEnd = currentPosition;

        while(lineStart > 0 && inputSpan[lineStart - 1] != CNL) { --lineStart; }
        while(lineEnd < inputSize && inputSpan[lineEnd] != CNL) { ++lineEnd; }
        errorMessage << input.substr(lineStart, lineEnd - lineStart) << NEWL;

        // Include a marker pointing to the position of the error
        errorMessage << std::string(currentPosition - lineStart, ' ') << std::string(values.length(), '^') << NEWL;

        LERROR(errorMessage.str());
    }

private:
    std::string input;
    std::span<const char> inputSpan;
    std::size_t currentPosition = 0;
    std::size_t inputSize = 0;
    std::size_t currentLine = 1;
    std::size_t currentColumn = 1;

    // Funzione per aggiungere un carattere al valore e incrementare posizione e colonna corrente
    inline void appendCharToValue(std::string &value, char character) {
        value += character;
        ++currentPosition;
        ++currentColumn;
    }
    [[nodiscard]] inline bool isPlusORMinus(char c) const noexcept { return (c == '+' || c == '-'); }

    [[nodiscard]] inline bool isOperator(char c) const noexcept {
        // Add logic to recognize specific operators
        // Example:
        return (isPlusORMinus(c) || c == '*' || c == '/');
    }

    Token extractIdentifier() {
        std::string value;
        while(currentPosition < inputSize && (std::isalnum(inputSpan[currentPosition]) || inputSpan[currentPosition] == '_')) {
            appendCharToValue(value, inputSpan[currentPosition]);
        }
        return {TokenType::IDENTIFIER, value, currentLine, currentColumn - value.length()};
    }

    inline void extractDigits(std::string &value) {
        while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) {
            appendCharToValue(value, inputSpan[currentPosition]);
        }
    }

    inline void extractExponent(std::string &value) {
        if(currentPosition < inputSize) {
            auto c = inputSpan[currentPosition];
            if(isPlusORMinus(c)) { appendCharToValue(value, c); }
        }
        extractDigits(value);
    }

    Token extractnumber() {
        std::string value;
        extractDigits(value);

        if(currentPosition < inputSize && inputSpan[currentPosition] == PNT) {
            appendCharToValue(value, inputSpan[currentPosition]);
            extractDigits(value);

            if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == ECR) {
                appendCharToValue(value, inputSpan[currentPosition]);
                extractExponent(value);
            }
            return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
        }

        if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == ECR) {
            appendCharToValue(value, inputSpan[currentPosition]);
            extractExponent(value);
            return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
        }
        return {TokenType::INTEGER, value, currentLine, currentColumn - value.length()};
    }

    Token extractOperator() {
        auto value = input.substr(currentPosition, 1);
        ++currentPosition;
        ++currentColumn;
        return {TokenType::OPERATOR, value, currentLine, currentColumn - 1};
    }

    void handleWhitespace(char currentChar) noexcept {
        if(currentChar == CNL) {
            ++currentLine;
            currentColumn = 1;
        } else {
            ++currentColumn;
        }
        ++currentPosition;
    }
};
