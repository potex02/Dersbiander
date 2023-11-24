#include <vector>
#include "Token.h"

enum class InstructionType : short {

    OPERATION,
    CONDITION,
    DEFINITION,
    BLANK,
    UNKNOWN,

};

class Instruction {
public:
    explicit Instruction(const std::vector<Token> &tokens);
    std::string validate();

private:
    std::vector<Token> tokens;
    std::vector<Token> previousTokens;
    InstructionType instructionType;
    std::string unexpected(const Token &token);
    bool checkToken(const Token &token);
    bool checkIdentifier();
    bool checkNumber();
    bool checkOperator();
    bool checkKeyWord();

};