#include <Arduino.h>
#include "Coord.h"

#ifndef BODY_H
class Body {
public:
    Coord *part;
    Body *next_part;

    Body(Coord *init_part, Body *init_next_part);
    ~Body();
};

#define BODY_H
#endif