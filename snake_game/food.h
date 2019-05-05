#ifndef FOOD_H
#include "const.h"
#include <Arduino.h>
class Food
{
private:
    /* data */
public:
    Food(/* args */);
    ~Food();
    bool Food::set_random_coord(Coord *coord, Grid *grid);
};
#define FOOD_H
#endif