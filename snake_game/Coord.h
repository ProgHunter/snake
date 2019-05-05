// X and Y coordinates in the 8x8 matrix
#include <Arduino.h>
#ifndef COORD_H

class Coord {
  public:
  uint8_t x;
  uint8_t y;

  Coord(uint8_t init_x, uint8_t init_y);

  // Use to set pos of the "random_dot_position" of the game
  // Return false if no place LEFT
  bool set_random_coord(Coord *coord, Grid *grid);

  // Return if both coords are the same position
  bool same_pos(Coord *pos1, Coord *pos2);
};
#define COORD_H
#endif 