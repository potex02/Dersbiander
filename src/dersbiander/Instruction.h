#include "Token.h"
#include <vector>

enum class InstructionType : short {

    OPERATION,
    CONDITION,
    DEFINITION,
    BLACK,
    UNKNOWN,

};

class Instruction {
public:
    explicit Instruction(const std::vector<Token> &tokens);
    bool validate();

private:
    std::vector<Token> tokens;
    std::vector<Token> previousTokens;
    InstructionType instructionType;
    bool checkToken(const Token &token);
    bool checkIdentifier(const Token &token);
};