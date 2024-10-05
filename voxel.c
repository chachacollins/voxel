#include "dos.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

uint8_t *heightmap = NULL;
uint8_t *colormap = NULL;

typedef struct {
  float x;
  float y;
  float zfar;
} camera_t;

camera_t camera = {
    .x = 512,
    .y = 512,
    .zfar = 400,
};

int main(int argc, char **argv) {
  setvideomode(videomode_320x200);
  uint8_t pallete[256 * 3];
  int map_width, map_height, pal_count;

  colormap = loadgif("./maps/map0.color.gif", &map_width, &map_height,
                     &pal_count, pallete);

  heightmap =
      loadgif("./maps/map0.height.gif", &map_width, &map_height, NULL, NULL);
  setdoublebuffer(1);

  for (int i = 0; i < pal_count; i++) {
    setpal(i, pallete[3 * i + 0], pallete[3 * i + 1], pallete[3 * i + 2]);
  }
  setpal(0, 36, 36, 56);

  uint8_t *framebuffer = screenbuffer();

  while (!shuttingdown()) {
    waitvbl();
    clearscreen();
    // TODO: processing input
    // update
    // render

    float plx = -camera.zfar;
    float ply = +camera.zfar;
    float prx = +camera.zfar;
    float pry = +camera.zfar;

    for (int i = 0; i < SCREEN_WIDTH; i++) {
      float Dx = (plx + (prx - plx) / SCREEN_WIDTH * i) / camera.zfar;
      float Dy = (ply + (pry - ply) / SCREEN_WIDTH * i) / camera.zfar;

      float rx = camera.x;
      float ry = camera.y;
      for (int z = 1; z < camera.zfar; z++) {
        rx += Dx;
        ry -= Dy;

        framebuffer[(SCREEN_WIDTH * (int)(ry / 4)) + (int)(rx / 4)] = 0x20;
      }
    }
    framebuffer = swapbuffers();
    if (keystate(KEY_ESCAPE)) {
      break;
    }
  }

  return EXIT_SUCCESS;
}
