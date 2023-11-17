#pragma once
#include "headers.h"
#include <string>

enum class TokenType : int { IDENTIFIER, INTEGER, DOUBLE, OPERATOR, KEYWORD, EOFT, ERROR, UNKNOWN };

static inline constexpr TokenType eofTokenType = TokenType::EOFT;
struct Token {
    TokenType type{TokenType::UNKNOWN};
    std::string value;
    std::size_t line{};
    std::size_t column{};

    [[nodiscard]] inline std::string typeToString() const noexcept;
    [[nodiscard]] inline std::string toString() const;
    auto operator<=>(const Token &other) const = default;
};

std::string Token::typeToString() const noexcept {
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

std::string Token::toString() const {
    return D_FORMAT("type: {}, value: '{}', line {}, column {}", typeToString(), value, line, column);
}