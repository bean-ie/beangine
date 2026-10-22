#include "utils.h"
#include <SDL3/SDL_error.h>

SDLException::SDLException(const std::string& message) : std::runtime_error(message + "\n" + SDL_GetError()) {}