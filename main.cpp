    #define SDL_MAIN_HANDLED
    #include <SDL2/SDL.h>
    #include <cmath>
    #include <cstdlib>
    #include <stdlib.h>
    #include <ctime>
    #define WINDOW_WIDTH 600
    #define TARGET_FPS 30
    #define FRAME_DELAY (1000 / TARGET_FPS) // Milliseconds per frame
    int randomgen(int min, int max)
    {
        int range = max-min+1 ;
        return rand()%range + min ;
    }
    // Function to calculate Euclidean distance
    double calculateDistance(double x, double y) {
        return std::sqrt(x * x + y * y);
    }
    void drawPixel(SDL_Renderer *renderer, int x, int y, int pixelSize) {
        // Directly use x and y as the position of the top-left corner of the pixel
        SDL_Rect pixelRect = { x, y, pixelSize, pixelSize };
        SDL_RenderFillRect(renderer, &pixelRect);  // Fill the rectangle
    }
    int main(void) {
        SDL_Event event;
        SDL_Renderer *renderer;
        SDL_Window *window;

        srand(static_cast<unsigned int>(time(0)));
        // Initialize SDL
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
        
        // Set the background to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        int pixelSize = 10;
        
        // lighting (NOT USED RIGHT NOW)
        int lightXpoint = 300;
        int lightYpoint = 300;


        double maxLightDistance = 100;
            

        for (int cx = 0; cx < WINDOW_WIDTH; cx++) {
            for (int cy = 0; cy < WINDOW_WIDTH; cy++) {
                

                double lightXdistance = cx - lightXpoint;
                double lightYdistance = cy - lightYpoint;
                double distance = calculateDistance(lightXdistance, lightYdistance);
                
                //if (distance <= 100) {
                //    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
                //    SDL_RenderDrawPoint(renderer, cx, cy);\
                //}
                
                
                if (distance > maxLightDistance) {
                    distance = maxLightDistance;
                }
                

                int alpha = static_cast<int>((1 - (distance / maxLightDistance)) * 255);
                if (alpha < 0) alpha = 0;  // Ensure alpha doesn't go below 0
                

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha); 


                SDL_RenderDrawPoint(renderer, cx, cy);
                
            }
        }



        //random color test
        /*
        for (int mx = 0; mx < 100; mx++) {
            int alpha = randomgen(50, 255);
            SDL_SetRenderDrawColor(renderer, 255, 60, 0, alpha); 
            
            drawPixel(renderer, mx, 0, 5);
        }
        */


        SDL_RenderPresent(renderer);
        int origxof;
        int xOffset;
        int yOffset;
        int randomOffset;
        bool running = true;
        int frameStart, frameTime;

        while (running) {

            //frameStart = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            /*
            for (int mx = 0; mx < 800; mx++) {
                int alpha = randomgen(50, 255); 
                SDL_SetRenderDrawColor(renderer, 255, randomgen(0, 255), 0, 255);
                xOffset = randomgen(0, 50);
                origxof = xOffset;
                yOffset = randomgen(0, 200);
                randomOffset = randomgen(0, 10);
                xOffset -= abs(yOffset / 4 - randomOffset);
                if (xOffset < 0) {
                    xOffset = 0;
                }
                else {
                    SDL_RenderDrawPoint(renderer, 300 - xOffset, 300 - yOffset);
                    SDL_SetRenderDrawColor(renderer, 255, randomgen(0, 120), 0, 255);
                    xOffset = 50 - xOffset;
                    SDL_RenderDrawPoint(renderer, 350 - xOffset, 300 - yOffset);
                    
                }
            }
            */
           /*
            for (int mx = 0; mx < 1200; mx++) {
                int alpha = randomgen(50, 255); 
                
                xOffset = randomgen(0, 50);
                origxof = xOffset;
                yOffset = randomgen(0, 200);
                randomOffset = randomgen(0, 15);
                xOffset -= abs(yOffset / 4 - randomOffset);
                yOffset -= randomOffset;
                if (xOffset < 0) {
                    xOffset = 0;
                }
                else {
                    //int fullyellow = randomgen(0, 180 + 2 * (50 - xOffset));
                    int fullyellow = randomgen(0, 180);
                    if (xOffset <= 15 - yOffset / 14 && yOffset <= 80 && yOffset >= 25) {
                        fullyellow = randomgen(155 - yOffset / 20, 255);
                    }
                    SDL_SetRenderDrawColor(renderer, 255, fullyellow, 0, 255);
                    drawPixel(renderer, 299 - xOffset, 301 - yOffset, randomgen(2, 3));
                    SDL_SetRenderDrawColor(renderer, 255, fullyellow, 0, 255);
                    xOffset = 50 - xOffset;
                    drawPixel(renderer, 350 - xOffset, 300 - yOffset, randomgen(2, 3));
                    
                }
            }
            drawPixel(renderer, 60, 60, 5);

            SDL_RenderPresent(renderer);

            frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < FRAME_DELAY) {
                SDL_Delay(FRAME_DELAY - frameTime); // Delay to maintain 30 FPS
            }
            */
        }

        // Clean up and quit
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_SUCCESS;
    }
