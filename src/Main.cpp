#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#if __EMSCRIPTEN__
#include <emscripten.h>
#endif

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
Mix_Chunk* sound = nullptr;

bool running = false;

inline void processFrame() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            Mix_PlayChannel(-1, sound, 0);
        }
        else if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                Mix_PlayChannel(-1, sound, 0);
            }
        }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    // Initialize SDL, SDL_image, and SDL_mixer
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("Unable to initialize SDL_image: %s", IMG_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("Unable to initialize SDL_mixer: %s", Mix_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("AwesomeGame!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        return 1;
    }

    // Load resources
    SDL_Surface* surface = IMG_Load("assets/Illustration.png");
    if (surface == nullptr)
    {
        SDL_Log("Unable to load image: %s", IMG_GetError());
        return 1;
    }

     texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        return 1;
    }

    SDL_FreeSurface(surface);

    sound = Mix_LoadWAV("assets/wow.wav");

    if (sound == nullptr)
    {
        SDL_Log("Unable to load sound: %s", Mix_GetError());
        return 1;
    }

    // Main loop
    running = true;

#if !__EMSCRIPTEN__
    while (running)
    {
        processFrame();
    }
#else
    emscripten_set_main_loop(processFrame, 0, 1);
#endif

    // Free resources and terminate
    Mix_FreeChunk(sound);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
