#include "Tokenizer.h"
#include "Timer.h"
// #include "TokentizeErr.h"
#include "headers.h"

DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820 6244 6285 6385 6386 26409 26415 26418 26429 26432 26437 26438 26440 26446 26447 26450 26451 26455 26457 26459 26460 26461 26467 26472 26473 26474 26475 26481 26482 26485 26490 26491 26493 26494 26495 26496 26497 26498 26800 26814 26818 26826)

/**
 * @class Tokenizer
 * @brief A class for tokenizing a given input string.
 */
Tokenizer::Tokenizer(const std::string &_input)
  : input(_input), inputSpan(input.c_str(), input.size()), inputSize(input.size()) {}

bool Tokenizer::isPositionInText() const noexcept { return currentPosition < inputSize; }
/**
 * @brief Tokenizes a given string into individual tokens.
 *
 * This function takes a string as input and breaks it down into multiple tokens. The tokens are determined based on
 * certain delimiters provided as input. By default, whitespace characters (' ', '\t', '\n') are considered as delimiters.
 *
 * @param str The input string to be tokenized.
 * @param delimiters (Optional) The delimiters used for tokenizing the input string. Defaults to whitespace characters.
 *
 * @return A vector of tokens extracted from the input string based on the specified delimiters.
 *
 * @note The input string will not be modified by this function.
 * @note This function will disregard consecutive occurrences of delimiters and will not generate empty tokens.
 *
 * @sa Tokenizer::setDelimiters()
 */
std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    while(isPositionInText()) {
        const char currentChar = inputSpan[currentPosition];
        if(std::isalpha(currentChar)) {
            tokens.emplace_back(extractIdentifier());
        } else if(std::isdigit(currentChar)) {
            tokens.emplace_back(extractnumber());
        } else if(isComment(currentPosition)) {
            tokens.emplace_back(extractComment());
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
 * @brief Handles errors encountered during tokenization process.
 *
 * This function is responsible for handling errors that occur during the tokenization
 * process in the Tokenizer class. It takes the input values and error message as
 * parameters and performs appropriate error handling actions.
 *
 * @param values The input values that resulted in an error.
 * @param errorMsg The error message describing the nature of the error.
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
 * \class Tokenizer
 * \brief This class is responsible for tokenizing a given input string.
 */
std::size_t Tokenizer::findLineStart() {
    std::size_t lineStart = currentPosition;
    while(lineStart > 0 && inputSpan[lineStart - 1] != CNL) { --lineStart; }
    return lineStart;
}

/**
 * \class Tokenizer
 *
 * \brief The Tokenizer class provides a method to find the end position of a line within a given text.
 *
 * The Tokenizer class is designed to handle text parsing and provides various methods for tokenizing strings.
 * One of the methods provided is the findLineEnd() method, which returns the end position (index) of a line within a given text.
 * This is useful when parsing text files and processing line-by-line.
 */
std::size_t Tokenizer::findLineEnd() {
    std::size_t lineEnd = currentPosition;
    while(lineEnd < inputSize && inputSpan[lineEnd] != CNL) { ++lineEnd; }
    return lineEnd;
}

/**
 * \brief Retrieves a context line given the start and end positions of a line.
 *
 * This function returns a context line starting from position 'lineStart'
 * and ending at position 'lineEnd' (inclusive) within the input string. The
 * context line contains the characters around the requested line to provide
 * a contextual understanding.
 *
 * \param lineStart The starting position of the line within the input string.
 * \param lineEnd The ending position of the line within the input string.
 *
 * \return The context line from 'lineStart' to 'lineEnd' (inclusive).
 */
std::string Tokenizer::getContextLine(std::size_t lineStart, std::size_t lineEnd) const {
    return std::string(input.begin() + static_cast<long>(lineStart), input.begin() + static_cast<long>(lineEnd)) +
           NEWL;  // NOLINT(*-narrowing-conversions)
}

/**
 * @brief Get the highlighting information for a specific line in the tokenizer.
 *
 * This function returns the highlighting information for a specific line in the tokenizer based on the line's starting index and
 * length.
 *
 * @param lineStart The starting index of the line in the tokenizer.
 * @param length The length of the line in the tokenizer.
 * @return The highlighting information for the specified line.
 *
 * @note The highlighting information can be used to apply different colors or styles to the tokens in the line,
 *       such as keywords, comments, strings, etc.
 *
 * @warning The lineStart parameter should be a valid index within the tokenizer, otherwise undefined behavior may occur.
 *          The length parameter should be a positive non-zero value, otherwise the returned highlighting information may be
 * incorrect.
 *
 * @see Tokenizer
 * @see Token
 */
std::string Tokenizer::getHighlighting(std::size_t lineStart, std::size_t length) const {
    return std::string(currentPosition - lineStart, ' ') + std::string(length, '^') + NEWL;
}

/**
 * @brief Get the error message for a given line of text.
 *
 * This function generates an error message for a given line of text, displaying additional values,
 * the error message itself, and highlighting specific parts of the line.
 *
 * @param values        Additional values to be displayed in the error message.
 * @param errMsg        The error message.
 * @param contextLine   The line of text in which the error occurred.
 * @param highlighting  The specific parts of the line to be highlighted.
 *
 * @return The generated error message.
 */
std::string Tokenizer::getErrorMessage(const std::string &values, const std::string &errMsg, const std::string &contextLine,
                                       const std::string &highlighting) const {
    std::ostringstream errorMessageStream;
    errorMessageStream << D_FORMAT("{} '{}' (line {}, column {}):\n", errMsg, values, currentLine, currentColumn);
    errorMessageStream << "Context:" << NEWL;
    errorMessageStream << contextLine;
    errorMessageStream << highlighting;
    return errorMessageStream.str();
}

/**
 * @class Tokenizer
 * @brief The Tokenizer class provides a method to append a character to a value.
 */
void Tokenizer::appendCharToValue(std::string &value) {
    value += inputSpan[currentPosition];
    incPosAndCol();
}

/**
 * Increment the current position and column of the tokenizer
 * This function is used to track the current position and column of the tokenizer in the text as it's taking in the input and
 * processing it.
 *
 * @pre The object has been properly initialized
 * @post The current position and column of the tokenizer have been incremented by one
 *
 * @note This function modifies the internal state of the object but is not visible externally. It affects how the tokenizer
 * processes the input text.
 */
void Tokenizer::incPosAndCol() {
    ++currentPosition;  // Increment the current position within the text
    ++currentColumn;    // Increment the current column within the line
}

// NOLINTBEGIN
bool Tokenizer::isPlusORMinus(char c) const noexcept { return c == '+' || c == '-'; }

/**
 * @brief Checks if the character at the given position is part of a comment.
 * @param position The position of the character to check.
 * @return true if the character at the given position is part of a comment, false otherwise.
 */
bool Tokenizer::isComment(size_t position) const noexcept {
    return position != inputSpan.size() && input[position] == '/' &&
           (inputSpan[position + 1] == '/' || inputSpan[position + 1] == '*');
}
/**
 * @brief Checks if a character is an operator.
 *
 * This function checks whether the given character is an operator.
 *
 * @param c The character to check.
 * @return true if the character is an operator, false otherwise.
 */
bool Tokenizer::isOperator(char c) const noexcept {
    static const std::unordered_set<char> operators = {'*', '/', '=', ',', ':', '<', '>', '!', '|', '&', '+', '-'};
    return operators.contains(c);
}

/**
 * @brief Checks if the given value is an equal operator.
 *
 * This function checks if the given string value is an equal operator.
 * An equal operator is a binary operator that compares two operands for equality.
 *
 * @param value The string value to check.
 * @return True if the value is an equal operator, false otherwise.
 * @remark The value is case-sensitive.
 */
bool Tokenizer::isOperationEqualOperator(const std::string &value) const noexcept {
    return value == "+=" || value == "-=" || value == "*=" || value == "/=";
}

/**
 * @brief Checks if the given value is a boolean operator.
 *
 * This function verifies if the provided string value is a boolean operator. A boolean operator is a logical
 * operator that operates on boolean values. The supported boolean operators in this tokenizer are 'AND', 'OR',
 * and 'NOT'.
 *
 * @param value The string value to check for being a boolean operator.
 * @return true if the value is a boolean operator, false otherwise.
 *
 * @note This function is case-sensitive and only considers 'AND', 'OR', and 'NOT' as valid boolean operators.
 * Other values will be considered as not being boolean operators.
 * @note This function is noexcept, so it does not throw any exception.
 */
bool Tokenizer::isBooleanOperator(const std::string &value) const noexcept {
    return value == "==" || value == ">=" || value == "<=" || value == "!=";
}

/**
 * @brief Checks if a character is a bracket.
 *
 * This function determines whether a given character is a bracket or not.
 * It is const and noexcept, meaning it does not modify the object's state
 * and does not throw any exceptions.
 *
 * @param c The character to check.
 * @return true if the character is a bracket, false otherwise.
 */
bool Tokenizer::isBrackets(char c) const noexcept { return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}'; }

/**
 * @brief Checks if a given string is a logical operator.
 *
 * This function determines if a given string represents a logical operator.
 * A logical operator is a special symbol or sequence of symbols used to
 * combine or modify logical predicates.
 *
 * @param value The string to be checked.
 * @return `true` if the string is a logical operator, `false` otherwise.
 * @remark The function is case-sensitive.
 * @remark This function is `const` and does not modify the state of the `Tokenizer`.
 * @remark The function is marked as `noexcept` and will not throw any exceptions.
 */
bool Tokenizer::isLogicalOperator(const std::string &value) const noexcept { return value == "&&" || value == "||"; }

/**
 * @brief Checks whether the given string represents a variable length operator.
 *
 * A variable length operator is an operator that can have different lengths, such as "+=" or ">>=".
 *
 * @param val The string to be checked.
 * @return true if the string represents a variable length operator, false otherwise.
 */
bool Tokenizer::isVarLenOperator(const std::string &val) const noexcept {
    return isOperator(val[0]) || isBrackets(val[0]) || isOperationEqualOperator(val) || isBooleanOperator(val) ||
           isLogicalOperator(val);
}

// NOLINTEND

/**
 * @class Tokenizer
 * @brief A class for tokenizing a string and extracting identifiers
 */
Token Tokenizer::extractIdentifier() {
    using enum TokenType;
    std::string value;
    TokenType type = IDENTIFIER;
    while(isPositionInText() && (std::isalnum(inputSpan[currentPosition]) || inputSpan[currentPosition] == '_')) {
        appendCharToValue(value);
    }
    if(value == "var" || value == "const") { type = KEYWORD_VAR; }
    if(value == "if" || value == "while") { type = KEYWORD_STRUCTURE; }
    if(value == "true" || value == "false") { type = BOOLEAN; }
    if(currentPosition + 1 < inputSize && (isCurrentAndNextPlusOrMinus())) {
        appendCharToValue(value);
        appendCharToValue(value);
    }
    return {type, value, currentLine, currentColumn - value.length()};
}
bool Tokenizer::isCurrentAndNextPlusOrMinus() const noexcept { return isPlusPlus() || isMinusMinus(); }
bool Tokenizer::isMinusMinus() const noexcept {
    return inputSpan[currentPosition] == '-' && inputSpan[currentPosition + 1] == '-';
}
bool Tokenizer::isPlusPlus() const noexcept { return inputSpan[currentPosition] == '+' && inputSpan[currentPosition + 1] == '+'; }

/**
 * @class Tokenizer
 * @brief A class for tokenizing a string.
 */
bool Tokenizer::isApostrophe(char c) const noexcept { return c == '\''; }

/**
 * @brief Extracts all digits from a given string.
 *
 * This function takes a reference to a string and extracts all digits from the string,
 * removing any non-digit characters. The digits are stored back into the original string.
 *
 * @param value A reference to the string to extract digits from.
 *
 * @return The number of digits extracted from the string.
 */
void Tokenizer::extractDigits(std::string &value) {
    while(isPositionInText() && std::isdigit(inputSpan[currentPosition])) { appendCharToValue(value); }
}

/**
 * @brief Extracts the exponent from a given string representing a numeric value.
 *
 * This function searches for the exponent part in the given string and returns it.
 * The exponent part is expected to follow the "e" or "E" character, indicating the power
 * of 10 by which the value needs to be multiplied.
 *
 * @param value The string representing a numeric value from which the exponent needs to be extracted.
 * @return The exponent part as a string, or an empty string if no exponent is found.
 *
 * @note The input string is modified in-place, removing the extracted exponent part.
 * @note This function assumes that the input string is a valid numeric value with at most one exponent part.
 *
 * @code
 * std::string value = "2.5e10";
 * std::string exponent = Tokenizer::extractExponent(value);
 * // After calling the function, value = "2.5", and exponent = "10".
 * @endcode
 */
void Tokenizer::extractExponent(std::string &value) {
    if(isPositionInText() && isPlusORMinus(inputSpan[currentPosition])) { appendCharToValue(value); }
    extractDigits(value);
}

/**
 * @class Tokenizer
 * @brief A class for tokenizing a string and extracting numbers from it.
 */
Token Tokenizer::extractnumber() {
    std::string value;
    extractDigits(value);

    if(isPositionInText() && inputSpan[currentPosition] == PNT) {
        appendCharToValue(value);
        extractDigits(value);

        if(isPositionInText() && std::toupper(inputSpan[currentPosition]) == ECR) {
            appendCharToValue(value);
            extractExponent(value);
        }
        return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
    }

    if(isPositionInText() && std::toupper(inputSpan[currentPosition]) == ECR) {
        appendCharToValue(value);
        extractExponent(value);
        return {TokenType::DOUBLE, value, currentLine, currentColumn - value.length()};
    }
    return {TokenType::INTEGER, value, currentLine, currentColumn - value.length()};
}

/**
 * @brief Extracts a variable length operator from a given string.
 *
 * This function is responsible for extracting a variable length operator from a given string.
 *
 * @param value The string from which to extract the operator. The string will be modified to remove the extracted operator.
 *
 * @return A string containing the extracted operator, or an empty string if no operator is found.
 */
void Tokenizer::extractVarLenOperator(std::string &value) {
    while(isPositionInText() && isOperator(inputSpan[currentPosition])) { appendCharToValue(value); }
}

/**
 * @class Tokenizer
 * @brief A class for tokenizing strings and determining the type of a single character
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
 * \class Tokenizer
 * \brief This class provides a method to determine the token type based on its value.
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
 * @brief Responsible for tokenizing input strings
 *
 * The Tokenizer class provides methods to extract operators, operands,
 * and other tokens from a given input string.
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
 * This function is used to extract brackets from a given character.
 * The extracted brackets are determined based on the input character,
 * which can be one of the following: '(', ')', '[', ']', '{', or '}'.
 *
 * @param c The character from which to extract brackets.
 * @return A string containing the extracted brackets.
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
    case '[':
        type = OPEN_SQUARE_BRACKETS;
        break;
    case ']':
        type = CLOSED_SQUARE_BRACKETS;
        break;
    case '{':
        type = OPEN_CURLY_BRACKETS;
        break;
    case '}':
        type = CLOSED_CURLY_BRACKETS;
        break;
    default:
        type = UNKNOWN;
    }
    incPosAndCol();
    return {type, std::string(1, c), currentLine, currentColumn - 1};
}

/**
 * @class Tokenizer
 * @brief Class for tokenizing a string
 *
 * This class provides methods for extracting characters from a string
 * and tokenizing it based on specific delimiters.
 */
Token Tokenizer::extractChar() {
    using enum TokenType;
    std::size_t startcol = currentColumn;
    incPosAndCol();
    std::string value;
    while(!isApostrophe(inputSpan[currentPosition])) {
        if(currentPosition + 1 == inputSpan.size() || inputSpan[currentPosition] == CNL) {
            return {UNKNOWN, "'" + value + "'", currentLine, currentColumn - startcol};
        }
        appendCharToValue(value);
    }
    incPosAndCol();
    if(value.empty() || isEscapedChar(value)) { return {CHAR, value, currentLine, currentColumn - startcol}; }
    return {UNKNOWN, "'" + value + "'", currentLine, currentColumn - startcol};
}

bool Tokenizer::isEscapedChar(const std::string &val) const noexcept {
    return (val.size() == 1 && val != "\\") || (val.size() == 2 && val[0] == '\\');
}

/**
 * @brief Extracts a comment from a given line of code.
 *
 * This function searches for a comment in a line of code and extracts it.
 * A comment is identified by the presence of a single line or multi line comment at the beginning
 * of the line. Inline comments are extracted until the end of the line
 * or until the start of a block comment is encountered.
 *
 * @param line The line of code to extract the comment from.
 * @return The extracted comment, or an empty string if no comment is found.
 */
Token Tokenizer::extractComment() {
    using enum TokenType;
    if(inputSpan[currentPosition + 1] == '/') { return {COMMENT, handleWithSingleLineComment(), currentLine, currentColumn}; }
    if(inputSpan[currentPosition + 1] == '*') {
        auto [result, value] = this->handleWithMultilineComment();
        if(!result) { return {UNKNOWN, value, currentLine, currentColumn}; }
        return {COMMENT, value, currentLine, currentColumn};
    }
    return {UNKNOWN, "", currentLine, currentColumn};
}

/**
 * @brief Handles processing of single line comments in the tokenizer.
 *
 * This function is responsible for handling the processing of single line comments
 * in the tokenizer. It identifies the single line comment delimiter and removes
 * the entire comment from the provided input string. It updates the tokenizer's
 * internal state accordingly.
 *
 * @param input The input string to process.
 */
std::string Tokenizer::handleWithSingleLineComment() {
    std::string value;

    while(inputSpan[currentPosition] != CNL) { appendCharToValue(value); }
    return value;
}

/**

* \brief Handles the input with multiline comment.

*

* This function is responsible for handling the input when a multiline comment

* is encountered in the code. It reads the input character by character and

* ignores all the characters until the closing tag '*/
std::pair<bool, std::string> Tokenizer::handleWithMultilineComment() {
    std::string value;

    while(currentPosition + 1 != inputSize && (inputSpan[currentPosition] != '*' || inputSpan[currentPosition + 1] != '/')) {
        if(inputSpan[currentPosition] == CNL) {
            ++currentLine;
            currentColumn = 1;
        }
        appendCharToValue(value);
    }
    if(currentPosition + 1 == inputSize) { return {false, value}; }
    appendCharToValue(value);
    appendCharToValue(value);
    return {true, value};
}

/**
 * @brief Handles whitespace characters in the Tokenizer.
 *
 * This function is responsible for processing whitespace characters in the Tokenizer.
 * It performs the necessary actions based on the given whitespace character.
 *
 * @param currentChar The current whitespace character to handle.
 * @return void
 * @note This function is declared as noexcept.
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
