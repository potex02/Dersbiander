#include "Instruction.h"

DISABLE_WARNINGS_PUSH(26461 26821)

Instruction::Instruction() noexcept
  : tokens({}), instructionTypes({InstructionType::BLANK}),
    allowedTokens({TokenType::KEYWORD_VAR, TokenType::IDENTIFIER, TokenType::EOFT}) {
    booleanOperatorPresent = {false};
    previousTokens.reserve(tokens.size());
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
    std::string result = "";

    for(const InstructionType &i : this->instructionTypes) {
        switch(i) {
            using enum InstructionType;
        case PROCEDURE_CALL:
            result += "PROCEDURE_CALL";
            break;
        case OPERATION:
            result += "OPERATION";
            break;
        case ASSIGNATION:
            result += "ASSIGNATION";
            break;
        case EXPRESSION:
            return "EXPRESSION";
        case CONDITION:
            result += "CONDITION";
            break;
        case DECLARATION:
            result += "DECLARATION";
            break;
        case INITIALIZATION:
            result += "INITIALIZATION";
            break;
        case DEFINITION:
            result += "DEFINITION";
            break;
        case BLANK:
            result += "BLANK";
            break;
        default:
            result += "UNKNOWN";
            break;
        }
        result += "\n";
    }
    return result;
}

[[nodiscard]] std::pair<bool, std::string> Instruction::checkToken(const Token &token) {
    std::string msg = token.toString();
    if(std::ranges::find(this->allowedTokens, token.type) == this->allowedTokens.end()) { return {false, msg}; }
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
    case OPEN_BRACKETS:
        this->checkOpenBrackets();
        break;
    case CLOSED_BRACKETS:
        this->checkClosedBrackets();
        break;
    case KEYWORD_VAR:
        this->checkKeywordVar();
        break;
    case EOFT:
        break;
    case ERROR:
    case UNKNOWN:
        break;
    }
    this->previousTokens.emplace_back(token);
    return {true, msg};
}

void Instruction::checkIdentifier() noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION ||
       this->lastInstructionType() == EXPRESSION) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        if(!this->lastBooleanOperatorPresent()) { this->allowedTokens.emplace_back(BOOLEAN_OPERATOR); }
        if(this->lastInstructionType() == EXPRESSION) { this->allowedTokens.emplace_back(CLOSED_BRACKETS); }
        if(this->lastInstructionType() != EXPRESSION) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(EOFT);
        }
        return;
    }
    if(this->lastInstructionType() == BLANK || this->lastInstructionType() == OPERATION) {
        this->setLastInstructionType(OPERATION);
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, COMMA};
        return;
    }
    if(this->lastInstructionType() == DECLARATION) {
        if(!this->ispreviousEmpty() && this->previousTokensLast() == COLON) {
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
    if(this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION ||
       this->lastInstructionType() == EXPRESSION) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        if(!this->lastBooleanOperatorPresent()) { this->allowedTokens.emplace_back(BOOLEAN_OPERATOR); }
        if(this->lastInstructionType() == EXPRESSION) { this->allowedTokens.emplace_back(CLOSED_BRACKETS); }
        if(this->lastInstructionType() != EXPRESSION) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(EOFT);
        }
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION ||
       this->lastInstructionType() == EXPRESSION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkMinusOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION ||
       this->lastInstructionType() == EXPRESSION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkEqualOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == OPERATION || this->lastInstructionType() == DECLARATION) {
        if(this->lastInstructionType() == OPERATION) {
            this->setLastInstructionType(ASSIGNATION);

        } else {
            this->setLastInstructionType(INITIALIZATION);
        }
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkBooleanAndLogicalOperator(TokenType type) {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION ||
       this->lastInstructionType() == EXPRESSION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
        if(type != NOT_OPERATOR) {
            this->allowedTokens.emplace_back(NOT_OPERATOR);
            this->setLastBooleanOperatorPresent(type == BOOLEAN_OPERATOR);
        }
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkComma() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == OPERATION || this->lastInstructionType() == DECLARATION) {
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    if(this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkColon() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == DECLARATION) {
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOpenBrackets() {
    using enum TokenType;
    using enum InstructionType;
    this->addInstructionType(EXPRESSION);
    this->addBooleanOperatorPresent();
    this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS, CLOSED_BRACKETS};
}

void Instruction::checkClosedBrackets() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == EXPRESSION) {
        this->removeInstructionType();
        this->removeBooleanOperatorPresent();
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        if(this->lastInstructionType() == EXPRESSION) {
            this->allowedTokens.emplace_back(CLOSED_BRACKETS);
        } else {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(EOFT);
        }
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkKeywordVar() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == BLANK) {
        this->lastInstructionType() = DECLARATION;
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

DISABLE_WARNINGS_POP()
