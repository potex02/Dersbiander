#pragma once

#include <stdexcept>

class TranspilerException : public std::runtime_error {

	public:
		TranspilerException(const std::string& msg) : std::runtime_error(msg) {};

};