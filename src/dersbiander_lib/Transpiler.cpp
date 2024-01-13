#include "Dersbiander/Transpiler.hpp"

Transpiler::Transpiler(std::vector<Instruction> _instructions) noexcept : instructions(_instructions), scope(), main(false) {}
void Transpiler::transpile() {
	
    this->output.open("output.cpp");
	this->write("#include <iostream>\n");
	using enum InstructionType;
    try {
		for(const Instruction i : this->instructions) {
			switch(i.getType()) {
				case MAIN:
					if(this->main) { throw TranspilerException("Main already declared", i); }
					this->write("int main() {");
                    this->openScope();
                    this->checkTrailingBracket(i);
                    this->write("\n");
					this->main = true;
					break;
			}
		}
	}
	catch (TranspilerException& e) {

		LERROR("{}", e.what());

	}
    output.close();

}

void Transpiler::write(const std::string& str) {
	this->output << str;
}

void Transpiler::checkTrailingBracket(const Instruction& instruction) {
	if (instruction.size() == 0) { return; }
    if(instruction.getToken(instruction.size() - 1).getType() == TokenType::CLOSED_CURLY_BRACKETS) {
        this->write("}");
        this->closeScope();
    }
}

void Transpiler::openScope() {

	std::shared_ptr<Scope> newScope = std::make_shared<Scope>(this->scope);
    
    this->scope = newScope;
}

void Transpiler::closeScope() {

    std::shared_ptr<Scope> oldScope = this->scope;
    
    this->scope = this->scope->getParent();
    oldScope->removeParent();
}