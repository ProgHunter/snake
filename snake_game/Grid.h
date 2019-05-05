#include <Arduino.h>

#ifndef GRID_H
#include "Coord.h"
// Grid representing the 8x8 dot-matrix
//       x =
// y = 0\0 1 2 3 ...
//     1
//     2
//     ...        7\7
//
class Grid {
  public:
  uint8_t grid[];

  Grid();

  ~Grid();

  // Activate a dot on the grid at a position
  void add_dot(Grid *grid, Coord *pos);

  // Deactivate a dot on the grid at a position
  void delete_dot(Grid *grid, Coord *pos);

  // Reset the grid to 0
  void reset(Grid *grid);

  // Place the snake and the dot on the grid
  void UPdate_snake_and_dot(Grid *grid, Snake *snake, Coord *random_dot_position);
#define GRID_H
#endif
