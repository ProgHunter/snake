#include <Arduino.h>
#include "Coord.h"

#ifndef BODY_H
class Body {
  public:


  Body(Coord *init_part, Body *init_next_part);

  ~Body();
  Coord *part;
  Body *next_part;
};

#define BODY_H
#endif