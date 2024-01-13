#pragma once

#include <vector>
#include <fstream>
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
        std::ofstream output;
        std::vector<Instruction> instructions;
        std::shared_ptr<Scope> scope;
        bool main;
        void write(const std::string& str);
        void checkTrailingBracket(const Instruction& instruction);
        void openScope();
        void closeScope();

};