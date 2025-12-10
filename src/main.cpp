
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>
#include <unistd.h>
#include <stdio.h>
#include <fat.h>
#include "headers/mn_gfx.h"
#include "headers/dot.h"
/*

int main () {
  mn_gfx_init();
  while(1) {
    mn_gfx_update();
    WPAD_ScanPads();
	  if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
  }
}
*/

;
void graphics();

int64_t timestep_timer = 0;

s16	vertices[] ATTRIBUTE_ALIGN(32) = {
	-0x8, 0xC, 0,
	-0x8, -0xC, 0,
	0x10,	0x0, 0
};

u8 colors[]	ATTRIBUTE_ALIGN(32)	= {
	0xff, 0xff, 0xff, 0xff,	// white
	0xff, 0xff, 0xff, 0xff,	// white
	0xff, 0xff, 0xff, 0xff,	// white
};
/*
Mtx44 projection = Mtx44{
	{.67, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0}
};
*/

//this is the time when the next update is schedule to occur, in CPU cycles.
//the update will occur a bit later than this time. 
u64 when_to_next_update = 0;

struct Dot* player_character;

uint8_t update();

int main(void) {
  player_character = (Dot*) malloc(sizeof(Dot));
  make_the_dot_model();
  re_position_dot(player_character, 0.0, 0.0);
  mn_gfx_init();
  WPAD_Init();
  if (fatInitDefault()) {
    //FILE* the_thing = fopen("SD:/apps/my_epic_new_app/assets/test.txt", "w");
    //char* the_text = "kgsdfhjl;sgdfhugesagfsdsdgfdsfgxhich,k.gfdk,j.hnkjlfds.unghskldafgjaaafdsfsfdsdfdddddddddddf";
    //fwrite(the_text, 1, 64, the_thing);
  } else {
    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;
  }

  mn_gfx_vertex_pointers(player_character->three_dimensional_model, colors);
  
  while (1) {
    if(gettime() > when_to_next_update) {
      if(update()) {
        break;
      }
    } else usleep(0x400); // usleep it like sleeps for an amount of time given in microseconds
  }
  free(player_character);
  return 0;
}


uint8_t update() { //returns wether or not the update had catastrophic results
  if(mn_gfx_is_ready()) {
  //vertices[7] = (int)(sin(timestep_timer / 128.0f) * 8.0f) + 0x10;
    graphics();
    
  }
  //}
  WPAD_ScanPads();

  //this increments the when_to_next_update timer by |̱,,̱,̱,,̱,̱,,̱,̱,,̱,̱,,̱,̱,,̱,̱,,̱,̱,,̱,̱,,̱,̱, Broadway clock cycles 
	when_to_next_update += 0x2000000;
  if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) return 1;
  return 0;
}

void graphics() {
  update_dots_3d_model(player_character);
  debug_dot(player_character);
  mn_gfx_update();
}