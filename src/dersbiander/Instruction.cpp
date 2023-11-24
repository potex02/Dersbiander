#include <vector>
#include "Instruction.h"

Instruction::Instruction(const std::vector<Token> &tokens) : tokens(tokens) {

    this->instructionType = InstructionType::UNKNOWN;

}

std::string Instruction::validate() {
    
    for(Token i : this->tokens) {

        if(!this->checkToken(i)) {

            return this->unexpected(i);;

        }
        this->previousTokens.push_back(i);
    }
    return "OK";
}

std::string Instruction::unexpected(const Token &token) {
    
    return "Unexpected token: " + token.value;

}

bool Instruction::checkToken(const Token &token) {

    switch(token.type) {
        using enum TokenType;
        case IDENTIFIER:
            return this->checkIdentifier();
            break;
        case INTEGER:
        case DOUBLE:
            return this->checkNumber();
            break;
        case OPERATOR:
            return this->checkOperator();
            break;
        /*case KEYWORD:
            break;*/
        case EOFT:
            return true;
            break;
        /*case ERROR:
            break;
        case UNKNOWN:
            break;*/

    }
    return false;
}

bool Instruction::checkIdentifier() {

    if(this->previousTokens.empty()) {

        this->instructionType = InstructionType::OPERATION;
        return true;
    }
    if(this->previousTokens.back().type == TokenType::OPERATOR) {

        return true;

    }
    return false;
}

bool Instruction::checkNumber() {

    if(this->previousTokens.empty() || this->previousTokens.back().type != TokenType::OPERATOR) {

        return false;

    }
    return true;

}

bool Instruction::checkOperator() {

    if(!this->previousTokens.empty() && (this->previousTokens.back().type == TokenType::IDENTIFIER || this->previousTokens.back().isNumber())) {
        
        return true;
        
    }
    return false;

}

bool Instruction::checkKeyWord() { return false; }
