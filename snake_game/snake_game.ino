// ------------------------- SNAKE GAME! ------------------------- //
// Arduino software that work with a joystick as input and a dot-matrix as output
// Personnal project created by Olivier Juteau-Desjardins (@thePro) and Pierre-Yves Vincent-Tremblay (@theNoob)
// Date: 20 avril 2019
// --------------------------------------------------------------- //
#include "Body.h"
#include "Coord.h"
#include "Grid.h"
#include "Snake.h"
#include "const.h"
#include "Food.h"

// Difficulty of the game
uint8_t difficulty;
// The game state
Game_states the_game_state = IN_PROGRESS;
// Flag, restart a game
bool restart_game = false;
// Flag, responsible for game progression speed control
bool game_progress = false;
// The grid of the game
Grid grid = NULL;
// Representing the position of the dot to eat
Food ze_food = NULL;
// The snaaaaaaaake!!
Snake snake = NULL;
// For random number
time_t t;

void setup() {
  setupTimer();
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  pinMode(JOY_X_PIN, INPUT);  // Analog
  pinMode(JOY_Y_PIN, INPUT);  // Analog
  pinMode(JOY_B_PIN, INPUT);  // Digital
  digitalWrite(JOY_B_PIN, HIGH);

  // Initialize random number generator
  srand((unsigned) time(&t));

  grid = Grid();
  snake = Snake(UP, new Coord(4,4));
  ze_food = Food(new Coord(0,0));
  communicate_free_coord_to_set_new_food(grid, ze_food);
}

void loop() {
  // Check flag to restart the game
  if(restart_game)
    new_game(&grid, &snake, &ze_food, &the_game_state);

  if(the_game_state == IN_PROGRESS) {
    // TODO: Check for controler input here
    // Affect the "next_head_dir" member of the snake (joystick) and the "restart_game" flag of the programm (button)

    // Check flag for game progression (speed control)
    if(game_progress) {
      switch(snake.move_update(&snake, &ze_food)) { // TODO: Check the return value to change the_game_state if necessary
        case 1:
          if(!communicate_free_coord_to_set_new_food(grid, ze_food))
          the_game_state = WIN;
          break;
        case 2:
          the_game_state = LOSE;
          break;
      }
      grid.update_snake_and_food(&grid, &snake, ze_food.location);
      game_progress = false;
    }

    // Check flag to redraw the matrix
    if(refresh_screen){
      draw(&grid);
      refresh_screen = false;
    }
  } else if(the_game_state == WIN) {
    // TODO: display winner screen
  } else if(the_game_state == LOSE){
    // TODO: display loser screen
  }
}


//---GENERAL FUNCTIONS---//

// Do all the job to end the present game and set a new one
void new_game(Grod *grid, Snake *snake, Food *ze_food, Game_states *the_game_state) {
  grid->reset(grid);
  snake->~Snake();
  snake = new Snake(up, new Coord(4,4));
  communicate_free_coord_to_set_new_food(grid, ze_food);
  *the_game_state = IN_PROGRESS;
}


// TODO: function for joystick's input :)
Mvt_dir getDirection(){
  int16_t x = analogRead(JOY_X_PIN)-512;
  int16_t y = analogRead(JOY_Y_PIN)-512;

  int16_t absX = abs(x);
  int61_t absY = abs(y);

  if(absX<DEAD_ZONE && absY<DEAD_ZONE){
    return UNKNOWN;
  }else if(absX<absY){
    return (y>0)? DOWN : UP;
  }else{
    return (x>0)? LEFT : RIGTH;
  }
}

// Ask the grid for free spots (coord), and pass them to food as potential new coord
// Return false if no coord available
bool communicate_free_coord_to_set_new_food(Grid *grid, Food *ze_food) {
    Coord free[MATRIX_SIZE];
    uint8_t free_length = grid->get_free_coord(grid, free);

    if(!free_length)
        return false;

    ze_food->set_random_coord(ze_food, free, free_length);
    return true;
}

// Draw the grid on the physical dot-matrix
void draw(Grid *grid) {

  static uint8_t columnSlect = 0;
  //first we send the row value to the first shift register
  //
  //then we send the colum value to the first shift register,
  //this means that the row value is shift to the second shift
  //register by the first shift register

  digitalWrite(LATCH_PIN,LOW);
  shiftOut(DATA_PIN,CLOCK_PIN,MSBFIRST,grid->grid[columnSlect]);//send row to the matrix

  //select the rigth column
  shiftOut(DATA_PIN,CLOCK_PIN,MSBFIRST, ~(0x01 << columnSlect));
  applyChangeToShiftRegisterPins();

  columnSlect++;
  if(columnSlect>=sizeOfGrid){
    columnSlect = 0;//roll back
  }
}

//--TIMER--//
void setupTimer(void){
  //base on https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below

  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

}

//is called when the timer0 generate an interupt
SIGNAL(TIMER0_COMPA_vect) {
  volatile static uint8_t nbInterupt = 0;
  game_progress = true;

  nbInterupt++;

  if(nbInterupt == 10){
    refresh_screen = true;
    nbInterupt = 0;
  }
}
