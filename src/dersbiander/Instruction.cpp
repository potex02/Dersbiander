#include "Instruction.h"

Instruction::Instruction(const std::vector<Token> &tokens) : tokens(tokens), instructionType(InstructionType::BLANK) {}

std::string Instruction::validate() {
    for(const Token &i : this->tokens) {
        if(!this->checkToken(i)) { return this->unexpected(i); }
        this->previousTokens.emplace_back(i);
    }
    return D_FORMAT("OK: {}", this->typeToString());
}

std::string Instruction::unexpected(const Token &token) const { return D_FORMAT("Unexpected token:: {}", token.value); }

std::string Instruction::typeToString() const noexcept {
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
    case DEFINITION:
        return "DEFINITION";
    case BLANK:
        return "BLANK";
    default:
        return "UNKNOWN";
    }
}

bool Instruction::checkToken(const Token &token) {
    switch(token.type) {
        using enum TokenType;
    case IDENTIFIER:
        return this->checkIdentifier();
    case INTEGER:
    case DOUBLE:
        return this->checkNumber();
    case OPERATOR:
        return this->checkOperator(token);
    /*case KEYWORD:*/
    case EOFT:
        return true;
        /*case ERROR:
        case UNKNOWN:*/
    }
    return false;
}

bool Instruction::checkIdentifier() noexcept {
    if(this->previousTokens.empty()) {
        this->instructionType = InstructionType::OPERATION;
        return true;
    }
    if(this->previousTokens.back().type == TokenType::OPERATOR) { return true; }
    return false;
}

bool Instruction::checkNumber() noexcept {
    if(this->previousTokens.empty() || this->instructionType == InstructionType::OPERATION ||
       this->previousTokens.back().type != TokenType::OPERATOR) {
        return false;
    }
    return true;
}

bool Instruction::checkOperator(const Token &token) {
    using enum InstructionType;
    if(this->previousTokens.empty()) { return false; }

    if(token.value == "=") {
        if(this->instructionType == OPERATION && this->previousTokens.size() == 1 &&
           this->previousTokens.back().type == TokenType::IDENTIFIER) {
            this->instructionType = ASSIGNATION;
            return true;
        }
        return false;
    }
    if(this->instructionType == ASSIGNATION &&
       (token.value == "-" ||
        (this->previousTokens.back().type == TokenType::IDENTIFIER || this->previousTokens.back().isNumber()))) {
        return true;
    }
    return false;
}

bool Instruction::checkKeyWord() const noexcept { return false; }