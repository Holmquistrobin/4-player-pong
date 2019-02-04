/*This file is authored by Robin Holmquist, and added to by Erik Flink in order
to incorporate the two screens 2018.*/

#include <pic32mx.h>
#include "analog_communication_header.h"
#include "draw_header.h"

#define WIDTH 64

//Global variables
int game_over = 0;

int paddle_width = 1;
int paddle_length = 15;

float paddle_top_coordinates[2] = {0,0};
float paddle_bottom_coordinates[2] = {0, WIDTH - 1};
float paddle_left_coordinates[2] = {0,0};
float paddle_right_coordinates[2] = {WIDTH - 1, 0};

float ball_speed[2] = {0,0};
float ball_coordinates[2] = {32,32};

int score_top = 5;
int score_bottom = 5;
int score_left = 5;
int score_right = 5;

int difficulty = 1;

void set_coordinates(void){
  ball_coordinates[0] = WIDTH/2;
  ball_coordinates[1] = WIDTH/2;
}
/*This function resets scores, the position of the ball and determines the ball speed
depending on the value contained in the difficulty-variable. Authored by Robin
Holmquist.*/
void reset(void){
  set_coordinates();
  score_top = 5;
  score_bottom = 5;
  score_left = 5;
  score_right = 5;
  game_over = 0;
  ball_speed[0] = 0.7 * difficulty;
  ball_speed[1] = 0.4 * difficulty;
}
/*This function initallizes everything that has to be initiallized for this
specific file, and is authored by Robin Holmquist*/
void initiallize_logic(void){
  //Begins with difficulty 1
  reset();
  //Initiallizing startup coordinates of the paddles
  set_coordinates();

  //Initiallizeing for analog reading
  initiallize_analog();

  //Initializes buttons to change difficulty
  TRISD = 0x0E0; //RD5-7 are inputs
  TRISFSET = 1 << 1; //RF1 are inputs

}
/*These two following functions handles the change of the ball's direction if
it were to hit a paddle, and is autored by Robin Holmquist.*/
void direction_change_top_bottom(float x){
  //Bounce
  ball_speed[1] = -1*ball_speed[1];
  if(ball_coordinates[0]>(x + paddle_length/2)){
    ball_speed[0] +=0.3 * difficulty; //right side
  }
  else{
    ball_speed[0] -=0.3 * difficulty; //left side
  }
}
void direction_change_left_right(float y){
  //Bounce
  ball_speed[0] = -1*ball_speed[0];
  if(ball_coordinates[1] > (y + paddle_length/2)){
    ball_speed[1] += 0.3 * difficulty; //upper side
  }
  else{
    ball_speed[1] -= 0.3 * difficulty; //lower side
  }
}
/*This function checks if any paddle's score has been reduced to 0, which then
stops the game. Authored by Robin Holmquist*/
void set_game_over(void){
  if(score_top <= 0 || score_bottom <= 0 || score_left <= 0 || score_right <= 0){
    game_over = 1;
  }
}
/*This function checks the state of the ball in relation to the paddles; if
there is a hit, if the ball has gotten past a paddle, or if the ball is somewhere
else on the screen. It also takes appropriate measures if any of these incidents
occurs. Authored by Robin Holmquist.*/
void check_ball_hit(void){
  if(ball_coordinates[1] <= (paddle_top_coordinates[1] + paddle_width)&&
  ball_coordinates[0]<=(paddle_top_coordinates[0] + paddle_length)&&ball_coordinates[0]>=paddle_top_coordinates[0]){
    direction_change_top_bottom(paddle_top_coordinates[0]);
    ball_coordinates[1] = 0 + paddle_width;
  }
  else if(ball_coordinates[1] >= paddle_bottom_coordinates[1] - paddle_width&&
    ball_coordinates[0]<=(paddle_bottom_coordinates[0] + paddle_length)&&ball_coordinates[0]>=paddle_bottom_coordinates[0]){
      direction_change_top_bottom(paddle_bottom_coordinates[0]);
      ball_coordinates[1] = WIDTH - 1 - paddle_width;
    }
    else if(ball_coordinates[0] <= (paddle_left_coordinates[0] + paddle_width)&&
    ball_coordinates[1] <=(paddle_left_coordinates[1] + paddle_length)&&ball_coordinates[1]>=paddle_left_coordinates[1]){
      direction_change_left_right(paddle_left_coordinates[1]);
      ball_coordinates[0] = 0 + paddle_width;
    }
    else if(ball_coordinates[0] >= (paddle_right_coordinates[0]-paddle_width)&&
    ball_coordinates[1] <=(paddle_right_coordinates[1] + paddle_length)&&ball_coordinates[1]>=paddle_right_coordinates[1]){
      direction_change_left_right(paddle_right_coordinates[1]);
      ball_coordinates[0] = WIDTH - 1 - paddle_width;
    }
    //Should someone get a point or not? If so reset
    else{
      if(ball_coordinates[1] <= 0){
        score_top--;
        set_coordinates();
      }
      if(ball_coordinates[1] >= WIDTH){
        score_bottom--;
        set_coordinates();
      }
      if(ball_coordinates[0] <= 0){
        score_left--;
        set_coordinates();
      }
      if(ball_coordinates[0] >= WIDTH){
        score_right--;
        set_coordinates();
      }
      set_game_over();
    }
}
/*This function updates the coordinates of the ball and the paddles. Authored by
Robin Holmquist and Erik Flink.*/
void get_coordinates(void){
  //Coordinates for the ball
  ball_coordinates[0] += ball_speed[0];
  ball_coordinates[1] += ball_speed[1];
  //Coordinates for the top paddle
  paddle_top_coordinates[0] = ((float) get_A1() / 1024) * (WIDTH - paddle_length);
  //Coordinates for the bottom paddle
  paddle_bottom_coordinates[0] = ((float) get_A2() / 1024) * (WIDTH - paddle_length);
  //Coordinates for the left paddle
  paddle_left_coordinates[1] = ((float) get_A3() / 1024) * (WIDTH - paddle_length);
  //Coordinates for the right paddle
  paddle_right_coordinates[1] = ((float) get_A11() / 1024) * (WIDTH - paddle_length);
}

/*This function draws the paddles and the ball on the external screen. Authored by
Erik Flink.*/
void draw_frame() {
  draw_horizontal_paddle(paddle_top_coordinates[0] + WIDTH/2,
    paddle_top_coordinates[1], paddle_length);
  draw_horizontal_paddle(paddle_bottom_coordinates[0] + WIDTH/2,
    paddle_bottom_coordinates[1], paddle_length);

  draw_vertical_paddle(paddle_left_coordinates[0] + WIDTH/2,
    paddle_left_coordinates[1], paddle_length);
  draw_vertical_paddle(paddle_right_coordinates[0] + WIDTH/2,
    paddle_right_coordinates[1], paddle_length);

  draw_ball(ball_coordinates[0] + WIDTH/2, ball_coordinates[1], 1);
}
/*This function creates a 32 bit value from the 4 buttons and returns it.
Authored by Robin Holmquist and modified by Erik Flink.*/
int getbtns(void){
  int returnBtns = (PORTF & 0x2) >> 1;
  returnBtns = returnBtns | ((PORTD & 0x0E0)>>4);
  return returnBtns;
}

/*This function changes the difficulty-variable depending on which buttons are
pressed. It also resets the game in case any button is
pressed. Authored by Robin Holmquist.*/
void change_difficulty(void){
  if(getbtns() != 0){
    switch(getbtns()){
      case 1:
      difficulty = 1;
      break;
      case 2:
      difficulty = 2;
      break;
      case 4:
      difficulty = 3;
      break;
      case 8:
      difficulty = 4;
      break;
    }
    reset();
  }
}
/*This function displays the points of each paddle on the I/O-shield's screen.
Authored by Erik Flink.*/
void print_points(){
  char str_top[]    = "Top player:    X";
  str_top[15] = 0x30 + score_top;

  char str_bottom[] = "Bottom player: X";
  str_bottom[15] = 0x30 + score_bottom;

  char str_left[]   = "Left player:   X";
  str_left[15] = 0x30 + score_left;

  char str_right[]  = "Right player:  X";
  str_right[15] = 0x30 + score_right;

  display_string(0, str_top);
  display_string(1, str_bottom);
  display_string(2, str_left);
  display_string(3, str_right);
  display_update();
}
/*This function updates all game logic, and calls the display-functions. The
logic-related code is authored by Roin Holmquist, and the display-related code is
authored by Erik Flink.*/
void update_logic(void){
  if(game_over){
    clear_buffer();
    oled_update();

    display_string(0, "GAME OVER!");
    display_string(1, "");
    if (score_top == 0) {
      display_string(2, "Top player");
    }
    else if (score_bottom == 0) {
      display_string(2, "Bottom player");
    }
    else if (score_left == 0) {
      display_string(2, "Left player");
    }
    else if (score_right == 0) {
      display_string(2, "Right player");
    }
    display_string(3, "lost this time.");
    display_update();
  }
  else{
    //Updates the coordinates for all vectors
    //Change position of paddles based on potentiometers
    get_coordinates();
    //Check if the ball has hit or gotten past a paddle_length, and adjust scores,
    //ball speed and ball coordinates accordingly
    check_ball_hit();

    clear_buffer();
    draw_frame();
    oled_update();
    print_points();
  }
  //Changes difficulty level if buttons are pressed
  change_difficulty();
}
