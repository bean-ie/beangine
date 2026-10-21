#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_video.h>
#include <cstdlib>
#include <stdexcept>
#include <vector>

class SDLException final : public std::runtime_error {
    public: 
    explicit SDLException(const std::string& message) : std::runtime_error(message + "\n" + SDL_GetError()) {
    }
};

int main() {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw SDLException("SDL_Init failed");

    // Create a window
    SDL_Window* window{SDL_CreateWindow("beangine", 800, 600, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE)};
    if (!window) 
        throw SDLException("SDL_CreateWindow failed");

    // Create GPU logical device
    SDL_GPUDevice* device{SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_DXIL, 
        true, 
        nullptr)};
    if (!device)
        throw SDLException("SDL_CreateGPUDevice failed");

    // Claim window for the GPU device
    if (!SDL_ClaimWindowForGPUDevice(device, window)) 
        throw SDLException("SDL_ClaimWindowForGPUDevice failed");

    // Show the window
    if (!SDL_ShowWindow(window))
        throw SDLException("SDL_ShowWindow failed");

    bool isRunning{true};
    SDL_Event event;

    // main loop
    while (isRunning) {
        // event loop
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;
                default: 
                    break;
            }
        }

        SDL_GPUCommandBuffer* commandBuffer{SDL_AcquireGPUCommandBuffer(device)};
        if (!commandBuffer) {
            throw SDLException("SDL_AcquireGPUCommandBuffer failed");
        } else {

            SDL_GPUTexture* swapchainTexture;

            if (SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, nullptr, nullptr)) {
                if (swapchainTexture) {
                    SDL_GPUColorTargetInfo colorTarget{};
                    colorTarget.texture = swapchainTexture;
                    colorTarget.clear_color = SDL_FColor{1.0f, 0.0f, 0.0f, 1.0f};
                    colorTarget.load_op = SDL_GPU_LOADOP_CLEAR;
                    colorTarget.store_op = SDL_GPU_STOREOP_STORE;
                    std::vector colorTargets{colorTarget};
                    SDL_GPURenderPass* renderPass{
                        SDL_BeginGPURenderPass(commandBuffer, colorTargets.data(), colorTargets.size(), nullptr)};
                    SDL_EndGPURenderPass(renderPass);
                }

                if(!SDL_SubmitGPUCommandBuffer(commandBuffer)) {
                    throw SDLException("SDL_SubmitGPUCommandBuffer failed");
                }
            }
        }
    }

    return EXIT_SUCCESS;
}