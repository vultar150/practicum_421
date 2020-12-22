#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>


#include "survival_function.h"
#include "print.h"



Picture::Picture(const int& field_width, const int& field_height) :
            field_width(field_width),
            field_height(field_height)
{
    SDL_Init( SDL_INIT_EVERYTHING );
    window = SDL_CreateWindow("Примитивы", 
                              SDL_WINDOWPOS_UNDEFINED, 
                              SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, 
                              SDL_WINDOW_SHOWN);
    if ( NULL == window ) {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        exit(1);
    }

    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        exit(1);
    }
    cellSizeX = (SCREEN_WIDTH - 2 * BORDER_LINE) / field_width;
    cellSizeY = (SCREEN_HEIGHT - 2 * BORDER_LINE) / field_height;
}


void Picture::show(const IndividualType& individual,  
                   const int& num_it, 
                   const int& num_per_sec) {
    SDL_Event windowEvent;
    SDL_PollEvent(&windowEvent);


    bool isquit = false;
    SDL_Event event;

    double delay = 1000 / num_per_sec;

    int size = std::sqrt(individual.size());
    IndividualType result = individual;
    printIndividual(result, size);
    std::cout << std::endl;
    int i = 0;

    while (!isquit) {
        if (SDL_PollEvent( & event)) {
            if (event.type == SDL_QUIT) {
                isquit = true;
            }
        }
        if (i <= num_it) {
            SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00); // background
            SDL_RenderClear(ren);
            SDL_SetRenderDrawColor(ren, 0x55, 0x6B, 0x2F, 0xFF); // grid
            drawGrid();
            SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x7F, 0xFF); // individual

            printIndividual(result, size);
            std::cout << std::endl;

            drawField(result);

            SDL_RenderPresent(ren);
            int counts[size * size];
            for (int i = 0; i < size * size; ++i) counts[i] = 0;

            for(int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    int dx_up = i > 0 ? -1 : 0;
                    int dx_down = i < size - 1 ? 1 : 0;

                    int dy_left = j > 0 ? -1 : 0;
                    int dy_right = j < size - 1 ? 1 : 0;

                    for (int k = i + dx_up; k <= i + dx_down; ++k) {
                        for (int m = j + dy_left; m <= j + dy_right; ++m) {
                            if (result[k * size + m]) {
                                ++counts[i * size + j];
                            }
                        }
                    }
                    if (result[i * size + j]) --counts[i * size + j];
                }
            }

            for(int i = 0; i < size * size; ++i) {
                if (not result[i] and counts[i] == 3) {
                    result[i] = true;
                }
                else if (result[i] and (counts[i] < 2 or counts[i] > 3)) {
                    result[i] = false;
                }
            }
            ++i;
            SDL_Delay(delay);
        }
    }
}


void Picture::drawField(const IndividualType& result) {
    for (int i = 0; i < field_width; ++i) {
        for (int j = 0; j < field_height; ++j) {
            if (result[i * field_width + j]) drawRect(i, j);
        }
    }
}


void Picture::drawRect(int x, int y) {
    SDL_Rect rect1 = {BORDER_LINE + y * cellSizeX, 
                      BORDER_LINE + x * cellSizeY, 
                      cellSizeX, cellSizeY};
    SDL_RenderFillRect(ren, &rect1);
}



void Picture::drawGrid() {
    for (int i = 0; i <= field_width; ++i) {
        SDL_RenderDrawLine(ren, BORDER_LINE + i * cellSizeX, BORDER_LINE, 
                                BORDER_LINE + i * cellSizeX, 
                                SCREEN_HEIGHT - BORDER_LINE);
    }

    for (int i = 0; i <= field_height; ++i) {
        SDL_RenderDrawLine(ren, BORDER_LINE, BORDER_LINE + i * cellSizeY, 
                                SCREEN_WIDTH - BORDER_LINE, 
                                BORDER_LINE + i * cellSizeY);
    }
}


Picture::~Picture() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
