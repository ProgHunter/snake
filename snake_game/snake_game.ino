// ------------------------- SNAKE GAME! ------------------------- //
// Arduino software that work with a joystick as input and a dot-matrix as output
// Personnal project created by Olivier Juteau-Desjardins and Pierre-Yves Vincent-Tremblay
// Date: 20 avril 2019
// --------------------------------------------------------------- //

//errors
#define SIZE_ERROR -1

// Constants related to the pin mapping of the 74595 chip -> dot-matri
const char LATCH_PIN = 8;   //Pin connected to ST_CP(RCLK) of 74HC595
const char CLOCK_PIN = 12;  //Pin connected to SH_CP(SRCLK) of 74HC595
const char DATA_PIN = 11;   //Pin connected to DS(SER) of 74HC595

// Constants related to the pin mapping of the joystick
const char JOY_X_PIN = A0;  // X axis of the joystick
const char JOY_Y_PIN = A1;  // Y axis of the joystick
const char JOY_B_PIN = 7;   // Button of the joystick


uint8_t testGrid[8] = {B01010101,
                       B10101010,
                       B01010101,
                       B10101010,
                       B01010101,
                       B10101010,
                       B01010101,
                       B10101010};


// Difficulty of the game
uint8_t difficulty;
// Grid representing the 8x8 dot-matrix
uint8_t grid[8] = {0};
// Number from 0 to 63 representing the position of the dot to eat
uint8_t random_dot_position;
// A gentle snake to play with
/*
class Snake {
  // Number representing the direction of the next movement
  // 0 == Left, 1 == Down, 2 == Right, 3 == Up
  uint8_t movement_direction;
  // Tab that represent the dots that are occupied by the snake on the grid
  uint8_t position_in_grid[8] = {0};
  // Number from 0 to 63 representing the snake's head position
  uint8_t head;
  // Number from 0 to 63 representing the snake's tail position
  uint8_t tail;

  // Construtor of the snake
  public:
  Snake(uint8_t init_direction, uint8_t init_position) {
    movement_direction = init_direction;
    positionToGrid(&position_in_grid, init_position);
    head = init_position;
    tail = init_position;
  }

  void move_update(){
    
  }
};*/

 
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
 showGrid(testGrid,8);
 delay(1);
}

//---FUNCTIONS---//
// Transform a position number into the correspnding position in the 8x8 grid
void positionToGrid(uint8_t *grid[], uint8_t position_num) {
  
}

//generate a very short pulse on the pin RCLK of both shift register
//to indiquate that the values in memory should be apply to the pins
void applyChangeToShiftRegisterPins(){
  
  digitalWrite(LATCH_PIN,HIGH);
  //the pulse duration should be minimally 20 ns(see 
  //http://www.ti.com/lit/ds/symlink/sn74hc595.pdf p.7)
  //since the arduino uno use a 16 MHz clock we should
  //be ok without any additional delay
  digitalWrite(LATCH_PIN,LOW);
}
int showGrid(uint8_t grid[], size_t sizeOfGrid){

  if(sizeOfGrid > 8){
    return SIZE_ERROR;
  }
  static uint_fast8_t columnSlect = 0;
  //first we send the row value to the first shift register
  //
  //then we send the colum value to the first shift register,
  //this means that the row value is shift to the second shift
  //register by the first shift register
  
  digitalWrite(LATCH_PIN,LOW);
  shiftOut(DATA_PIN,CLOCK_PIN,MSBFIRST,grid[columnSlect]);//send row to the matrix

  //select the rigth column
  shiftOut(DATA_PIN,CLOCK_PIN,MSBFIRST, ~(0x01 << columnSlect));
  applyChangeToShiftRegisterPins();

  columnSlect++;
  if(columnSlect>=sizeOfGrid){
    columnSlect = 0;//roll back
  }
  
  return 0;
}
