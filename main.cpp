#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

int main(int argv, char** args) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        std::cout << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return 1;
    }

    // Load and display an image
    SDL_Window* window = SDL_CreateWindow("SDL_image Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* imageSurface = IMG_Load("My_pic.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageSurface);

    // Clear the renderer
    SDL_RenderClear(renderer);

    // Render the texture
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for a key press to exit
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                quit = true;
            }
        }
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(imageSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}