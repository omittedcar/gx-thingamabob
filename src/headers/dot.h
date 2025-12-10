#ifndef MECHANISM_DOT_H
#define MECHANISM_DOT_H
#include "util.h"

struct Dot {
  double x;
  double y;
  struct {float x; float y;} positions[16];
  struct {float x; float y;} velocities[16];
  signed short three_dimensional_model[48];
};
void make_the_dot_model();
void update_dots_3d_model(Dot* g);
void re_position_dot(Dot* g, double x, double y);
void debug_dot(Dot* g);
extern short dot_model[32];

#endif