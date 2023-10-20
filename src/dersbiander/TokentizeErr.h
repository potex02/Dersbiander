#pragma once
#include "headers.h"

class TokenizerErr : public std::runtime_error {
public:
#pragma optimize("gt", on)
    explicit TokenizerErr(std::string_view message) : std::runtime_error(message.data()) {}
};
