#include "Instruction.h"

DISABLE_WARNINGS_PUSH(26461 26821)

Instruction::Instruction(const std::vector<Token> &_tokens)
  : tokens(_tokens), instructionType(InstructionType::BLANK),
    allowedTokens({TokenType::KEYWORD, TokenType::IDENTIFIER, TokenType::EOFT}) {
    previousTokens.reserve(tokens.size());
}

std::string Instruction::validate() {
    for(const Token &inst : tokens) {
        if(!this->checkToken(inst)) { return this->unexpected(inst); }
        this->previousTokens.emplace_back(inst);
    }
    return D_FORMAT("OK: {}", this->typeToString());
}

[[nodiscard]] std::string Instruction::unexpected(const Token &token) const {
    return D_FORMAT("Unexpected token: {}", token.value);
}

[[nodiscard]] std::string Instruction::typeToString() const noexcept {
    switch(this->instructionType) {
        using enum InstructionType;
    case PROCEDURE_CALL:
        return "PROCEDURE_CALL";
    case OPERATION:
        return "OPERATION";
    case ASSIGNATION:
        return "ASSIGNATION";
    case CONDITION:
        return "CONDITION";
    case DECLARATION:
        return "DECLARATION";
    case DEFINITION:
        return "DEFINITION";
    case BLANK:
        return "BLANK";
    default:
        return "UNKNOWN";
    }
}

[[nodiscard]] bool Instruction::checkToken(const Token &token) {
    if(std::ranges::find(this->allowedTokens, token.type) == this->allowedTokens.end()) { return false; }
    switch(token.type) {
        using enum TokenType;
    case IDENTIFIER:
        this->checkIdentifier();
        break;
    case INTEGER:
    case DOUBLE:
        this->checkNumber();
        break;
    case OPERATOR:
        this->checkOperator();
        break;
    case MINUS_OPERATOR:
        this->checkMinusOperator();
        break;
    case EQUAL_OPERATOR:
        this->checkEqualOperator();
        break;
    case COMMA:
        this->checkComma();
        break;
    case COLON:
        this->checkColon();
        break;
    case KEYWORD:
        this->checkKeyword(token);
        break;
    case EOFT:
    case ERROR:
    case UNKNOWN:
        break;
    }
    return true;
}

void Instruction::checkIdentifier() noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(this->instructionType == BLANK) {
        this->instructionType = OPERATION;
        this->allowedTokens = {EQUAL_OPERATOR};
        return;
    }
    if(this->instructionType == DECLARATION) {
        if (!this->ispreviousEmpty() && this->previousTokensLast() == TokenType::COLON) {
            this->allowedTokens = {EOFT};
            return;
        }
        this->allowedTokens = {COMMA, COLON};
        return;
    }
    if(this->instructionType == ASSIGNATION) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, EOFT};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkNumber() noexcept {
    if(this->instructionType == InstructionType::ASSIGNATION) {
        this->allowedTokens = {TokenType::OPERATOR, TokenType::MINUS_OPERATOR, TokenType::EOFT};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOperator() {
    if(this->instructionType == InstructionType::ASSIGNATION) {
        this->allowedTokens = {TokenType::IDENTIFIER, TokenType::INTEGER, TokenType::DOUBLE, TokenType::MINUS_OPERATOR};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkMinusOperator() {
    if(this->instructionType == InstructionType::ASSIGNATION) {
        this->allowedTokens = {TokenType::IDENTIFIER, TokenType::INTEGER, TokenType::DOUBLE};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkEqualOperator() {
    if(this->instructionType == InstructionType::OPERATION) {
        this->instructionType = InstructionType::ASSIGNATION;
        this->allowedTokens = {TokenType::IDENTIFIER, TokenType::INTEGER, TokenType::DOUBLE, TokenType::MINUS_OPERATOR};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkComma() {
    if(this->instructionType == InstructionType::DECLARATION) {
        this->allowedTokens = {TokenType::IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkColon() {
    if(this->instructionType == InstructionType::DECLARATION) {
        this->allowedTokens = {TokenType::IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkKeyword(const Token &token) {
    if(token.value == "var" && this->instructionType == InstructionType::BLANK) {
        this->instructionType = InstructionType::DECLARATION;
        this->allowedTokens = {TokenType::IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

DISABLE_WARNINGS_POP()
