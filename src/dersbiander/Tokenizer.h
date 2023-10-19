#include "headers.h"

enum class TokenType : long { IDENTIFIER, INTEGER, OPERATOR, KEYWORD, EOFT };

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string &input) : input(input) {}

    Token getNextToken() {
        if(currentPosition >= inputSize) { return {TokenType::EOFT, ""}; }

        char currentChar = input[currentPosition];
        if(std::isalpha(currentChar)) {
            return extractIdentifier();
        } else if(std::isdigit(currentChar)) {
            return extractInteger();
        } else if(isOperator(currentChar)) {
            return extractOperator();
        } else if(std::isspace(currentChar)) {
            // Ignora spazi vuoti
            currentPosition++;
            return getNextToken();
        } else {
            // Gestisci caratteri non validi o sconosciuti
            currentPosition++;
            return {TokenType::EOFT, ""};
        }
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token;
        while((token = getNextToken()).type != TokenType::EOFT) { tokens.emplace_back(token); }
        return tokens;
    }

private:
    std::string input;
    std::size_t currentPosition = 0;
    std::size_t inputSize = input.size();

    [[nodiscard]] inline bool isOperator(char c) const {
        // Aggiungi logica per riconoscere operatori specifici
        // Esempio:
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    Token extractIdentifier() {
        std::string value;
        while(currentPosition < inputSize && (std::isalnum(input[currentPosition]) || input[currentPosition] == '_')) {
            value += input[currentPosition];
            currentPosition++;
        }
        return {TokenType::IDENTIFIER, value};
    }

    Token extractInteger() {
        std::string value;
        while(currentPosition < inputSize && std::isdigit(input[currentPosition])) {
            value += input[currentPosition];
            currentPosition++;
        }
        return {TokenType::INTEGER, value};
    }

    Token extractOperator() {
        auto value = input.substr(currentPosition, 1);
        currentPosition++;
        return {TokenType::OPERATOR, value};
    }
};
