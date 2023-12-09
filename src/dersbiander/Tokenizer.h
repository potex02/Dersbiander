#pragma once

#include "Timer.h"
#include "Token.h"
#include <span>
#include <string>
#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(const std::string &_input);

    // delete copy constructor and copy assignment operator
    Tokenizer(const Tokenizer &) = delete;
    Tokenizer(Tokenizer &&) = delete;
    Tokenizer &operator=(const Tokenizer &) = delete;
    Tokenizer &operator=(Tokenizer &&) = delete;

    std::vector<Token> tokenize();

    void handleError(const std::string &values, const std::string &errorMsg);

private:
    std::string input;
    std::span<const char> inputSpan;
    std::size_t currentPosition = 0;
    std::size_t inputSize = 0;
    std::size_t currentLine = 1;
    std::size_t currentColumn = 1;

    inline void appendCharToValue(std::string &value);

    // NOLINTBEGIN
    [[nodiscard]] inline bool isPlusORMinus(char c) const noexcept;
    [[nodiscard]] inline bool isComment(size_t position) const noexcept;
    [[nodiscard]] inline bool isOperator(char c) const noexcept;
    [[nodiscard]] inline bool isOperationEqualOperator(const std::string &value) const noexcept;
    [[nodiscard]] inline bool isBooleanOperator(const std::string &value) const noexcept;
    [[nodiscard]] inline bool isLogicalOperator(const std::string &value) const noexcept;
    [[nodiscard]] inline bool isVarLenOperator(const std::string &val) const noexcept;
    [[nodiscard]] inline bool isBrackets(char c) const noexcept;
    [[nodiscard]] inline bool isApostrophe(char c) const noexcept;
    TokenType typeBySingleCharacter(char c) const;
    TokenType typeByValue(const std::string &value) const;
    // NOLINTEND
    Token extractIdentifier();
    void extractDigits(std::string &value);
    void extractExponent(std::string &value);
    void extractVarLenOperator(std::string &value);
    Token extractnumber();
    Token extractOperator();
    Token extractBrackets(char c);
    Token extractChar();
    Token extractComment();
    std::string handleWithSingleLineComment();
    std::pair<bool, std::string> handleWithMultilineComment();
    void handleWhitespace(char currentChar) noexcept;
    [[nodiscard]] std::size_t findLineStart();
    [[nodiscard]] std::size_t findLineEnd();
    [[nodiscard]] std::string getContextLine(size_t lineStart, size_t lineEnd) const;
    [[nodiscard]] std::string getHighlighting(size_t lineStart, size_t length) const;
    [[nodiscard]] std::string getErrorMessage(const std::string &values, const std::string &errMsg,
                                              const std::string &contextLine, const std::string &highlighting) const;
    void incPosAndCol();
};
