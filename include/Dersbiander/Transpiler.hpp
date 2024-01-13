#pragma once

#include <vector>
#include <memory>
#include "Instruction.hpp"
#include "Scope.hpp"
#include "TranpilerException.hpp"
#include "Log.hpp"

class Transpiler {
	
	public:
		Transpiler(std::vector<Instruction> _instructions) noexcept;
        void transpile();
    private:
        std::vector<Instruction> instructions;
        std::shared_ptr<Scope> scope;
        bool main;

};