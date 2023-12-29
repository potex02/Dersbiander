#pragma once

#include "Timer.h"
#include "Token.h"
#include "TokenizerUtils.h"
#include <span>
#include <string>
#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(const std::string &_input);
    ~Tokenizer() = default;

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
    [[nodiscard]] bool isPositionInText() const noexcept;

    // NOLINTBEGIN
    [[nodiscard]] constexpr TokenType typeBySingleCharacter(char c) const;
    TokenType typeByValue(const std::string &value) const;
    // NOLINTEND
    Token extractIdentifier();
    void kewordType(const std::string &value, TokenType &type) const;
    void extractDigits(std::string &value);
    void extractExponent(std::string &value);
    void extractVarLenOperator(std::string &value);
    Token extractnumber();
    void extractOperator(std::vector<Token> &tokens);
    Token extractBrackets(char c);
    Token extractChar();
    Token extractString();
    Token extractComment();
    std::string handleWithSingleLineComment();
    std::pair<bool, std::string> handleWithMultilineComment();
    void handleWhitespace(char currentChar) noexcept;
    [[nodiscard]] std::size_t findLineStart() const;
    [[nodiscard]] std::size_t findLineEnd() const;
    [[nodiscard]] std::string getContextLine(size_t lineStart, size_t lineEnd) const;
    [[nodiscard]] std::string getHighlighting(size_t lineStart, size_t length) const;
    [[nodiscard]] std::string getErrorMessage(const std::string &values, const std::string &errMsg,
                                              const std::string &contextLine, const std::string &highlighting) const;
    void incPosAndCol();
    [[nodiscard]] bool isEscapedChar(const std::string &val) const noexcept;
};
