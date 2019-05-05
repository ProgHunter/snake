#include "Snake.h"

Snake::Snake(Mvt_dir init_direction, Coord *init_head) {
next_head_dir = init_direction;
head = new Body(init_head, NULL);
body_size = 1;
}

Snake::~Snake() {
    for(;body_size > 0; body_size--) {
        Body *futur_head = NULL;
        if(head->next_part)
            futur_head = head->next_part;
        head->~Body();
        if(futur_head)
            head = futur_head;
    }
}

bool Snake::valid_new_head(Snake *snake) {
    uint8_t head_x = snake->head->part->x;
    uint8_t head_y = snake->head->part->y;
    // Check for out of borders
    switch(snake->next_head_dir) {
        case LEFT:
            if(head_x <= MATRIX_MIN)
                return false;
            head_x--;
            break;
        case DOWN:
            if(head_y >= MATRIX_MAX)
                return false;
            head_y++;
            break;
        case RIGTH:
            if(head_x >= MATRIX_MAX)
                return false;
            head_x++;
            break;
        case UP:
            if(head_y <= MATRIX_MIN)
                return false;
            head_y--;
            break;
        default:
            return false;
    }
    // Check for body
    Body *current_body_part_checked = snake->head;
    // Start at 1 because we skip head, finish at body_size-1 because the last body part wont be there
    for(uint8_t i = 1; i < snake->body_size-1; i++) {
        current_body_part_checked = current_body_part_checked->next_part;
        if(current_body_part_checked->part->x == head_x && current_body_part_checked->part->y == head_y)
            return false;
    }
    return true;
}

void Snake::eat_food(Snake *snake, Food *ze_food) {
    snake->head = new Body(new Coord(ze_food->location->x, ze_food->location->y), snake->head);
    snake->body_size++;
}

uint8_t Snake::move_update(Snake *snake, Food *ze_food) {
    bool food_ate = false;
    if(!valid_new_head(snake))
        return 2;
    // Eat dot ?
    if(snake->head->part->same_pos(snake->head->part, ze_food->location)) {
        snake->eat_dot(snake, ze_food);
        food_eated = true;
    }
    uint8_t head_x = snake->head->part->x;
    uint8_t head_y = snake->head->part->y;
    switch(snake->next_head_dir) {
        case LEFT:
            head_x--;
            break;
        case DOWN:
            head_y++;
            break;
        case RIGTH:
            head_x++;
            break;
        case RIGTH:
            head_y--;
            break;
            // we assume valid_new_head() is checking default for us
    }
    // Create a head according to direction
    snake->head = new Body(new Coord(head_x, head_y), snake->head);
    Body *temp_body = snake->head;
    for(int i = 1; i < snake->body_size-1; i++)
        temp_body = temp_body->next_part;
    // Delete the last part of the snake
    temp_body->next_part->~Body();
    temp_body->next_part = NULL;
    if(food_eated)
        return 1;
    return 0;
}
