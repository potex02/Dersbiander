#include "Instruction.h"

#define ONLY_TOKEN_TYPE

DISABLE_WARNINGS_PUSH(26461 26821)

Instruction::Instruction() noexcept
  : tokens({}), instructionTypes({InstructionType::BLANK}),
    allowedTokens({TokenType::KEYWORD_VAR, TokenType::KEYWORD_STRUCTURE, TokenType::KEYWORD_FOR, TokenType::IDENTIFIER,
                   TokenType::OPEN_CURLY_BRACKETS, TokenType::CLOSED_CURLY_BRACKETS, TokenType::EOFT}) {
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
    std::vector<std::string> result;
    result.reserve(this->instructionTypes.size());

    // NOLINTNEXTLINE
    for(const InstructionType &i : this->instructionTypes) {
        switch(i) {
            using enum InstructionType;
        case PROCEDURE_CALL:
            result.emplace_back("PROCEDURE_CALL");
            break;
        case PARAMETER_EXPRESSION:
            result.emplace_back("PARAMETER_EXPRESSION");
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
        case FOR_STRUCTURE:
            result.emplace_back("FOR_STRUCTURE");
            break;
        case FOR_INITIALIZATION:
            result.emplace_back("FOR_INITIALIZATION");
            break;
        case FOR_CONDITION:
            result.emplace_back("FOR_CONDITION");
            break;
        case FOR_STEP:
            result.emplace_back("FOR_STEP");
            break;
        case DEFINITION:
            result.emplace_back("DEFINITION");
            break;
        case OPEN_SCOPE:
            result.emplace_back("OPEN_SCOPE");
            break;
        case CLOSE_SCOPE:
            result.emplace_back("CLOSE_SCOPE");
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
#ifdef ONLY_TOKEN_TYPE
    std::string msg = token.typeToString();
#else
    std::string msg = token.toString();
#endif  // ONLY_TOKEN_TYPE
    if(std::ranges::find(this->allowedTokens, token.type) == this->allowedTokens.end()) [[unlikely]] { return {false, msg}; }
    switch(token.type) {
        using enum TokenType;
    case IDENTIFIER:
        [[fallthrough]];
    case UNARY_OPERATOR:
        this->checkIdentifier(token.type);
        break;
    // NOLINTNEXTLINE
    case INTEGER:
    case DOUBLE:
    case CHAR:
    case STRING:
        [[fallthrough]];
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
        [[fallthrough]];
    case OPERATION_EQUAL:
        this->checkEqualOperator();
        break;
    case BOOLEAN_OPERATOR:
    case NOT_OPERATOR:
        [[fallthrough]];
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
        [[fallthrough]];
    case OPEN_SQUARE_BRACKETS:
        this->checkOpenBrackets(token.type);
        break;
    case CLOSED_BRACKETS:
        [[fallthrough]];
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
    case KEYWORD_FOR:
        this->checkKeywordFor();
        break;
    case EOFT:
    case ERROR:
    case UNKNOWN:
        [[fallthrough]];
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

bool Instruction::isExpression() noexcept {
    using enum InstructionType;
    return lastInstructionTypeIs(PARAMETER_EXPRESSION) || lastInstructionTypeIs(ASSIGNATION) || lastInstructionTypeIs(INITIALIZATION) ||
           lastInstructionTypeIs(ARRAY_INIZIALIZATION) || lastInstructionTypeIs(EXPRESSION) ||
           lastInstructionTypeIs(SQUARE_EXPRESSION) || this->isForExpression();
}

bool Instruction::isForExpression() noexcept {
    using enum InstructionType;
    return lastInstructionTypeIs(FOR_INITIALIZATION) || lastInstructionTypeIs(FOR_CONDITION) || lastInstructionTypeIs(FOR_STEP);
}

void Instruction::checkIdentifier(const TokenType &type) noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR, OPEN_BRACKETS, OPEN_SQUARE_BRACKETS};
        this->emplaceBooleanOperator();
        this->emplaceUnaryOperator(type);
        if(this->emplaceTokenType(SQUARE_EXPRESSION, CLOSED_SQUARE_BRACKETS)) { return; }
        if(this->emplaceTokenType(EXPRESSION, CLOSED_BRACKETS)) { return; }
        if(this->lastInstructionTypeIs(PARAMETER_EXPRESSION)) {
            this->allowedTokens.emplace_back(CLOSED_BRACKETS);
            this->allowedTokens.emplace_back(COMMA);
            return;
        }
        if(lastInstructionTypeIs(ARRAY_INIZIALIZATION)) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
            return;
        }
        if(this->emplaceForTokens()) { return; }
        this->emplaceCommaEoft();
        return;
    }
    switch(this->lastInstructionType()) {
    case BLANK:
    case OPERATION:
        this->setLastInstructionType(OPERATION);
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, COMMA, OPEN_SQUARE_BRACKETS, EOFT};
        if(previousTokens.empty()) { this->allowedTokens.emplace_back(OPEN_BRACKETS); }
        this->emplaceUnaryOperator(type);
        break;
    case DECLARATION:
        if(!this->isPreviousEmpty() && this->previousTokensLast() == COLON) {
            this->allowedTokens = {EQUAL_OPERATOR, OPEN_SQUARE_BRACKETS, EOFT};
            break;
        }
        this->allowedTokens = {COMMA, COLON};
        break;
    case FOR_STRUCTURE:
        if(this->isPreviousEmpty()) {
            this->allowedTokens = {};
            break;
        }
        if(this->previousTokensLast() == KEYWORD_VAR) {
            this->allowedTokens = {COLON};
            break;
        }
        this->allowedTokens = {EQUAL_OPERATOR};
        break;
    default:
        this->allowedTokens = {};
        break;
    }
}

void Instruction::checkNumber() noexcept {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        this->emplaceBooleanOperator();
        if(this->emplaceTokenType(SQUARE_EXPRESSION, CLOSED_SQUARE_BRACKETS)) { return; }
        if(this->emplaceTokenType(EXPRESSION, CLOSED_BRACKETS)) { return; }
        if(this->lastInstructionTypeIs(PARAMETER_EXPRESSION)) {
            this->allowedTokens.emplace_back(CLOSED_BRACKETS);
            this->allowedTokens.emplace_back(COMMA);
            return;
        }
        if(lastInstructionTypeIs(ARRAY_INIZIALIZATION)) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
            return;
        }
        if(this->emplaceForTokens()) { return; }
        this->emplaceCommaEoft();
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkMinusOperator() {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkEqualOperator() {
    using enum TokenType;
    using enum InstructionType;
    this->allowedTokens = {IDENTIFIER,          INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS,
                           OPEN_SQUARE_BRACKETS};
    if(lastInstructionTypeIs(OPERATION) || lastInstructionTypeIs(DECLARATION)) {
        if(lastInstructionTypeIs(OPERATION)) {
            this->setLastInstructionType(ASSIGNATION);
        } else {
            this->setLastInstructionType(INITIALIZATION);
        }
        return;
    }
    if(lastInstructionTypeIs(FOR_STRUCTURE)) {
        this->addInstructionType(FOR_INITIALIZATION);
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkBooleanAndLogicalOperator(const TokenType &type) {
    using enum TokenType;
    using enum InstructionType;
    if(this->isExpression()) {
        this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
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
    if(lastInstructionTypeIs(OPERATION) || lastInstructionTypeIs(DECLARATION)) {
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    if(lastInstructionTypeIs(FOR_INITIALIZATION)) {
        this->setLastInstructionType(FOR_CONDITION);
    } else if(lastInstructionTypeIs(FOR_CONDITION)) {
        this->setLastInstructionType(FOR_STEP);
    }
    this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS,
                           OPEN_SQUARE_BRACKETS};
}

void Instruction::checkColon() {
    using enum TokenType;
    using enum InstructionType;
    if(lastInstructionTypeIs(DECLARATION) || lastInstructionTypeIs(FOR_STRUCTURE)) {
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkOpenBrackets(const TokenType &type) {
    using enum TokenType;
    using enum InstructionType;
    this->addBooleanOperatorPresent();
    this->allowedTokens = {IDENTIFIER, INTEGER, DOUBLE, CHAR, STRING, BOOLEAN, MINUS_OPERATOR, NOT_OPERATOR, OPEN_BRACKETS};
    if(type == OPEN_BRACKETS) {
        this->allowedTokens.emplace_back(CLOSED_BRACKETS);
        if(this->previousTokensLast() == IDENTIFIER || this->previousTokensLast() == CLOSED_SQUARE_BRACKETS) {
            if(this->lastInstructionTypeIs(OPERATION)) { this->setLastInstructionType(PROCEDURE_CALL); }
            this->addInstructionType(PARAMETER_EXPRESSION);
            return;
        }
        this->addInstructionType(EXPRESSION);
        return;
    }
    if(this->previousTokensLast() == EQUAL_OPERATOR || this->previousTokensLast() == COMMA ||
       this->previousTokensLast() == OPEN_SQUARE_BRACKETS) {
        this->addInstructionType(ARRAY_INIZIALIZATION);
        this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS);
        this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
        return;
    }
    if(lastInstructionTypeIs(DECLARATION)) { this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS); }
    this->addInstructionType(SQUARE_EXPRESSION);
}

void Instruction::checkClosedBrackets(const TokenType &type) {
    using enum TokenType;
    using enum InstructionType;

    const InstructionType last = this->lastInstructionType();

    this->removeInstructionType();
    this->removeBooleanOperatorPresent();
    if(this->isExpression()) {
        this->allowedTokens = {OPERATOR, MINUS_OPERATOR, LOGICAL_OPERATOR};
        if(last != ARRAY_INIZIALIZATION) {
            this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS);
            if(type == CLOSED_SQUARE_BRACKETS) {
                this->allowedTokens.emplace_back(UNARY_OPERATOR);
                this->allowedTokens.emplace_back(OPEN_BRACKETS);
            }
        }
        this->emplaceBooleanOperator();
        if(this->emplaceTokenType(SQUARE_EXPRESSION, CLOSED_SQUARE_BRACKETS)) { return; }
        if(this->emplaceTokenType(EXPRESSION, CLOSED_BRACKETS)) { return; }
        if(this->lastInstructionTypeIs(PARAMETER_EXPRESSION)) {
            this->allowedTokens.emplace_back(CLOSED_BRACKETS);
            this->allowedTokens.emplace_back(COMMA);
            return;
        }
        if(lastInstructionTypeIs(ARRAY_INIZIALIZATION)) {
            this->allowedTokens.emplace_back(COMMA);
            this->allowedTokens.emplace_back(CLOSED_SQUARE_BRACKETS);
            return;
        }
        if(this->emplaceForTokens()) { return; }
        this->emplaceCommaEoft();
        return;
    }
    switch(this->lastInstructionType()) {
    case PROCEDURE_CALL:
        this->allowedTokens = {eofTokenType};
        break;
    case OPERATION:
        this->allowedTokens = {EQUAL_OPERATOR, OPERATION_EQUAL, COMMA};
        if(type == CLOSED_SQUARE_BRACKETS) { this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS); }
        this->allowedTokens.emplace_back(UNARY_OPERATOR);
        break;
    case DECLARATION:
        this->allowedTokens = {EQUAL_OPERATOR, EOFT};
        if(type == CLOSED_SQUARE_BRACKETS) { this->allowedTokens.emplace_back(OPEN_SQUARE_BRACKETS); }
        break;
    case STRUCTURE:
        this->allowedTokens = {OPEN_CURLY_BRACKETS};
        break;
    default:
        this->allowedTokens = {};
        break;
    }
}

void Instruction::checkOpenCurlyBrackets() {
    using enum InstructionType;
    using enum TokenType;
    if(lastInstructionTypeIs(BLANK)) { this->setLastInstructionType(OPEN_SCOPE); }
    this->allowedTokens = {eofTokenType, CLOSED_CURLY_BRACKETS};
}

void Instruction::checkClosedCurlyBracktes() {
    using enum InstructionType;
    if(lastInstructionTypeIs(BLANK)) { this->setLastInstructionType(CLOSE_SCOPE); }
    this->allowedTokens = {eofTokenType};
}

void Instruction::checkKeywordVar() {
    using enum TokenType;
    using enum InstructionType;
    if(lastInstructionTypeIs(BLANK)) {
        this->setLastInstructionType(DECLARATION);
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    if(lastInstructionTypeIs(FOR_STRUCTURE)) {
        this->allowedTokens = {IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkKeywordStructure() {
    using enum TokenType;
    using enum InstructionType;
    if(lastInstructionTypeIs(BLANK)) {
        this->setLastInstructionType(STRUCTURE);
        this->allowedTokens = {OPEN_BRACKETS};
        return;
    }
    this->allowedTokens = {};
}

void Instruction::checkKeywordFor() {
    using enum TokenType;
    using enum InstructionType;
    if(lastInstructionTypeIs(BLANK)) {
        this->setLastInstructionType(FOR_STRUCTURE);
        this->allowedTokens = {KEYWORD_VAR, IDENTIFIER};
        return;
    }
    this->allowedTokens = {};
}

DISABLE_WARNINGS_POP()
