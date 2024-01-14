#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>

class Scope {

    public:
        Scope() noexcept;
        Scope(std::shared_ptr<Scope> _parent) noexcept;
        std::shared_ptr<Scope> getParent() const noexcept;
        void removeParent() noexcept;
    private:
        std::unordered_map<std::string, std::string> identifiers;
        std::unordered_map<std::string, bool> types;
        std::shared_ptr<Scope> parent;
    
};