// ------------------------- SNAKE GAME! ------------------------- //
// Arduino software that work with a joystick as input and a dot-matrix as output
// Personnal project created by Olivier Juteau-Desjardins and Pierre-Yves Vincent-Tremblay
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

// Difficulty of the game
uint8_t difficulty;
// Representing the position of the dot to eat
Coord random_dot_position;
// Dirrection of a moovement
enum mvt_dir { left, down, right, up, unknown };

// Grid representing the 8x8 dot-matrix
//       x =
// y = 0\0 1 2 3 ...
//     1
//     2
//     ...        7\7
//
class Grid {
  uint8_t grid[];

  public:
  Grid() {
    grid[MATRIX_SIZE] = {0};
  }

  ~Grid() {
    delete[] grid;
  }
  
  // Activate a dot on the grid at a position
  void add_dot(Coord *pos) {
    grid[pos->y] |= (1 << pos->x);
  }

  // Deactivate a dot on the grid at a position
  void delete_dot(Coord *pos) {
    grid[pos->y] &= ~(1 << pos->x);
  }
};

// X and Y coordinates in the 8x8 matrix
class Coord {
  uint8_t x;
  uint8_t y;

  public:
  Coord(uint8_t init_x, uint8_t init_y) {
    x = init_x;
    y = init_y;
  }

  ~Coord() {
    delete x;
    delete y;
  }

  bool setCoord_XY(uint8_t new_x, uint8_t new_y) {
      if(   new_x < MATRIX_MIN
         || new_x > MATRIX_MAX
         || new_y < MATRIX_MIN
         || new_y > MATRIX_MAX)
        return true;
      x = new_x;
      y = new_y;
      return false;
  }
  
  bool moove(enum mvt_dir moovement_direction) {
    bool error = false;
    switch(moovement_direction) {
      case left:
        if(x <= MATRIX_MIN)
          error = true;
        else
          x--;
        break;
      case down:
        if(y >= MATRIX_MAX)
          error = true;
        else
          y++;
        break;
      case right:
        if(x >= MATRIX_MAX)
          error = true;
        else
          x++;
        break;
      case up:
        if(y <= MATRIX_MIN)
          error = true;
        else
          y--;
        break;
      default:
        error = true;
    }
    return error;
  }
};

// A part of the snake
Class Body {
  Coord *part;
  Coord *next;

  public:
  Body(Coord *init_part) {
    part = init_head;
    next = NULL;
  }

  ~Body() {
    if(part != NULL)
      part.~Coord();
    if(next != NULL)
      next.~Coord();
  }

  void addNext(Coord *new_next) {
    next = new_next;
  }
};

// A gentle snake to play with
// Implemented as a FIFO (for the moment)
class Snake {
  // Tab that represent the dots that are occupied by the snake on the grid
  // Keeped in sync with the body position when using the snake's functions
  Grid positions;
  enum mouvement_direction next_head_dir;
  uint_8 body_size;
  // The snake's head position and next body part
  Body *head;

  // Construtor of the snake
  public:
  Snake(enum mouvement_direction init_direction, Body *init_head) {
    next_head_dir = init_direction;
    head = init_head;
    body_size = 1;
    &positions = new Grid();
    positions.add_dot(head->part);
  }

  ~Snake() {
    delete next_head_dir;
    delete[] position_in_grid;
    delete body_size;
    if(*head != NULL)
      head->~Coord();  // legit ?
  }
  
  bool move_update(){
    // - TODO: FIFO
    // - Note, cannot go to head->next->part coord if != NULL
    // - Change for new head (set next), then erase the "next" of body at body_size-1
  }
};

 
void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  pinMode(JOY_X_PIN, INPUT);  // Analog
  pinMode(JOY_Y_PIN, INPUT);  // Analog
  pinMode(JOY_B_PIN, INPUT);  // Digital
  digitalWrite(JOY_B_PIN, HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:
//analogRead(JOY_X_PIN);
}

//---FUNCTIONS---//
