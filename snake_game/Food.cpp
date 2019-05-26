#include "Food.h"

Food::Food(Coord *init_pos)
{
    location = init_pos;
}

Food::~Food()
{
    location->~Coord();
}

void set_random_coord(Food *ze_food, Coord[] free, uint8_t free_length) {
    int random_number = 0;
    random_number = rand() % free_length;
    ze_food->location = free[random_number];

    for(uint8_t i = free_length; i > 0; i--)
        if(i-1 != random_number)  // Destroy the tab of coord except the one we given to food as new location
            free[i-1]->~Coord();
}