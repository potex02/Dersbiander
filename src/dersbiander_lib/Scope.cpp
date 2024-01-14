#include "Dersbiander/Scope.hpp"

Scope::Scope() noexcept {

	this->types["int"] = true;
    this->types["float"] = true;
    this->types["double"] = true;
    this->types["char"] = true;
    this->types["bool"] = true;

}

Scope::Scope(std::shared_ptr<Scope> _parent) noexcept : parent(_parent) {}

std::shared_ptr<Scope> Scope::getParent() const noexcept { return this->parent; }

void Scope::removeParent() noexcept { this->parent = nullptr; }