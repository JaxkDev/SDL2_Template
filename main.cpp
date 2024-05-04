#include <iostream>
#include <SDL.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv []) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    //DEMO WINDOW
    SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_BORDERLESS);
    if (win == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_HideWindow(win);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++){
            SDL_SetRenderDrawColor(ren, rand() % 255, rand() % 255, rand() % 255, 255);
            SDL_Rect r;
            r.x = 50*i;
            r.y = 50*j;
            r.w = 50;
            r.h = 50;
            SDL_RenderFillRect(ren, &r);
            //SDL_RenderPresent(ren);// - Show individual draws.
        }
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning!", "Failed to do x y z.", win);
    SDL_ShowWindow(win);
    SDL_RenderPresent(ren);

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                if (e.key.keysym.sym == SDLK_ESCAPE){
                    quit = true;
                }
            }
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
