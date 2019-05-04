#include "Snake.h"

// A gentle snake to play with
// Implemented as a FIFO
class Snake {
public:
    Mvt_dir next_head_dir;
    uint8_t body_size;
    // The snake's head position + next
    // We add a new head every time we eat the random_dot or move
    Body *head;

    Snake(Mvt_dir init_direction, Coord *init_head) {
        next_head_dir = init_direction;
        head = new Body(init_head, NULL);
        body_size = 1;
    }

    ~Snake() {
        for(;body_size > 0; body_size--) {
            Body *futur_head = NULL;
            if(head->next_part)
                futur_head = head->next_part;
            head->~Body();
            if(futur_head)
                head = futur_head;
        }
    }

    // Return true if the next position of the head is valid (not out-grid, not on itself)
    bool valid_new_head(Snake *snake) {
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

    // Eat dot
    void eat_dot(Snake *snake, Coord *random_dot_position) {
        snake->head = new Body(new Coord(random_dot_position->x, random_dot_position->y), snake->head);
        snake->body_size++;
    }

    // Move the snake @next_head_dir and UPdate the grid
    // Return 0 if move, 1 if move and dot eated, 2 if game loss
    // To minimize the use of constructors/destructors, the body'coords values will be replaced instead of deleting/creating one.
    uint8_t move_update(Snake *snake, Coord *random_dot_position) {
        bool dot_eated = false;
        if(!valid_new_head(snake))
            return 2;

        // Eat dot ?
        if(snake->head->part->same_pos(snake->head->part, random_dot_position)) {
            snake->eat_dot(snake, random_dot_position);
            dot_eated = true;
        }
        uint8_t head_x = snake->head->part->x;
        uint8_t head_y = snake->head->part->y;
        switch(snake->next_head_dir) {
            case LEFT:
                snake->body[body_size-1].x--;
                break;
            case DOWN:
                snake->body[body_size-1].y++;
                break;
            case RIGTH:
                snake->body[body_size-1].x++;
                break;
            case RIGTH:
                snake->body[body_size-1].y--;
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
        if(dot_eated)
            return 1;
        return 0;
    }
};
