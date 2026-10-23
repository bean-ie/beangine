#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include "utils.h"

SDL_Window* window;
SDL_GPUDevice* device;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    window = SDL_CreateWindow("beangine", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) throw SDLException{"Window creation failed!"};
    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
    if (!device) throw SDLException{"Failed to acquire GPU!"};

    if (!SDL_ClaimWindowForGPUDevice(device, window)) throw SDLException{"Failed to claim window to device!"};

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);

    SDL_GPUTexture* swapchainTexture;
    Uint32 width, height;
    SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &width, &height);

    if (swapchainTexture == NULL) {
        SDL_SubmitGPUCommandBuffer(commandBuffer);
        return SDL_APP_CONTINUE;
    }

    SDL_GPUColorTargetInfo colorTargetInfo{};
    colorTargetInfo.clear_color = {240/255.0f, 240/255.0f, 255/255.0f};
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    colorTargetInfo.texture = swapchainTexture;

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);
    


    SDL_EndGPURenderPass(renderPass);
    
    SDL_SubmitGPUCommandBuffer(commandBuffer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        return SDL_APP_SUCCESS;
    }
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
}

int main() {

    return EXIT_SUCCESS;
}