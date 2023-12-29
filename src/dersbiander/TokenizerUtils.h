#pragma once
#include "headers.h"
#include <span>

class TokenizerUtils {
public:
    [[nodiscard]] static bool isPlusORMinus(char c) noexcept;
    [[nodiscard]] static bool isComment(const std::span<const char> &inputSpan, size_t position) noexcept;
    [[nodiscard]] static bool isOperator(char c) noexcept;
    [[nodiscard]] static bool isOperationEqualOperator(const std::string &value) noexcept;
    [[nodiscard]] static bool isBooleanOperator(const std::string &value) noexcept;
    [[nodiscard]] static bool isLogicalOperator(const std::string &value) noexcept;
    [[nodiscard]] static bool isUnaryOperator(const std::string &value) noexcept;
    [[nodiscard]] static bool isVarLenOperator(const std::string &val) noexcept;
    [[nodiscard]] static bool isBrackets(char c) noexcept;
    [[nodiscard]] static bool isApostrophe(char c) noexcept;
    [[nodiscard]] static bool isQuotation(char c) noexcept;
    [[nodiscard]] static bool inCNL(char c) noexcept;
};
