#pragma once

#include "headers.h"
#include <string>

enum class TokenType : int {
    IDENTIFIER,
    INTEGER,
    BOOLEAN,
    DOUBLE,
    OPERATOR,
    MINUS_OPERATOR,
    EQUAL_OPERATOR,
    OPERATION_EQUAL,
    BOOLEAN_OPERATOR,
    NOT_OPERATOR,
    LOGICAL_OPERATOR,
    COMMA,
    COLON,
    OPEN_BRACKETS,
    CLOSED_BRACKETS,
    KEYWORD_VAR,
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
        case OPERATION_EQUAL:
            return "OPERATION_EQUAL";
        case BOOLEAN_OPERATOR:
            return "BOOLEAN_OPERATOR";
        case NOT_OPERATOR:
            return "NOT_OPERATOR";
        case LOGICAL_OPERATOR:
            return "LOGICAL_OPERATOR";
        case COMMA:
            return "COMMA";
        case COLON:
            return "COLON";
        case OPEN_BRACKETS:
            return "OPEN_BRACKETS";
        case CLOSED_BRACKETS:
            return "CLOSED_BRACKETS";
        case KEYWORD_VAR:
            return "KEYWORD_VAR";
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
