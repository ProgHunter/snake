#ifndef FOOD_H
#include "const.h"
#include <Arduino.h>
class Food
{
private:
    Coord *location;
public:
    Food(Coord *init_pos);
    ~Food();
    bool Food::set_random_coord(Food *ze_food, Grid *grid);
};
#define FOOD_H
#endif