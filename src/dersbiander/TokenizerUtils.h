#pragma once
#include <span>
#include <string>
#include <vector>

class TokenizerUtils {
public:
    static bool isPlusORMinus(char c) noexcept;
    static bool isComment(const std::span<const char> &inputSpan, size_t position) noexcept;
    static bool isOperator(char c) noexcept;
    static bool isOperationEqualOperator(const std::string &value) noexcept;
    static bool isBooleanOperator(const std::string &value) noexcept;
    static bool isLogicalOperator(const std::string &value) noexcept;
    static bool isUnaryOperator(const std::string &value) noexcept;
    static bool isVarLenOperator(const std::string &val) noexcept;
    static bool isBrackets(char c) noexcept;
    static bool isApostrophe(char c) noexcept;
};
