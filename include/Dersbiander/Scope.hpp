#pragma once
#include <memory>

class Scope {

    public:
        Scope(std::shared_ptr<Scope> _parent = nullptr) noexcept;
    private:
        std::shared_ptr<Scope> parent;

    
};