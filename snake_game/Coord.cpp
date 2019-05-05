#include "Coord.h"

Coord::Coord(uint8_t init_x, uint8_t init_y) {
    x = init_x;
    y = init_y;
}

  // Return if both coords are the same position
  bool Coord::same_pos(Coord *pos1, Coord *pos2) {
    if(pos1->x == pos2->x && pos1->y == pos2->y)
        return true;
    return false;
  }
