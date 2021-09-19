#include "Game.h"

Game* Game::instance = nullptr;

Game::Game (std::string title, int width, int height) {
    int flags, opaudio;

    // Game Instance
    if (instance != nullptr) {
        SDL_Log("Something went wrong...");
        exit(1);
    } else {
        instance = this;
    }
    this->title = title;
    this->width = width;
    this->height = height;

    // SDL
    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if (SDL_Init(flags) != OP_SUCCESS) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        exit(1);
    } else SDL_Log("SDL_Init: OK");

    // IMG
    flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if (IMG_Init(flags) == OP_FAILURE) {
        SDL_Log("IMG_Init: %s", IMG_GetError());
        exit(1);
    } else SDL_Log("IMG_Init: OK");

    // Mixer
    flags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD;
    if (Mix_Init(flags) == OP_FAILURE) {
        SDL_Log("Mix_Init: %s", Mix_GetError());
    }
    opaudio = Mix_OpenAudio(
        MIX_DEFAULT_FREQUENCY,
        MIX_DEFAULT_FORMAT,
        MIX_DEFAULT_CHANNELS,
        MUS_CHUNK_SIZE
    );
    if (opaudio != OP_SUCCESS) {
        SDL_Log("Mix_OpenAudio: %s", Mix_GetError());
    } else SDL_Log("Mix_Init: OK");
    Mix_AllocateChannels(MUS_CHANNELS);

    // window
    window = SDL_CreateWindow(
        this->title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        this->width, this->height, WINDOW_FLAGS
    );
    if (window == nullptr) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        exit(1);
    }

    // renderer
    renderer = SDL_CreateRenderer(
        window, AUTO_DRIVE_ID,
        SDL_RENDERER_ACCELERATED
    );
    if (renderer == nullptr) {
        SDL_Log("Unable to start renderer: %s", SDL_GetError());
        exit(1);
    }

    srand(time(NULL));
    this->state = new State();
}

Game::~Game () {
    delete state;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    instance = nullptr;
}

Game& Game::GetInstance () {
    if (instance == nullptr) {
        instance = new Game(WINDOW_TITLE, WINDOW_SIZE);
    }
    return *instance;
}

State& Game::GetState () {
    return *state;
}

SDL_Renderer* Game::GetRenderer () {
    return renderer;
}

void Game::Run () {
    state->LoadAssets();
    while (state->QuitRequested() == false) {
        state->Update(0);
        state->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY_VALUE);
    }
}