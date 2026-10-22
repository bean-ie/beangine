#pragma once

#include <stdexcept>

class SDLException final : public std::runtime_error {
    public: 
    explicit SDLException(const std::string& message);
};