#pragma once

#include "headers.h"

class TokenizerErr : public std::runtime_error {
public:
    explicit TokenizerErr(std::string_view message, std::size_t line, std::size_t column) // NOLINT(*-easily-swappable-parameters)
      : std::runtime_error(message.data()), line(line), column(column) {}

    [[nodiscard]] std::size_t getLine() const noexcept { return line; }
    [[nodiscard]] std::size_t getColumn() const noexcept { return column; }

private:
    std::size_t line;
    std::size_t column;
};
