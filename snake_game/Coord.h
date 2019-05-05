// X and Y coordinates in the 8x8 matrix
#include <Arduino.h>

#ifndef COORD_H

// X and Y coordinates in the 8x8 matrix
class Coord {
  public:
  uint8_t x;
  uint8_t y;

  Coord(uint8_t init_x, uint8_t init_y);


  // Return if both coords are the same position
  bool same_pos(Coord *pos1, Coord *pos2);
};
#define COORD_H
#endif
