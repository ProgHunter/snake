#include "Coord.h"
Coord::Coord(uint8_t init_x, uint8_t init_y) {
    x = init_x;
    y = init_y;
}

bool Coord::set_random_coord(Coord *coord, Grid *grid) {
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

bool Coord::same_pos(Coord *pos1, Coord *pos2) {
    if(pos1->x == pos2->x && pos1->y == pos2->y)
        return true;
    return false;
}
