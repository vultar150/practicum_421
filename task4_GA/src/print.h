#ifndef PRINT_H
#define PRINT_H

const int SCREEN_WIDTH = 670;
const int SCREEN_HEIGHT = 670;
const int BORDER_LINE = 10;



class Picture {
public:
    Picture(const int& field_width=50, const int& field_height=50);

    void show(IndividualType& individual, 
              CellularAutomaton& automaton, 
              const int& num_it, 
              const int& num_per_sec);

    void drawField(const IndividualType& individual);

    void drawRect(int x, int y);

    void drawGrid();

    ~Picture();

private:
    SDL_Window *window;
    SDL_Renderer *ren;
    int cellSizeX;
    int cellSizeY;

    int field_width;
    int field_height;

};


#endif // PRINT_H