#include "Dersbiander/Scope.hpp"

Scope::Scope(std::shared_ptr<Scope> _parent) noexcept : parent(_parent) {}