#pragma once
#include "Token.h"
#include <vector>

enum class InstructionType : short { PROCEDURE_CALL, OPERATION, ASSIGNATION, CONDITION, DECLARATION, DEFINITION, BLANK };

class Instruction {
public:
    explicit Instruction(const std::vector<Token> &tokens);
    std::string validate();
    [[nodiscard]] std::string typeToString() const noexcept;

private:
    std::vector<Token> tokens;
    std::vector<Token> previousTokens;
    InstructionType instructionType;
    std::vector<TokenType> allowedTokens;
    [[nodiscard]] std::string unexpected(const Token &token) const;
    [[nodiscard]] bool checkToken(const Token &token);
    void checkIdentifier() noexcept;
    void checkNumber() noexcept;
    void checkOperator();
    void checkMinusOperator();
    void checkEqualOperator();
    void checkKeyword(const Token &token);
};
