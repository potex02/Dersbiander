#include "Instruction.h"
#include <vector>

Instruction::Instruction(const std::vector<Token> &tokens) : tokens(tokens) { this->instructionType = InstructionType::UNKNOWN; }

bool Instruction::validate() {
    for(Token i : this->tokens) {
        if(!this->checkToken(i)) { return false; }
        this->previousTokens.push_back(i);
    }
    return true;
}

bool Instruction::checkToken(const Token &token) {
    switch(token.type) {
        using enum TokenType;
    case IDENTIFIER:
        return this->checkIdentifier(token);
        /* case INTEGER:
            this->checkIdentifier(token);
        case DOUBLE:
            this->checkIdentifier(token);
        case OPERATOR:
            this->checkIdentifier(token);
        case KEYWORD:
            this->checkIdentifier(token);
        case EOFT:
            this->checkIdentifier(token);
        case ERROR:
            this->checkIdentifier(token);
        case UNKNOWN:
            this->checkIdentifier(token);*/
    }
    return false;
}

bool Instruction::checkIdentifier(const Token &token) {
    if(this->previousTokens.empty()) {
        this->instructionType = InstructionType::OPERATION;
        LINFO("{}", token.typeToString());
        return true;
    }

    return false;
}
