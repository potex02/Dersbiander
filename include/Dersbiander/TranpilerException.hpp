#pragma once

#include <stdexcept>
#include "Instruction.hpp"
#include "Log.hpp"

class TranspilerException : public std::runtime_error {

	public:
    TranspilerException(const std::string &msg, const Instruction &instruction)
      : std::runtime_error(instruction.toString() + "\n" + msg) {}

};