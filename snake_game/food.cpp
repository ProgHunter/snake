#include "Food.h"

Food::Food(Coord *init_pos)
{
    location = init_pos;
}

Food::~Food()
{
    location->~Coord();
}

bool Food::set_random_coord(Food *ze_food, Grid *grid) {
    int random_number = 0;
    uint8_t free_pos = 0;
    // Count avable pos for the random dot
    for(uint8_t i = 0; i < MATRIX_SIZE; i++) {
        for(uint8_t j = 0 ; j < MATRIX_SIZE; j++) {
        if(*grid[i] & (1 << MATRIX_SIZE - j) != 0)
            free_pos++;
        }
    }
    if(!free_pos)
        return false;
    random_number = rand() % free_pos;
    // Count back free pos to place the dot
    for(uint8_t i = 0; i < MATRIX_SIZE; i++) {
        for(uint8_t j = 0 ; j < MATRIX_SIZE; j++) {
        if(*grid[i] & (1 << MATRIX_SIZE - j) != 0)
            if(!random_number) {
            ze_food->location->x = j;
            ze_food->localisation->y = i;
            return true;
            }
            random_number--;
        }
    }
}