#include "Instruction.h"

DISABLE_WARNINGS_PUSH(26461 26821)

Instruction::Instruction(const std::vector<Token> &_tokens)
  : tokens(_tokens), instructionType(InstructionType::BLANK),
    allowedTokens({TokenType::KEYWORD, TokenType::IDENTIFIER, TokenType::EOFT}) {
    booleanOperatorPresent = false;
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
    std::string value;

    if(token.type != TokenType::EOFT) {
        value = token.value;
    } else {
        value = "EOFT";
    }
    return D_FORMAT("Unexpected token: {} line {} column {}", value, token.line, token.column);
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
    case INITIALIZATION:
        return "INITIALIZATION";
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
    case BOOLEAN:
        this->checkNumber();
        break;
    case OPERATOR:
        this->checkOperator();
        break;
    case MINUS_OPERATOR:
        this->checkMinusOperator();
        break;
    case EQUAL_OPERATOR:
    case OPERATION_EQUAL:
        this->checkEqualOperator();
        break;
    case BOOLEAN_OPERATOR:
    case NOT_OPERATOR:
    case LOGICAL_OPERATOR:
        this->checkBooleanAndLogicalOperator(token.type);
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
    if(this->instructionType == ASSIGNATION || this->instructionType == INITIALIZATION) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, EOFT};
        if (!this->booleanOperatorPresent) {
            this->allowedTokens.emplace_back(BOOLEAN_OPERATOR);
        }
        if(this->instructionType == INITIALIZATION) { this->allowedTokens.emplace_back(COMMA); }
        return;
    }
    if(this->instructionType == BLANK) {
        this->instructionType = OPERATION;
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL};
        return;
    }
    if(this->instructionType == DECLARATION) {
        if(!this->ispreviousEmpty() && this->previousTokensLast() == TokenType::COLON) {
            this->allowedTokens = {EQUAL_OPERATOR, EOFT};
            return;
        }
        this->allowedTokens = {COMMA, COLON};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkNumber() noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(this->instructionType == ASSIGNATION || this->instructionType == INITIALIZATION) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, EOFT};
        if(!this->booleanOperatorPresent) {
            this->allowedTokens.emplace_back(BOOLEAN_OPERATOR);
        }
        if(this->instructionType == INITIALIZATION) { this->allowedTokens.emplace_back(COMMA); }
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->instructionType == ASSIGNATION || this->instructionType == INITIALIZATION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkMinusOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->instructionType == ASSIGNATION || this->instructionType == INITIALIZATION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkEqualOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->instructionType == OPERATION || this->instructionType == DECLARATION) {
        if(this->instructionType == OPERATION) {
            this->instructionType = ASSIGNATION;
        } else {
            this->instructionType = INITIALIZATION;
        }
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkBooleanAndLogicalOperator(TokenType type) {
    using enum TokenType;
    using enum InstructionType;
    this->booleanOperatorPresent = true;
    if(this->instructionType == ASSIGNATION || this->instructionType == INITIALIZATION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR};
        if(type != NOT_OPERATOR) {
            this->allowedTokens.emplace_back(NOT_OPERATOR);
            this->booleanOperatorPresent = type == BOOLEAN_OPERATOR;
        }
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkComma() {
    using enum TokenType;
    using enum InstructionType;
    if(this->instructionType == DECLARATION) {
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    if(this->instructionType == INITIALIZATION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR};
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
