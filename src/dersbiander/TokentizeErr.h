#pragma once
#include "headers.h"

class TokenizerErr : public std::runtime_error {
public:
#pragma optimize("gt", on)
    explicit TokenizerErr(std::string_view message, std::size_t line, std::size_t column)
      : std::runtime_error(message.data()), line(line), column(column) {}

    std::size_t getLine() const noexcept { return line; }
    std::size_t getColumn() const noexcept { return column; }

private:
    std::size_t line;
    std::size_t column;
};
