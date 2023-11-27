#pragma once
#include "Token.h"
#include <vector>

enum class InstructionType : short { PROCEDURE_CALL, OPERATION, ASSIGNATION, CONDITION, DEFINITION, BLANK };

class Instruction {
public:
    explicit Instruction(const std::vector<Token> &tokens);
    std::string validate();
    [[nodiscard]] std::string typeToString() const noexcept;

private:
    std::vector<Token> tokens;
    std::vector<Token> previousTokens;
    InstructionType instructionType;
    std::string unexpected(const Token &token) const;
    [[nodiscard]] bool checkToken(const Token &token);
    [[nodiscard]] bool checkIdentifier() noexcept;
    [[nodiscard]] bool checkNumber() noexcept;
    [[nodiscard]] bool checkOperator(const Token &token);
    [[nodiscard]] inline bool checkKeyWord() const noexcept { return false; }
    [[nodiscard]] constexpr Token &previousTokensLast() noexcept { return this->previousTokens.back(); }
    [[nodiscard]] constexpr bool ispreviousEmpty() const noexcept { return this->previousTokens.empty(); }
};
