#pragma once

#include <vector>
#include "Instruction.hpp"

class Transpiler {
	
	public:
		Transpiler(std::vector<Instruction> _instructions) noexcept;
        void transpile();
    private:
        std::vector<Instruction> instructions;

};