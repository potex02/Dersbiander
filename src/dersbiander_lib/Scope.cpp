#include "Dersbiander/Scope.hpp"

Scope::Scope(std::shared_ptr<Scope> _parent) noexcept : parent(_parent) {}

std::shared_ptr<Scope> Scope::getParent() const noexcept { return this->parent; }

void Scope::removeParent() noexcept { this->parent = nullptr; }