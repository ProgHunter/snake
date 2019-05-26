#ifndef FOOD_H
#include "const.h"
#include <Arduino.h>
#include "Grid.h"
class Food
{
public:
    Coord *location;

    Food(Coord *init_pos);
    ~Food();

    // Set location of food to a new Coord randomly choose in the "free" tab choices
    // It assume free_length is > 0 and is corresponding to the number of Coord initialized in the "free" tab
    // (it free the memory of coords in "free")
    void set_random_coord(Food *ze_food, Coord[] free, uint8_t free_length);
};
#define FOOD_H
#endif