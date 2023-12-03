#pragma once

#include "headers.h"
#include <string>

static const std::array<const std::string, 1> KEYWORDS = {"var"};

enum class TokenType : int {
    IDENTIFIER,
    INTEGER,
    BOOLEAN,
    DOUBLE,
    OPERATOR,
    MINUS_OPERATOR,
    EQUAL_OPERATOR,
    INCREMENT_EQUAL,
    DECREMENT_EQUAL,
    MULTIPLY_EQUAL,
    DIVIDE_EQUAL,
    BOOLEAN_OPERATOR,
    COMMA,
    COLON,
    KEYWORD,
    EOFT,
    ERROR,
    UNKNOWN

};

static inline constexpr TokenType eofTokenType = TokenType::EOFT;

struct Token {
    TokenType type{TokenType::UNKNOWN};
    std::string value;
    std::size_t line{};
    std::size_t column{};

    [[nodiscard]] std::string typeToString() const noexcept {
        switch(type) {
            using enum TokenType;
        case IDENTIFIER:
            return "IDENTIFIER";
        case BOOLEAN:
            return "BOOLEAN";
        case INTEGER:
            return "INTEGER";
        case DOUBLE:
            return "DOUBLE";
        case OPERATOR:
            return "OPERATOR";
        case MINUS_OPERATOR:
            return "MINUS_OPERATOR";
        case EQUAL_OPERATOR:
            return "EQUAL_OPERATOR";
        case INCREMENT_EQUAL:
            return "INCREMENT_EQUAL";
        case DECREMENT_EQUAL:
            return "DECREMENT_EQUAL";
        case MULTIPLY_EQUAL:
            return "MULTIPLY_EQUAL";
        case DIVIDE_EQUAL:
            return "DIVIDE_OPERATOR";
        case BOOLEAN_OPERATOR:
            return "BOOLEAN_OPERATOR";
        case COMMA:
            return "COMMA";
        case COLON:
            return "COLON";
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

    [[nodiscard]] inline bool isNumber() const noexcept {
        return this->type == TokenType::INTEGER || this->type == TokenType::DOUBLE;
    }

    auto operator<=>(const Token &other) const = default;
};
