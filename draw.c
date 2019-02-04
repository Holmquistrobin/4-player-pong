// Written 2018 by Erik Flink

#include "oled_i2c_communication_header.h"
#include "game_logic_header.h"

// Draws a vertical paddle at (x, y) coordinates, with specified height, in oled buffer
void draw_vertical_paddle(int x, int y, int height) {
  int i = 0;
  for (i = 0; i < height; i++) {
    set_pixel(x, y + i);
  }
}

// Draws a horizontal paddle at (x, y), with specified height coordinates in oled buffer
void draw_horizontal_paddle(int x, int y, int width) {
  int i = 0;
  for (i = 0; i < width; i++) {
    set_pixel(x + i, y);
  }
}

// Draws ball/square at (x, y) coordinates, with specified radius/width
void draw_ball(int x, int y, int r) {
  int i = 0;
  int j = 0;

  for (i = 0; i < r; i++) {
    for (j = 0; j < r; j++) {
      set_pixel(x + i, y + j);
    }
  }
}
