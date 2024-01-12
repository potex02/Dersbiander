#include "Dersbiander/Transpiler.hpp"
#include "Dersbiander/Log.hpp"

Transpiler::Transpiler(std::vector<Instruction> _instructions) noexcept : instructions(_instructions) {}
void Transpiler::transpile() {
	using enum InstructionType;
	for(const Instruction i : this->instructions) {
		switch(i.getType()) {
			case MAIN:
				LINFO("MAIN");
		}
	}
}