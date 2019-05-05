// A gentle snake to play with
// Implemented as a FIFO
#include <Arduino.h>
#pragma once
#include "const.h"
#include "Body.h"
class Snake {
public:
    Mvt_dir next_head_dir;
    uint8_t body_size;
    // The snake's position
    // The head of the snake is @body_size-1,
    // since we add a new head every time we eat the random_dot
    Body *head;

    Snake(Mvt_dir init_direction, Coord *init_head);

    ~Snake();

    // Return true if the next position of the head is valid (not out-grid, not on itself)
    bool valid_new_head(Snake *snake);

    // Eat dot
    void eat_dot(Snake *snake, Coord *random_dot_position);

    // Move the snake @next_head_dir and update the grid
    // Return 0 if move, 1 if move and dot ate, 2 if game loss
    // To minimize the use of constructors/destructors, the body'coords values will be replaced instead of deleting/creating one.
    uint8_t move_update(Snake *snake, Coord *random_dot_position);
};