#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>


#include "survival_function.h"
#include "print.h"
#include "functions.h"


void update(IndividualType& individual) {
   for (int i = 0; i < 2500; ++i) {
        individual[i] = getRandomBool();
    }
}


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


void Picture::show(IndividualType& individual, 
                   CellularAutomaton& automaton, 
                   const int& num_it, 
                   const int& num_per_sec) {
    SDL_Event windowEvent;
    SDL_PollEvent(&windowEvent);

    // IndividualType result = individual;
    // const int mill = 500;
    // if ( SDL_PollEvent( &windowEvent ) ) {
    //     if ( SDL_QUIT == windowEvent.type ) {
    //     }
    // }
    // SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00); // background
    // SDL_RenderClear(ren);
    // SDL_SetRenderDrawColor(ren, 0x55, 0x6B, 0x2F, 0xFF); // grid

    // SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00); // background
    // SDL_RenderClear(ren);
    // SDL_SetRenderDrawColor(ren, 0x55, 0x6B, 0x2F, 0xFF); // grid
    // drawGrid();
    // SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x7F, 0xFF); // individual
    // drawField(result);
    // SDL_RenderPresent(ren);
    // SDL_Delay(500);
    IndividualType test(individual.size());
    update(test);


    for (int i = 0; i <= num_it; ++i) {
        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00); // background
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x55, 0x6B, 0x2F, 0xFF); // grid
        drawGrid();
        SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x7F, 0xFF); // individual
        drawField(test);
        SDL_RenderPresent(ren);
        // automaton.oneStep(individual);
        SDL_Delay(500);
        update(test);
    }
    SDL_Delay(5000);
}


void Picture::drawField(const IndividualType& individual) {
    for (int i = 0; i < field_width; ++i) {
        for (int j = 0; j < field_height; ++j) {
            if (individual[i * field_width + j]) drawRect(i, j);
        }
    }
}


void Picture::drawRect(int x, int y) {
    SDL_Rect rect1 = {BORDER_LINE + x * cellSizeX, 
                      BORDER_LINE + y * cellSizeY, 
                      cellSizeX, cellSizeY};
    SDL_RenderFillRect(ren, &rect1);
}



void Picture::drawGrid() {
    for (int i = 0; i <= field_width; ++i) {
        SDL_RenderDrawLine(ren, BORDER_LINE + i * cellSizeX, BORDER_LINE, 
                                BORDER_LINE + i * cellSizeX, SCREEN_HEIGHT - BORDER_LINE);
    }

    for (int i = 0; i <= field_height; ++i) {
        SDL_RenderDrawLine(ren, BORDER_LINE, BORDER_LINE + i * cellSizeY, 
                                SCREEN_WIDTH - BORDER_LINE, BORDER_LINE + i * cellSizeY);
    }
}


Picture::~Picture() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
