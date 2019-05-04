#include "Grid.h"

Grid::Grid(){
    grid[MATRIX_SIZE] = {0};
}

Grid::~Grid() {
    delete[] grid;
}

// Activate a dot on the grid at a position
void Grid::add_dot(Grid *grid, Coord *pos) {
    grid[pos->y] |= (1 << MATRIX_SIZE - pos->x);
}

// Deactivate a dot on the grid at a position
void Grid::delete_dot(Grid *grid, Coord *pos) {
    grid[pos->y] &= ~(1 << MATRIX_SIZE - pos->x);
}

// Reset the grid to 0
void Grid::reset(Grid *grid) {
    for(uint8_t i = 0; i < MATRIX_SIZE; i++)
        grid[i] = 0;
}

// Place the snake and the dot on the grid
void Grid::UPdate_snake_and_dot(Grid *grid, Snake *snake, Coord *random_dot_position){
    grid.reset(grid);

    Body *temp_body = snake->head;
    for(uint8_t i = 0; i < snake->body_size; i++) {
        if(i > 0)
        temp_body = temp_body->next_part;
        grid.add_dot(grid, temp_body->part);
    }

    grid.add_dot(grid, random_dot_position);
}