#include "headers/dot.h"
#include <math.h>
#include <gccore.h>
#include <cstdio>

asm("nop");

short dot_model[32];

constexpr float angle = 6.283185307179586476925 / 15.0f;
constexpr float dot_radius = .2f;

void make_the_dot_model() {
  dot_model[0] = 0;
  dot_model[1] = 0;
  for(int i = 0; i < 15; i++) {
    //premature adoption of fixed point arithmeti🥶🥶🥶🥶
    dot_model[i * 2 + 2] = (short)round(cos(angle * i) * 16384.0);
    dot_model[i * 2 + 2] = (short)round(sin(angle * i) * 16384.0);
  }
}
void update_dots_3d_model(Dot* g) {
  
  for(int i = 0; i < 16; i++) {
    g->three_dimensional_model[i*3] = (short)round(g->positions[i].x * 32);
    g->three_dimensional_model[i*3+1] = (short)round(g->positions[i].y * 32);
    g->three_dimensional_model[i*3+2] = 0;
  }
  
  DCStoreRange(g->three_dimensional_model, 96);
}
//this is the function to reset her shape and put her at a certain location, such as at the start of a level*
void re_position_dot(Dot* g, double x, double y) {
  for(int i = 0; i < 32; i++) {
    ((float*)(g->positions))[i] = ((float)(dot_model[i])) / 16384.0f * dot_radius;
  }
  g->x = x;
  g->y = y;
}
//*not all levels though

void debug_dot(Dot* g) {
  for(int i = 0; i < 16; i++) {
    printf("%i ", g->three_dimensional_model[i*3]);
    printf("%i ", g->three_dimensional_model[i*3+1]);
    printf("%i\n", g->three_dimensional_model[i*3+2]);
    printf("\n \n");
  }
}