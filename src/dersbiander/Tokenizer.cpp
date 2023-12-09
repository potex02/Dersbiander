#include "Tokenizer.h"
#include "Timer.h"
// #include "TokentizeErr.h"
#include "headers.h"
#include <compare>

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

/**
 * @class Tokenizer
 * @brief A class for tokenizing input strings.
 */
Tokenizer::Tokenizer(const std::string &_input)
  : input(_input), inputSpan(input.c_str(), input.size()), inputSize(input.size()) {}

/**
 * @class Tokenizer
 * @brief This class provides functionality to tokenize a given string into individual tokens based on delimiters.
 *
 * The Tokenizer class provides a public static method tokenize() which takes a string and a set of delimiters as input
 * and returns a vector of tokens obtained by splitting the input string based on the delimiters.
 */
std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    while(currentPosition < inputSize) {
        const char currentChar = inputSpan[currentPosition];
        if(std::isalpha(currentChar)) {
            tokens.emplace_back(extractIdentifier());
        } else if(std::isdigit(currentChar)) {
            tokens.emplace_back(extractnumber());
        } else if(isOperator(currentChar)) {
            tokens.emplace_back(extractOperator());
        } else if(isBrackets(currentChar)) {
            tokens.emplace_back(extractBrackets(currentChar));
        } else if(isApostrophe(currentChar)) {
            tokens.emplace_back(extractChar());
        } else if(std::isspace(currentChar)) {
            handleWhitespace(currentChar);
            continue;  // Continue the loop to get the next token
        } else {
            handleError(std::string(1, currentChar), "Unknown Character");
            std::exit(-1);  // Terminate the program with an error code
        }
    }

    tokens.emplace_back(eofTokenType, "", currentLine, currentColumn - 1);

    return tokens;
}

/**
 * @brief Handles an error encountered during tokenization.
 *
 * This function is responsible for handling an error occurred during tokenization.
 * It takes the values of the tokens that caused the error and the error message as input.
 * The error message should provide information about the type of error encountered.
 *
 * @param values The values of the tokens that caused the error.
 * @param errorMsg The error message describing the error.
 */
void Tokenizer::handleError(const std::string &values, const std::string &errorMsg) {
    Timer timer(errorMsg);

    const auto lineStart = findLineStart();
    const auto lineEnd = findLineEnd();

    std::string contextLine = getContextLine(lineStart, lineEnd);
    std::string highlighting = getHighlighting(lineStart, values.length());
    std::string errorMessage = getErrorMessage(values, errorMsg, contextLine, highlighting);

    LERROR("{}{}", errorMessage, timer.to_string());
}

/**
 * @brief Find the start position of the current line in the given input string.
 *
 * This function finds the start position of the current line in the input string by searching backwards
 * from the current position until it finds a newline character or reaches the beginning of the string.
 *
 * @return The start position of the current line.
 */
std::size_t Tokenizer::findLineStart() {
    std::size_t lineStart = currentPosition;
    while(lineStart > 0 && inputSpan[lineStart - 1] != CNL) { --lineStart; }
    return lineStart;
}

/**
 * @brief Find the end of the current line in the tokenizer's input.
 *
 * This function searches for the next line terminator from the current position in the input string.
 * The line terminator can be either the newline character ('\n') or the carriage return character ('\r').
 * If a line terminator is found, the function returns the position of the first character after the line
 * terminator. If no line terminator is found, it returns std::string::npos.
 *
 * @return The position of the first character after the line terminator, or std::string::npos if no line terminator is found.
 *
 * @note This function assumes that the input string is null-terminated.
 *
 * @note This function updates the tokenizer's current position internally.
 */
std::size_t Tokenizer::findLineEnd() {
    std::size_t lineEnd = currentPosition;
    while(lineEnd < inputSize && inputSpan[lineEnd] != CNL) { ++lineEnd; }
    return lineEnd;
}

/**
 * Retrieves the context line(s) of the given line range.
 * The context line(s) include the line before and after the given range.
 *
 * @param lineStart The starting line of the context range (inclusive).
 * @param lineEnd The ending line of the context range (inclusive).
 * @return The context line(s) of the given range as a string.
 */
std::string Tokenizer::getContextLine(std::size_t lineStart, std::size_t lineEnd) const {
    return std::string(input.begin() + lineStart, input.begin() + lineEnd) + NEWL;
}

/**
 * @brief Get the highlighting information for a specific range of code.
 *
 * This function retrieves the highlighting information for a specific range of code.
 *
 * @param lineStart The start position of the code range (0-indexed).
 * @param length The length of the code range.
 *
 * @return The highlighting information for the specified code range.
 *
 * @note The lineStart parameter should be within the range of available lines.
 *       The length parameter should be a positive value.
 *       The returned highlighting information corresponds to the specified code range.
 *
 * @see Tokenizer
 * @see Highlighting
 */
std::string Tokenizer::getHighlighting(std::size_t lineStart, std::size_t length) const {
    return std::string(currentPosition - lineStart, ' ') + std::string(length, '^') + NEWL;
}

/**
 * Retrieve the error message for the given values.
 *
 * This function generates an error message based on the provided values.
 *
 * @param values The values used in the error message.
 * @param errorMsg The error message template.
 * @param contextLine The line of code or context in which the error occurred.
 * @param highlighting The highlighting information to emphasize the error.
 * @return The generated error message as a string.
 */
std::string Tokenizer::getErrorMessage(const std::string &values, const std::string &errorMsg, const std::string &contextLine,
                                       const std::string &highlighting) const {
    std::ostringstream errorMessageStream;
    errorMessageStream << D_FORMAT("{} '{}' (line {}, column {}):\n", errorMsg, values, currentLine, currentColumn);
    errorMessageStream << "Context:" << NEWL;
    errorMessageStream << contextLine;
    errorMessageStream << highlighting;
    return errorMessageStream.str();
}

/**
 * @brief Appends a character to the value of the tokenizer.
 *
 * This function appends the given character to the value string of the tokenizer. It is used to construct
 * the value of a token during tokenization.
 *
 * @param value A reference to the string where the character will be appended.
 */
void Tokenizer::appendCharToValue(std::string &value) {
    value += inputSpan[currentPosition];
    ++currentPosition;
    ++currentColumn;
}
// NOLINTBEGIN

/**
 * @brief Check if the character is a plus (+) or minus (-) sign.
 *
 * This function checks whether the given character is a plus or minus sign.
 *
 * @param c The character to be checked.
 * @return True if the character is a plus or minus sign, false otherwise.
 */
bool Tokenizer::isPlusORMinus(char c) const noexcept { return c == '+' || c == '-'; }
/**
 * @brief Checks if a given character is an operator.
 *
 * This function determines whether a given character is an operator or not. Operators are defined as special characters
 * used for performing mathematical or logical operations.
 *
 * @param c The character to be checked.
 * @return True if the character is an operator, false otherwise.
 */
bool Tokenizer::isOperator(char c) const noexcept {
    static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-'};
    return operators.contains(c);
}

/**
 * @brief Checks if the given string represents an equal operator operation.
 *
 * This function checks whether the given string represents an equal operator operation.
 *
 * @param value The string to be checked for equal operator operation.
 * @return `true` if the given string represents an equal operator operation, `false` otherwise.
 *
 * @note This function is case-sensitive.
 * @note This function only checks if the given string is an equal operator. It does not validate the correctness of the operation
 * or its context.
 *
 * @sa Tokenizer
 */
bool Tokenizer::isOperationEqualOperator(const std::string &value) const noexcept {
    return value == "+=" || value == "-=" || value == "*=" || value == "/=";
}

/**
 * @brief Checks if the given value is a boolean operator.
 *
 * A boolean operator is a logical operator that operates on boolean operands,
 * such as AND, OR, and NOT.
 *
 * @param value The value to check.
 * @return true if the given value is a boolean operator, false otherwise.
 */
bool Tokenizer::isBooleanOperator(const std::string &value) const noexcept {
    return value == "==" || value == ">=" || value == "<=" || value == "!=";
}

/**
 * @brief Checks if the given character is a bracket.
 *
 * This method determines whether the input character is an opening or closing bracket.
 *
 * @param c The character to be checked.
 * @returns `true` if the character is a bracket, `false` otherwise.
 *
 * @note This method considers the following characters as brackets:
 *   - Opening brackets: '('
 *   - Closing brackets: ')'
 *
 * @par Example:
 * @code
 * Tokenizer tokenizer;
 * bool result = tokenizer.isBrackets(')'); // Returns true
 * bool result = tokenizer.isBrackets('*'); // Returns false
 * @endcode
 */
bool Tokenizer::isBrackets(char c) const noexcept { return c == '(' || c == ')'; }

/**
 * @brief Checks if a given string is a logical operator.
 *
 * This function is a member of the Tokenizer class and checks whether a given string is a logical operator.
 *
 * @param value The string to be checked for being a logical operator.
 * @return True, if the string is a logical operator; false otherwise.
 * @remark This function is case-sensitive.
 * @remark The logical operators include "&&" and "||".
 * @remark This function does not throw exceptions and is marked as noexcept.
 */
bool Tokenizer::isLogicalOperator(const std::string &value) const noexcept { return value == "&&" || value == "||"; }

/**
 * @brief Checks if the given string is a variable-length operator.
 *
 * This function determines if the provided string represents a variable-length operator.
 * A variable-length operator is an operator that can consist of multiple characters.
 *
 * @param val The string to be checked.
 * @return True if the string is a variable-length operator, false otherwise.
 */
bool Tokenizer::isVarLenOperator(const std::string &val) const noexcept {
    return isOperator(val[0]) || isBrackets(val[0]) || isOperationEqualOperator(val) || isBooleanOperator(val) ||
           isLogicalOperator(val);
}

// NOLINTEND

/**
 * @brief Extracts an identifier from the given string.
 *
 * This method attempts to extract an identifier from the given string.
 * An identifier is a sequence of letters, digits, or underscores, starting with a letter or underscore.
 *
 * @param str The input string.
 * @return The extracted identifier, or an empty string if no identifier is found.
 */
Token Tokenizer::extractIdentifier() {
    std::string value;
    TokenType type = TokenType::IDENTIFIER;
    while(currentPosition < inputSize && (std::isalnum(inputSpan[currentPosition]) || inputSpan[currentPosition] == '_')) {
        appendCharToValue(value);
    }
    if(value == "var" || value == "const") { type = TokenType::KEYWORD_VAR; }
    return {type, value, currentLine, currentColumn - value.length()};
}

/**
 * @brief Checks if a character is an apostrophe.
 *
 * This function is a member of the Tokenizer class and is used to check if a given character
 * is an apostrophe. It returns true if the character is an apostrophe, and false otherwise.
 *
 * @param c The character to check.
 * @return True if the character is an apostrophe, false otherwise.
 *
 */
bool Tokenizer::isApostrophe(char c) const noexcept { return c == '\''; }

/**
 * @brief Extracts all the digits from a string.
 *
 * This function extracts all the digits from the input string and returns them as a new string.
 *
 * @param value The input string from which the digits need to be extracted.
 * @return A new string containing only the extracted digits.
 */
void Tokenizer::extractDigits(std::string &value) {
    while(currentPosition < inputSize && std::isdigit(inputSpan[currentPosition])) { appendCharToValue(value); }
}

/**
 * @brief Extracts the exponent from the given string value.
 *
 * This function searches for the exponent part in the given string value and returns it.
 *
 * @param value The string value from which the exponent needs to be extracted.
 *
 * @return The exponent value extracted from the given string value.
 */
void Tokenizer::extractExponent(std::string &value) {
    if(currentPosition < inputSize && isPlusORMinus(inputSpan[currentPosition])) { appendCharToValue(value); }
    extractDigits(value);
}

Token Tokenizer::extractnumber() {
    std::string value;
    extractDigits(value);

    if(currentPosition < inputSize && inputSpan[currentPosition] == PNT) {
        appendCharToValue(value);
        extractDigits(value);

        if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == ECR) {
            appendCharToValue(value);
            extractExponent(value);
        }
        return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
    }

    if(currentPosition < inputSize && std::toupper(inputSpan[currentPosition]) == ECR) {
        appendCharToValue(value);
        extractExponent(value);
        return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
    }
    return {TokenType::INTEGER, value, currentLine, currentColumn - value.length()};
}

/**
 * @brief Extracts a variable length operator from a given string.
 *
 * This function extracts a variable length operator from the provided string. The variable length
 * operator is defined as a sequence of characters that represents an operator and may have multiple
 * characters. The function modifies the provided string to remove the extracted operator.
 *
 * @param value The input string from which the variable length operator needs to be extracted.
 *
 * @return The extracted variable length operator as a string.
 */
void Tokenizer::extractVarLenOperator(std::string &value) {
    while(currentPosition < inputSize && isOperator(inputSpan[currentPosition])) { appendCharToValue(value); }
}

/**
 * @brief Returns the type of the given character.
 *
 * This function determines the type of a character based on some predefined rules. The character must
 * be a single character and is not case sensitive. The function checks if the character is one of the
 * following types: digit, letter, whitespace, special character or invalid character.
 *
 * @param c The character whose type needs to be determined.
 * @return The type of the character.
 */
TokenType Tokenizer::typeBySingleCharacter(char c) const {
    switch(c) {
        using enum TokenType;
    case '-':
        return MINUS_OPERATOR;
    case '=':
        return EQUAL_OPERATOR;
    case ',':
        return COMMA;
    case ':':
        return COLON;
    case '<':
    case '>':
        return BOOLEAN_OPERATOR;
    case '!':
        return NOT_OPERATOR;
    default:
        return OPERATOR;
    }
}

/**
 * @brief Determines the type of a token based on its value.
 *
 * This function takes a string value as input and returns the corresponding
 * type of the token. The token types are defined in the Tokenizer class.
 *
 * @param value The value of the token.
 * @return The type of the token as an enum value from the TokenType enumeration.
 */
TokenType Tokenizer::typeByValue(const std::string &value) const {
    using enum TokenType;
    if(isOperationEqualOperator(value)) { return OPERATION_EQUAL; }
    if(isBooleanOperator(value)) { return BOOLEAN_OPERATOR; }
    if(isLogicalOperator(value)) { return LOGICAL_OPERATOR; }
    return UNKNOWN;
}

/**
 * @class Tokenizer
 * @brief A class responsible for tokenizing a string into operators and operands.
 *
 * The Tokenizer class provides a method to extract operators from a given string.
 */
Token Tokenizer::extractOperator() {
    std::string value;
    extractVarLenOperator(value);
    TokenType type = value.size() == 1 ? typeBySingleCharacter(value[0]) : typeByValue(value);
    return {type, value, currentLine, currentColumn - value.length()};
}

/**
 * @brief Extracts brackets from a given character.
 *
 * This class provides a method to extract brackets from a given character. It searches for matching opening
 * and closing brackets and returns them as a pair. If the input character is not a bracket or if it does not
 * have a matching closing bracket, an empty pair is returned.
 *
 * @param c The character to extract brackets from.
 * @return A pair of opening and closing brackets found in the input character. If no brackets are found, an
 *         empty pair is returned.
 */
Token Tokenizer::extractBrackets(char c) {
    using enum TokenType;
    TokenType type{};
    switch(c) {
    case '(':
        type = OPEN_BRACKETS;
        break;
    case ')':
        type = CLOSED_BRACKETS;
        break;
    default:
        type = UNKNOWN;
    }
    ++currentPosition;
    ++currentColumn;
    return {type, std::string(1, c), currentLine, currentColumn - 1};
}

Token Tokenizer::extractChar() {
    std::size_t startcol = currentColumn;
    ++currentPosition;
    ++currentColumn;
    std::string value;
    while(!isApostrophe(inputSpan[currentPosition])) { appendCharToValue(value); }
    ++currentPosition;
    ++currentColumn;
    return {TokenType::CHAR, value, currentLine, currentColumn - startcol};
}

/**
 * @brief Handles whitespace character during tokenization.
 *
 * This function is used by the Tokenizer class to handle whitespace characters
 * encountered during tokenization process.
 *
 * @param currentChar The current character being processed.
 */
void Tokenizer::handleWhitespace(char currentChar) noexcept {
    if(currentChar == CNL) {
        ++currentLine;
        currentColumn = 1;
    } else {
        ++currentColumn;
    }
    ++currentPosition;
}

DISABLE_WARNINGS_POP()
