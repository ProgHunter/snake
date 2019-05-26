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

// TODO: only pass coords of the object snake
void Grid::update_snake_and_food(Grid *grid, Snake *snake, Coord *ze_food_location){
    grid.reset(grid);

    Body *temp_body = snake->head;
    for(uint8_t i = 0; i < snake->body_size; i++) {
        if(i > 0)
        temp_body = temp_body->next_part;
        grid.add_dot(grid, temp_body->part);
    }

    grid.add_dot(grid, ze_food_location);
}

// Add free coord to "free" and return the number of free positions.
uint8_t get_free_coord(Grid *grid, Coord[] free) {
    uint8_t free_length = 0;
    for(uint8_t i = 0; i < MATRIX_SIZE; i++) {
        for(uint8_t j = 0 ; j < MATRIX_SIZE; j++) {
            if(*grid[i] & (1 << MATRIX_SIZE - j) != 0) {
                free[free_length] = new Coord(j, i);  // TODO: Check if X, Y not swapped (i, j correlation)
                free_length++;
            }
        }
    }
    return free_length;
}
