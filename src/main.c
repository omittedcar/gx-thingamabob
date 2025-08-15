
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>
#include <unistd.h>
#include "headers/mn_gfx.h"
int main () {
  mn_gfx_init();
  while(1) {
    mn_gfx_update();
    WPAD_ScanPads();
	  if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
  }
}
/*
uint64_t when_to_next_update = 0;

uint8_t update();

int main(void) {
  mn_gfx_init();
  WPAD_Init();

  Mtx44 view_matrix = {
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0}
  };


  while (1) {
    if(gettime() > when_to_next_update) {
      if(update()) {
        break;
      }
    } else usleep(0x400); // usleep it like sleeps for an amount of time given in microseconds
  }
}

uint8_t update() { //returns wether or not the update had catastrophic results
  if(mn_gfx_is_ready()) {
    mn_gfx_update();
  }
  WPAD_ScanPads();
  //when_to_next_update += 0x172C9E; //delay 1/480 of second (not long time)
	when_to_next_update += 0x2000000;
  if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) return 1;
  return 0;
}
*/
