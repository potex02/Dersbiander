#pragma once
#include "headers.h"
#include <string>

static const std::array<std::string, 1> KEYWORDS = {"var"};

enum class TokenType : int { IDENTIFIER, INTEGER, DOUBLE, OPERATOR, KEYWORD, EOFT, ERROR, UNKNOWN };

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
    [[nodiscard]] inline bool isNumber() const noexcept {
        return this->type == TokenType::INTEGER || this->type == TokenType::DOUBLE;
    }
    auto operator<=>(const Token &other) const = default;
};
