
//TODO add graphics
#include <stdint.h>
struct Texture {
  char the_actual_texture;
  char name[56];
};

void mn_gfx_init();
uint8_t mn_gfx_is_ready();
void mn_gfx_update();