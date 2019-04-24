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
enum mvt_dir { left, down, right, up, unknown };

// X and Y coordinates in the 8x8 matrix
class Coord {
  public:
  uint8_t x;
  uint8_t y;

  Coord(uint8_t init_x, uint8_t init_y) {
    x = init_x;
    y = init_y;
  }

  ~Coord() {
    delete &x;
    delete &y;
  }

  bool set_coord_xy(uint8_t new_x, uint8_t new_y) {
      if(   new_x < MATRIX_MIN
         || new_x > MATRIX_MAX
         || new_y < MATRIX_MIN
         || new_y > MATRIX_MAX)
        return true;
      x = new_x;
      y = new_y;
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
  void add_dot(Coord *pos) {
    grid[pos->y] |= (1 << MATRIX_SIZE - pos->x);
  }

  // Deactivate a dot on the grid at a position
  void delete_dot(Coord *pos) {
    grid[pos->y] &= ~(1 << MATRIX_SIZE - pos->x);
  }

  void reset() {
    for(uint8_t i = 0; i < MATRIX_SIZE; i++)
      grid[i] = 0;
  }
};

// A gentle snake to play with
// Implemented as a static tab of Coords instead of a dynamic FIFO to minimize memory allocation/deallocation
// Instead, a Coord is created for each body part and the x/y values are updated in them
class Snake {
  public:
  enum mvt_dir next_head_dir;
  uint8_t body_size;
  // The snake's position
  // The head of the snake is @body_size-1, 
  // since we add a new head every time we eat the random_dot
  Coord body[MATRIX_SIZE * MATRIX_SIZE];  // Init to NULL ?

  Snake(enum mvt_dir init_direction, Coord *init_head) {
    next_head_dir = init_direction;
    body[0] = &init_head;
    body_size = 1;
  }

  ~Snake() {
    delete &next_head_dir;
    for(;body_size > 0; body_size--)
      body[body_size].~Coord();  // legit ?
    delete[] body;
    delete &body_size;
  }

  // Return true if the next position of the head is valid (not out-grid, not on itself) 
  bool valid_new_head() {
    uint8_t head_x = body[body_size-1].x;
    uint8_t head_y = body[body_size-1].y;
    // Check for out of borders
    switch(next_head_dir) {
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
    // Start at [1] because [0] will be at the present position of [1]
    for(uint8_t i = 1; i < body_size-1; i++)
      if(body[i].x == head_x && body[i].y == head_y)
        return false;
    return true;
  }
  
  // Move the snake @next_head_dir
  // Return true if move
  // To minimize the use of constructors/destructors, the body'coords values will be replaced instead of deleting/creating one.
  bool move_update() {
    if(!valid_new_head())
      return false
    // Move all the body except the head
    for(uint8_t i = 0; i < body_size-1; i++) {
      body[i+1].x = body[i].x;
      body[i+1].y = body[i].y;
    }
    // Move head according to direction
    switch(next_head_dir) {
      case left:
        body[body_size-1].x--;
        break;
      case down:
        body[body_size-1].y++;
        break;
      case right:
        body[body_size-1].x++;
        break;
      case right:
        body[body_size-1].y--;
        break;
      // we assume valid_new_head() is cheking default for us
    }
  }
};

// Difficulty of the game
uint8_t difficulty;
// Representing the position of the dot to eat
Coord random_dot_position = NULL;

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  pinMode(JOY_X_PIN, INPUT);  // Analog
  pinMode(JOY_Y_PIN, INPUT);  // Analog
  pinMode(JOY_B_PIN, INPUT);  // Digital
  digitalWrite(JOY_B_PIN, HIGH);

  // TODO: Create a instance of a class "Game" that has a grid, a snake, a dot and a difficulty
}

void loop() {
  // put your main code here, to run repeatedly:
  // e.g analogRead(JOY_X_PIN);

  // When starting a new game, cal the reinit_game() on the one created at setup()
}


//---FUNCTIONS---//

// Draw snake and random_dot on grid
// This fonction shoud be in the Game class once created
void draw() {
  grid.reset();
  
  for(uint8_t i = 0; i < snake.body_size; i++)
    grid.add_dot(snake.body[i]);
    
  grid.add_dot(random_dot_position);
}
