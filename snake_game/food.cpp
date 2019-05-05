#include "Food.h"

Food::Food(/* args */)
{
}

Food::~Food()
{
}


bool Food::set_random_coord(Coord *coord, Grid *grid) {
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
            coord->x = j;
            coord->y = i;
            return true;
            }
            random_number--;
        }
    }
}