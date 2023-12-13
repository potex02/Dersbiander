#include "TokenizerUtils.h"
#include <unordered_set>

bool TokenizerUtils::isPlusORMinus(char c) noexcept { return c == '+' || c == '-'; }

bool TokenizerUtils::isComment(const std::span<const char> &inputSpan, size_t position) noexcept {
    return position != inputSpan.size() && inputSpan[position] == '/' &&
           (inputSpan[position + 1] == '/' || inputSpan[position + 1] == '*');
}

bool TokenizerUtils::isOperator(char c) noexcept {
    static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-'};
    return operators.contains(c);
}

bool TokenizerUtils::isOperationEqualOperator(const std::string &value) noexcept {
    return value == "+=" || value == "-=" || value == "*=" || value == "/=";
}

bool TokenizerUtils::isBooleanOperator(const std::string &value) noexcept {
    return value == "==" || value == ">=" || value == "<=" || value == "!=";
}

bool TokenizerUtils::isLogicalOperator(const std::string &value) noexcept { return value == "&&" || value == "||"; }

bool TokenizerUtils::isVarLenOperator(const std::string &val) noexcept {
    return isOperator(val[0]) || isBrackets(val[0]) || isOperationEqualOperator(val) || isBooleanOperator(val) ||
           isLogicalOperator(val);
}

bool TokenizerUtils::isBrackets(char c) noexcept { return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}'; }

bool TokenizerUtils::isApostrophe(char c) noexcept { return c == '\''; }
