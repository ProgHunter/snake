#include <Arduino.h>
#include "Coord.h"

#ifndef COORD_H
class Body {
  public:
  Coord *part;
  Body *next_part;

  Body(Coord *init_part, Body *init_next_part);

  ~Body();
};

#define COORD_H
#endif