// ------------------------- SNAKE GAME! ------------------------- //
// Arduino software that work with a joystick as input and a dot-matrix as output
// Personnal project created by Olivier Juteau-Desjardins (@thePro) and Pierre-Yves Vincent-Tremblay (@theNoob)
// Date: 20 avril 2019
// --------------------------------------------------------------- //

// Constant related to the square dot-matrix size
#define MATRIX_MIN 0
#define MATRIX_MAX 7
#define MATRIX_SIZE 8

// Constants related to the pin mapping of the 74595 chip -> dot-matrix
#define LATCH_PIN 8   //Pin connected to ST_CP(RCLK) of 74HC595
#define CLOCK_PIN 12  //Pin connected to SH_CP(SRCLK) of 74HC595
#define DATA_PIN 11   //Pin connected to DS(SER) of 74HC595

// Constants related to the pin mapping of the joystick
#define JOY_X_PIN A0  // X axis of the joystick
#define JOY_Y_PIN A1  // Y axis of the joystick
#define JOY_B_PIN 7   // Button of the joystick

// Dirrection of a movement
enum mvt_dir { unknown, left, down, right, up };

// Game states..
enum game_states { in_progress, win, lose };

// Game difficulties
enum game_difficulties { ease, normal, hard };

// X and Y coordinates in the 8x8 matrix
class Coord {
  public:
  uint8_t x;
  uint8_t y;

  Coord(uint8_t init_x, uint8_t init_y) {
    x = init_x;
    y = init_y;
  }

  // Use to set pos of the "random_dot_position" of the game
  // Return false if no place left
  bool set_random_coord(Coord *coord, Grid *grid) {
    int random_number = 0;
    uint8_t free_pos = 0;
    // Count avable pos for the random dot
    for(uint8_t i = 0; i < MATRIX_SIZE; i++) {
      for(uint8_t j = 0 ; j < MATRIX_SIZE; j++) {
        if(*grid[i] & (1 << MATRIX_SIZE - j) != 0)
          free_pos++;
      }
    }
    if(!free_pos)
      return false;
    random_number = rand() % free_pos;
    // Count back free pos to place the dot
    for(uint8_t i = 0; i < MATRIX_SIZE; i++) {
      for(uint8_t j = 0 ; j < MATRIX_SIZE; j++) {
        if(*grid[i] & (1 << MATRIX_SIZE - j) != 0)
          if(!random_number) {
            coord->x = j;
            coord->y = i;
            return true;
          }
          random_number--;
      }
    }
  }

  // Return if both coords are the same position
  bool same_pos(Coord *pos1, Coord *pos2) {
    if(pos1->x == pos2->x && pos1->y == pos2->y)
      return true;
    return false;
  }
};

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

  Grid() {
    grid[MATRIX_SIZE] = {0};
  }

  ~Grid() {
    delete[] grid;
  }
  
  // Activate a dot on the grid at a position
  void add_dot(Grid *grid, Coord *pos) {
    grid[pos->y] |= (1 << MATRIX_SIZE - pos->x);
  }

  // Deactivate a dot on the grid at a position
  void delete_dot(Grid *grid, Coord *pos) {
    grid[pos->y] &= ~(1 << MATRIX_SIZE - pos->x);
  }

  // Reset the grid to 0
  void reset(Grid *grid) {
    for(uint8_t i = 0; i < MATRIX_SIZE; i++)
      grid[i] = 0;
  }

  // Place the snake and the dot on the grid
  void update_snake_and_dot(Grid *grid, Snake *snake, Coord *random_dot_position){
    grid.reset(grid);

    Body *temp_body = snake->head;
    for(uint8_t i = 0; i < snake->body_size; i++) {
       if(i > 0)
        temp_body = temp_body->next_part;
       grid.add_dot(grid, temp_body->part);
    }
   
    grid.add_dot(grid, random_dot_position);
  }
};

class Body {
  public:
  Coord *part;
  Body *next_part;

  Body(Coord *init_part, Body *init_next_part) {
    part = init_part;
    nest_part = init_next_part;
  }

  ~Body() {
    if(part)
      part->~Coord();
  }
}

// A gentle snake to play with
// Implemented as a FIFO
// Instead, a Coord is created for each body part and the x/y values are updated in them
class Snake {
  public:
  enum mvt_dir next_head_dir;
  uint8_t body_size;
  // The snake's position
  // The head of the snake is @body_size-1, 
  // since we add a new head every time we eat the random_dot
  Body *head;

  Snake(enum mvt_dir init_direction, Coord *init_head) {
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
      case left:
        if(head_x <= MATRIX_MIN)
          return false;
        head_x--;
        break;
      case down:
        if(head_y >= MATRIX_MAX)
          return false;
        head_y++;
        break;
      case right:
        if(head_x >= MATRIX_MAX)
          return false;
        head_x++;
        break;
      case up:
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
  
  // Move the snake @next_head_dir and update the grid
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
      case left:
        head_x--;
        break;
      case down:
        head_y++;
        break;
      case right:
        head_x++;
        break;
      case right:
        head_y--;
        break;
      // we assume valid_new_head() is cheking default for us
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

// Difficulty of the game
uint8_t difficulty;
// The game state
enum game_states the_game_state = in_progress;
// Flag, restart a game
bool restart_game = false;
// Flag, responsible for game progression speed control
bool game_progress = false;
// The grid of the game
Grid grid = NULL;
// Representing the position of the dot to eat
Coord random_dot_position = NULL;
// The snaaaaaaaake!!
Snake snake = NULL;
// For random number
time_t t;

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  pinMode(JOY_X_PIN, INPUT);  // Analog
  pinMode(JOY_Y_PIN, INPUT);  // Analog
  pinMode(JOY_B_PIN, INPUT);  // Digital
  digitalWrite(JOY_B_PIN, HIGH);

  // Intializes random number generator
  srand((unsigned) time(&t));
   
  grid = Grid();
  snake = Snake(up, &(Coord(4,4))); //  ?
  random_dot_position = Coord(0,0);
  random_dot_position.set_random_coord(&random_dot_position, &grid);
}

void loop() {
  // Check flag to restart the game
  if(restart_game)
    new_game(&grid, &snake, &random_dot_position, &the_game_state);
    
  if(the_game_state == in_progress) {
    // TODO: Check for controler input here
    // Affect the "next_head_dir" member of the snake (joystick) and the "restart_game" flag of the programm (button)

    // Check flag for game progression (speed control)
    if(game_progress) {
      switch(snake.move_update(&snake, &random_dot_position)) { // TODO: Check the return value to change the_game_state if necessary
        case 1:
          if(random_dot_position.set_random_coord(&random_dot_position, &grid))
          the_game_state = win;
          break;
        case 2:
          the_game_state = lose;
          break;
      }
      grid.update_snake_and_dot(&grid, &snake, &random_dot_position);
    }

    // Check flag to redraw the matrix
    if(refresh_screen)
      draw(&grid);
  } else if(the_game_state == win) {
    // TODO: display winner screen
  } else if(the_game_state == lose){
    // TODO: display loser screen
  }
}


//---GENERAL FUNCTIONS---//

// Do all the job to end the present game and set a new one
void new_game(Grod *grid, Snake *snake, Coord *random_dot_position, enum game_states *the_game_state) {
  grid->reset(grid);
  snake->~Snake();
  snake = new Snake(up, new Coord(4,4));
  random_dot_position->set_random_coord(random_dot_position, grid);
  *the_game_state = in_progress;
}

// TODO: function for joystick's input :)

// Draw the grid on the physical dot-matrix
void draw(Grid *grid) {
  // TODO: Send grid data to dot-matrix here
}
