#include "Instruction.h"

Instruction::Instruction(const std::vector<Token> &tokens) : tokens(tokens), instructionType(InstructionType::BLANK) {
    previousTokens.reserve(tokens.size());
}

std::string Instruction::validate() {
    for(const Token &inst : this->tokens) {
        if(!this->checkToken(inst)) { return this->unexpected(inst); }
        this->previousTokens.emplace_back(inst);
    }
    return D_FORMAT("OK: {}", this->typeToString());
}

std::string Instruction::unexpected(const Token &token) const { return D_FORMAT("Unexpected token: {}", token.value); }

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
    case DEFINITION:
        return "DEFINITION";
    case BLANK:
        return "BLANK";
    default:
        return "UNKNOWN";
    }
}

[[nodiscard]] bool Instruction::checkToken(const Token &token) {
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

[[nodiscard]] bool Instruction::checkIdentifier() noexcept {
    if(ispreviousEmpty()) {
        this->instructionType = InstructionType::OPERATION;
        return true;
    }
    if(previousTokensLast().type == TokenType::OPERATOR) { return true; }
    return false;
}

[[nodiscard]] bool Instruction::checkNumber() noexcept {
    if(ispreviousEmpty() || this->instructionType == InstructionType::OPERATION ||
       previousTokensLast().type != TokenType::OPERATOR) {
        return false;
    }
    return true;
}

[[nodiscard]] bool Instruction::checkOperator(const Token &token) {
    using enum InstructionType;
    if(ispreviousEmpty()) { return false; }

    if(token.value == "=") {
        if(this->instructionType == OPERATION && this->previousTokens.size() == 1 &&
           previousTokensLast().type == TokenType::IDENTIFIER) {
            this->instructionType = ASSIGNATION;
            return true;
        }
        return false;
    }
    if(this->instructionType == ASSIGNATION &&
       (token.value == "-" || (previousTokensLast().type == TokenType::IDENTIFIER || previousTokensLast().isNumber()))) {
        return true;
    }
    return false;
}
