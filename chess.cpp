#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 1920
#define HEIGHT 1080
#define IMG_PATH "card1.png"
#define IMG_PATH2 "card2.png"
#define IMG_SELECT "select2.png"
#define MOVE_STEP 88
#define MOVE_STEPy 128
#define NUM_IMGS 2

bool istherepiece(int x, int y, SDL_Rect imgs[]) {
    for (int piece = 0; piece < NUM_IMGS; piece++) {
        if (imgs[piece].x == x && imgs[piece].y == y) {
            return true;
        }
    }
    return false;
}

void resetAll(bool img_select_bool[]) {
    for (int piece = 0; piece < NUM_IMGS; piece++) {
        img_select_bool[piece] = false;
    }
}
bool isMovingCorrectly(int x, int y, int desiredx, int desiredy) {
    //fuck you
    if (y < 0 || x < 0) {
        return false;
    }
    int vY = abs((y - 124) / 128);
    int vX = abs((x - 58) / 88);
    int vdesiredx = abs((desiredx - 58) / 88);
    int vdesiredy = abs((desiredy - 124) / 128);
    if (abs(vX - vdesiredx) <= 1 && abs(vY - vdesiredy) <= 1) {return true;}
        

    return false;
}

int main(int argc, char *argv[]) {
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *img = NULL;
    SDL_Texture *img2 = NULL;
    SDL_Texture *select = NULL;
    int img_w, img_h, img2_w, img2_h, select_w, select_h;

    SDL_Init(SDL_INIT_VIDEO);
    
    win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    img = IMG_LoadTexture(renderer, IMG_PATH);
    img2 = IMG_LoadTexture(renderer, IMG_PATH2);
    select = IMG_LoadTexture(renderer, IMG_SELECT);

    SDL_QueryTexture(img, NULL, NULL, &img_w, &img_h);
    SDL_QueryTexture(img2, NULL, NULL, &img2_w, &img2_h);
    SDL_QueryTexture(select, NULL, NULL, &select_w, &select_h);

    SDL_Rect img_rects[NUM_IMGS];
    int img_rectstype[NUM_IMGS] = {0, 1}; 
    bool img_selected_bool[NUM_IMGS] = {false, false};

    for (int i = 0; i < NUM_IMGS; i++) {
        if (img_rectstype[i] == 0) {
            img_rects[i] = {WIDTH / 2 - img_w / 2 + i * MOVE_STEP, HEIGHT / 2 - img_h / 2, img_w * 2, img_h * 2};
        } else if (img_rectstype[i] == 1) {
            img_rects[i] = {WIDTH / 2 - img2_w / 2 + i * MOVE_STEP, HEIGHT / 2 - img2_h / 2, img2_w * 2, img2_h * 2};
        }
    }

    SDL_Rect select_rect = {WIDTH / 2 - select_w / 2, HEIGHT / 2 - select_h / 2, select_w * 2, select_h * 2};
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

    while (1) {
        SDL_Event e;
        bool space_pressed = false;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                break;
            else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                break;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        select_rect.y -= MOVE_STEPy;
                        break;
                    case SDLK_DOWN:
                        select_rect.y += MOVE_STEPy;
                        break;
                    case SDLK_LEFT:
                        select_rect.x -= MOVE_STEP;
                        break;
                    case SDLK_RIGHT:
                        select_rect.x += MOVE_STEP;
                        break;
                    case SDLK_SPACE:
                        space_pressed = true;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 9, 70, 34, 255);
        SDL_RenderClear(renderer);
        
        // Render images
        for (int i = 0; i < NUM_IMGS; i++) {
            if (img_rectstype[i] == 0) {
                SDL_RenderCopy(renderer, img, NULL, &img_rects[i]); // Render card1
            } else if (img_rectstype[i] == 1) {
                SDL_RenderCopy(renderer, img2, NULL, &img_rects[i]); // Render card2
            }

            if (img_rects[i].x == select_rect.x && img_rects[i].y == select_rect.y) {
                //printf("Match found: x = %d, y = %d\n", img_rects[i].x, img_rects[i].y);
                
                if (space_pressed) {
                    resetAll(img_selected_bool);
                    img_selected_bool[i] = true;
                    break;
                }
            } else {
                if (img_selected_bool[i] && space_pressed && !istherepiece(select_rect.x, select_rect.y, img_rects)) {
                    if (isMovingCorrectly(img_rects[i].x, img_rects[i].y, select_rect.x, select_rect.y) == true) {
                        printf("card posx: %d\n", img_rects[i].x);
                        printf("card posy: %d\n", img_rects[i].y);
                        img_rects[i].x = select_rect.x;
                        img_rects[i].y = select_rect.y;
                        printf("desired card posx: %d\n", img_rects[i].x);
                        printf("desired card posy: %d\n", img_rects[i].y);
                        resetAll(img_selected_bool);
                    }
                }
            }
        }

        space_pressed = false;

        // Render select image
        SDL_RenderCopy(renderer, select, NULL, &select_rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(img);
    SDL_DestroyTexture(img2);
    SDL_DestroyTexture(select);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
