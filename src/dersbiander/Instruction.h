#pragma once
#include "Token.h"
#include <vector>

enum class InstructionType : short {

    PROCEDURE_CALL,
    OPERATION,
    ASSIGNATION,
    CONDITION,
    DEFINITION,
    BLANK

};

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
    bool checkToken(const Token &token);
    bool checkIdentifier();
    bool checkNumber();
    bool checkOperator(const Token &token);
    bool checkKeyWord() const;
};
