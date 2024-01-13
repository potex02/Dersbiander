#pragma once
#include <memory>

class Scope {

    public:
        Scope(std::shared_ptr<Scope> _parent = nullptr) noexcept;
        std::shared_ptr<Scope> getParent() const noexcept;
        void removeParent() noexcept;
    private:
        std::shared_ptr<Scope> parent;

    
};