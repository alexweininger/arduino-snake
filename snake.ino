#include <RGBmatrixPanel.h>
#include <gamma.h>
#include <math.h>
#include <Fonts/Picopixel.h>
#define xaxis A4
#define yaxis A5
#define btn1  A8

#define CLK 11
#define LAT 10
#define OE 9
#define A A0
#define B A1
#define C A2
#define D A3

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
int x = 0;
int y = 0;
int joystickY;
int joystickX;

int headPositionX = 0;
int headPositionY = 0;

byte snakeDirection = 0;


int snake[100][2];
int snakeTmp[100][2];
int snakeLength = 1;
int gameBoard[9][20];

long randNumber;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);


void setup() {

  matrix.begin();
  Serial.begin(9600);

  pinMode(xaxis, INPUT);
  pinMode(yaxis, INPUT);
  pinMode(btn1, INPUT);

  // Set the screen to be in portrait mode.
  matrix.setRotation(1);

  // Set the font to a super tiny font.
  matrix.setFont(&Picopixel);

  for (int i = 0; i < 100; i++) {
    snake[i][0] = 0;
    snake[i][1] = 0;
  }

  snake[0][0] = 1;
  snake[0][1] = 2;

  for (int i = 0; i < 9; i++)
  {
    for ( int j = 0; j < 20; j++)
    {
      gameBoard[i][j] = 0;
    }
  }

  //top lines
  matrix.drawLine(0, 0, 31, 0, RED);
  matrix.drawLine(0, 1, 31, 1, RED);
  //bottom lines
  matrix.drawLine(0, 62, 31, 62, RED);
  matrix.drawLine(0, 63, 31, 63, RED);
  //left line
  matrix.drawLine(0, 0, 0, 63, RED);
  //right line
  matrix.drawLine(31, 0, 31, 63, RED);

  randomSeed(analogRead(10));

  createApple();

}

void createApple()
{
  int randX = random(9);
  int randY = random(20);
  Serial.println(randX);
  Serial.println(randY);

  gameBoard[randX][randY] = 2;

  matrix.fillRect((randX * 3) + 1, (randY * 3) + 2, 3, 3,  RED);
}

void loop() {
  delay(200);

  // Get the x and y joystick values.
  joystickY = map(analogRead(yaxis), 0, 1023, 100, -100);
  joystickX = map(analogRead(xaxis), 0, 1023, 100, -100);

  // right
  if (joystickX >= 30) {
    snakeDirection = 0;
  }

  // down
  if (joystickY <= -30) {
    snakeDirection = 1;
  }

  // up
  if (joystickY >= 30) {
    snakeDirection = 2;
  }

  // left
  if (joystickX <= -30) {
    snakeDirection = 3;
  }

  delay(200);

  moveSnake();

  for (int i = 0; i < 100; i++) {
    if (i < snakeLength) {
      matrix.fillRect(snake[i][0], snake[i][1], 3, 3,  WHITE);
      SetGameBoard(snake[i][0],  snake[i][1], 1);
    }
    else if (i == snakeLength) {
      matrix.fillRect(snake[i][0], snake[i][1], 3, 3,  GREEN);
      SetGameBoard(snake[i][0],  snake[i][1], 0);
    } else {
      Serial.println("here");
      matrix.fillRect(snake[i][0], snake[i][1], 3, 3,  BLACK);
    }
  }
}

void moveSnake() {

  int increment = 3;
  int head[2] = {snake[0][0], snake[0][1]};

  switch (snakeDirection) {
    case 0: // right
      head[0] = head[0] + increment;
      if (head[0] > 28)
      {
        head[0] = 0;
        //return;
      }
      break;

    case 1: // down
      head[1] = head[1] + increment;
      if (head[1] > 59)
      {
        head[1] = 2;
        //return;
      }
      break;
    case 2: // up
      head[1] = head[1] - increment;
      if (head[1] < 2)
      {
        head[1] = 59;
        //return;
      }
      break;
    case 3: // left
      head[0] = head[0] - increment;
      if (head[0] < 1)
      {
        head[0] = 28;
        //return;
      }
  }

  if (snakeLength == 1) {
    snake[1][0] = snake[0][0];
    snake[1][1] = snake[0][1];
  } else {
    // shifting the segments of the snake in the array
    for (int i = snakeLength - 1; i > 0; i--) {
      snake[i + 1][0] = snake[i][0];
      snake[i + 1][1] = snake[i][1];
    }
  }

  snake[0][0] = head[0];
  snake[0][1] = head[1];

  if (gameBoard[snake[0][0] / 3][snake[0][1] / 3] == 1) {
    Serial.println("you died");
  }

  if (gameBoard[snake[0][0] / 3][snake[0][1] / 3] == 2) {
    Serial.println("apple ate");
    snakeLength++;

    SetGameBoard(snake[0][0], snake[0][1], 1);
    createApple();
  }
}

void SetGameBoard(int x, int y, int value)
{
  int boardX = x / 3;
  int boardY = y / 3;

  gameBoard[boardX][boardY] = value;

  int headPosition[2] = {boardX, boardY};

  return headPosition;
}
