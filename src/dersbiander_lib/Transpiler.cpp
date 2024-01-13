#include <fstream>
#include "Dersbiander/Transpiler.hpp"

Transpiler::Transpiler(std::vector<Instruction> _instructions) noexcept : instructions(_instructions), scope(), main(false) {}
void Transpiler::transpile() {

	std::ofstream output;
    output.open("output.cpp");
	
	output << "#include <iostream>\n";
	using enum InstructionType;
    try {
		for(const Instruction i : this->instructions) {
			switch(i.getType()) {
				case MAIN:
					if(this->main) { throw TranspilerException("Main already declared"); }
					output << "int main() {}\n";
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