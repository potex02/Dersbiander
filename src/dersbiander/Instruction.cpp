#include "Instruction.h"

DISABLE_WARNINGS_PUSH(26461 26821)

Instruction::Instruction() noexcept
  : tokens({}), instructionTypes({InstructionType::BLANK}),
    allowedTokens({TokenType::KEYWORD_VAR, TokenType::KEYWORD_STRUCTURE, TokenType::IDENTIFIER, TokenType::OPEN_CURLY_BRACKETS,
                   TokenType::CLOSED_CURLY_BRACKETS, TokenType::EOFT}) {
    booleanOperatorPresent = {false};
    previousTokens.reserve(tokens.size());
}

[[nodiscard]] std::string Instruction::unexpected(const Token &token) const {
    std::string value;

    if(token.type != eofTokenType) {
        value = token.value;
    } else {
        value = "EOFT";
    }
    return D_FORMAT("Unexpected token: {} line {} column {}", value, token.line, token.column);
}

[[nodiscard]] std::vector<std::string> Instruction::typeToString() const noexcept {
    std::vector<std::string> result = {};

    for(const InstructionType &i : this->instructionTypes) {
        switch(i) {
            using enum InstructionType;
        case PROCEDURE_CALL:
            result.emplace_back("PROCEDURE_CALL");
            break;
        case OPERATION:
            result.emplace_back("OPERATION");
            break;
        case ASSIGNATION:
            result.emplace_back("ASSIGNATION");
            break;
        case EXPRESSION:
            result.emplace_back("EXPRESSION");
            break;
        case SQUARE_EXPRESSION:
            result.emplace_back("SQUARE_EXPRESSION");
            break;
        case DECLARATION:
            result.emplace_back("DECLARATION");
            break;
        case INITIALIZATION:
            result.emplace_back("INITIALIZATION");
            break;
        case ARRAY_INIZIALIZATION:
            result.emplace_back("ARRAY_INIZIALIZATION");
            break;
        case STRUCTURE:
            result.emplace_back("STRUCTURE");
            break;
        case DEFINITION:
            result.emplace_back("DEFINITION");
            break;
        case OPEN_SCOPE:
            result.emplace_back("OPEN_SCOPE");
            break;
        case CLOSE_SCOPE:
            result.emplace_back("OPEN_SCOPE");
            break;
        case BLANK:
            result.emplace_back("BLANK");
            break;
        default:
            result.emplace_back("UNKNOWN");
            break;
        }
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
    case CHAR:
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
    case OPEN_SQUARE_BRACKETS:
        this->checkOpenBrackets(token.type);
        break;
    case CLOSED_BRACKETS:
    case CLOSED_SQUARE_BRACKETS:
        this->checkClosedBrackets(token.type);
        break;
    case OPEN_CURLY_BRACKETS:
        this->checkOpenCurlyBrackets();
        break;
    case CLOSED_CURLY_BRACKETS:
        this->checkClosedCurlyBracktes();
        break;
    case KEYWORD_VAR:
        this->checkKeywordVar();
        break;
    case KEYWORD_STRUCTURE:
        this->checkKeywordStructure();
        break;
    case EOFT:
    case ERROR:
    case UNKNOWN:
    case COMMENT:
        break;
    }
    this->previousTokens.emplace_back(token);
    return {true, msg};
}

void Instruction::emplaceCommaEoft() noexcept {
    this->allowedTokens.emplace_back(TokenType::COMMA);
    this->allowedTokens.emplace_back(TokenType::EOFT);
}

bool Instruction::isExpression() {
    using enum InstructionType;
    return this->lastInstructionType() == ASSIGNATION || this->lastInstructionType() == INITIALIZATION ||
           this->lastInstructionType() == ARRAY_INIZIALIZATION || this->lastInstructionType() == EXPRESSION ||
           this->lastInstructionType() == SQUARE_EXPRESSION;
}

void Instruction::checkIdentifier() noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = { OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, OPEN_SQUARE_BRACKETS };
        this->emplaceBooleanOperator();
        if(this->emplaceTokenType(SQUARE_EXPRESSION, CLOSED_SQUARE_BRACKETS)) { return; }
        if(this->emplaceTokenType(EXPRESSION, CLOSED_BRACKETS)) { return; }
        if(this->lastInstructionType() == ARRAY_INIZIALIZATION) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
            return;
        }
        this->emplaceCommaEoft();
        return;
    }
    if(this->lastInstructionType() == BLANK || this->lastInstructionType() == OPERATION) {
        this->setLastInstructionType(OPERATION);
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, COMMA, OPEN_SQUARE_BRACKETS};
        return;
    }
    if(this->lastInstructionType() == DECLARATION) {
        if(!this->ispreviousEmpty() && this->previousTokensLast() == COLON) {
            this->allowedTokens = {EQUAL_OPERATOR, OPEN_SQUARE_BRACKETS, EOFT};
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
    if(this->isExpression()) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        this->emplaceBooleanOperator();
        if(this->emplaceTokenType(SQUARE_EXPRESSION, CLOSED_SQUARE_BRACKETS)) { return; }
        if(this->emplaceTokenType(EXPRESSION, CLOSED_BRACKETS)) { return; }
        if(this->lastInstructionType() == ARRAY_INIZIALIZATION) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
            return;
        }
        this->emplaceCommaEoft();
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, BOOLEAN, MINUS_OPERATOR, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkMinusOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, BOOLEAN, OPEN_BRACKETS};
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
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS, OPEN_SQUARE_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkBooleanAndLogicalOperator(const TokenType &type) {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
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
    this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS, OPEN_SQUARE_BRACKETS};
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

void Instruction::checkOpenBrackets(const TokenType &type) {
    using enum TokenType;
    using enum InstructionType;
    this->addBooleanOperatorPresent();
    this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
    if(type == OPEN_BRACKETS) {
        this->addInstructionType(EXPRESSION);
        this->allowedTokens.emplace_back(CLOSED_BRACKETS);
        return;
    }
    if(this->previousTokensLast() == EQUAL_OPERATOR || this->previousTokensLast() == COMMA || this->previousTokensLast() == OPEN_SQUARE_BRACKETS) {
        this->addInstructionType(ARRAY_INIZIALIZATION);
        this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS);
        this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
        return;
    }
    if(this->lastInstructionType() == DECLARATION) { this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS); }
    this->addInstructionType(SQUARE_EXPRESSION);
}

void Instruction::checkClosedBrackets(const TokenType &type) {
    using enum TokenType;
    using enum InstructionType;

    InstructionType last = this->lastInstructionType();

    this->removeInstructionType();
    this->removeBooleanOperatorPresent();
    if(this->isExpression()) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        if(type == CLOSED_SQUARE_BRACKETS && last != ARRAY_INIZIALIZATION ) { this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS); }
        this->emplaceBooleanOperator();
        if(this->emplaceTokenType(SQUARE_EXPRESSION, CLOSED_SQUARE_BRACKETS)) { return; }
        if(this->emplaceTokenType(EXPRESSION, CLOSED_BRACKETS)) { return; }
        if(this->lastInstructionType() == ARRAY_INIZIALIZATION) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
            return;
        }
        this->emplaceCommaEoft();
        return;
    }
    if (this->lastInstructionType() == OPERATION) {
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, COMMA};
        if(type == CLOSED_SQUARE_BRACKETS) { this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS); }
        return;
    }
    if(this->lastInstructionType() == DECLARATION) {
        this->allowedTokens = {EQUAL_OPERATOR, EOFT};
        if(type == CLOSED_SQUARE_BRACKETS) { this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS); }
        return;
    }
    if(this->lastInstructionType() == STRUCTURE) {
        this->allowedTokens = {OPEN_CURLY_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOpenCurlyBrackets() {
    using enum InstructionType;
    if(this->lastInstructionType() == BLANK) { this->setLastInstructionType(OPEN_SCOPE); }
    this->allowedTokens = {eofTokenType};
}

void Instruction::checkClosedCurlyBracktes() {
    using enum InstructionType;
    if(this->lastInstructionType() == BLANK) { this->setLastInstructionType(CLOSE_SCOPE); }
    this->allowedTokens = {eofTokenType};
}

void Instruction::checkKeywordVar() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == BLANK) {
        this->setLastInstructionType(DECLARATION);
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkKeywordStructure() {
    using enum TokenType;
    using enum InstructionType;
    if(this->lastInstructionType() == BLANK) {
        this->setLastInstructionType(STRUCTURE);
        this->allowedTokens = {OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

DISABLE_WARNINGS_POP()
