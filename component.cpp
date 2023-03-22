#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"

#include "pico_graphics.hpp"
#include "galactic_unicorn.hpp"
//#include "okcolor.hpp"


#include "esphome/core/log.h"
#include "empty_component.hpp"


extern uint8_t left_channel_bin[];
extern uint32_t left_channel_bin_len;
extern uint8_t right_channel_bin[];
extern uint32_t right_channel_bin_len;

using namespace pimoroni;

PicoGraphics_PenRGB888 graphics(53, 11, nullptr);
GalacticUnicorn galactic_unicorn;

// extra row of pixels for sourcing flames and averaging
int width = 53;
int height = 15;

// a buffer that's at least big enough to store 55 x 15 values (to allow for both orientations)
float heat[1000] = {0.0f};

void set(int x, int y, float v) {
  heat[x + y * width] = v;
}

float get(int x, int y) {
  /*if(x < 0 || x >= width || y < 0 || y >= height) {
    return 0.0f;
  }*/
  x = x < 0 ? 0 : x;
  x = x >= width ? width - 1 : x;

  return heat[x + y * width];
}


namespace esphome {
namespace esphome_galactic_unicorn {

static const char *TAG = "galactic_unicorn.component";

  bool landscape = true;

void EmptyComponent::setup() {
  //stdio_init_all();
  ESP_LOGCONFIG(TAG, "Setup");

  galactic_unicorn.init();
  galactic_unicorn.set_brightness(0.5);





  
}

void EmptyComponent::loop() {
    if(galactic_unicorn.is_pressed(galactic_unicorn.SWITCH_BRIGHTNESS_UP)) {
      galactic_unicorn.adjust_brightness(+0.01);
    }
    if(galactic_unicorn.is_pressed(galactic_unicorn.SWITCH_BRIGHTNESS_DOWN)) {
      galactic_unicorn.adjust_brightness(-0.01);
    }

    if(galactic_unicorn.is_pressed(galactic_unicorn.SWITCH_A)) {
      landscape = true;
      width = 53;
      height = 15;
      memset(heat, 0, sizeof(heat));
    }
    if(galactic_unicorn.is_pressed(galactic_unicorn.SWITCH_B)) {
      landscape = false;
      width = 11;
      height = 55;
      memset(heat, 0, sizeof(heat));
    }

    static bool was_a_pressed = false;

    if(galactic_unicorn.is_pressed(GalacticUnicorn::SWITCH_A)) {
      if(!was_a_pressed) {
        galactic_unicorn.play_sample(left_channel_bin, left_channel_bin_len);
      }
      was_a_pressed = true;
    }else{
      was_a_pressed = false;
    }

    for(int y = 0; y < height; y++) {
      for(int x = 0; x < width; x++) {
        float value = get(x, y);

        graphics.set_pen(0, 0, 0);
        if(value > 0.5f) {
          graphics.set_pen(255, 255, 180);
        }else if(value > 0.4f) {
          graphics.set_pen(220, 160, 0);
        }else if(value > 0.3f) {
          graphics.set_pen(180, 30, 0);
        }else if(value > 0.22f) {
          graphics.set_pen(20, 20, 20);
        }

        if(landscape) {
          graphics.pixel(Point(x, y));
        }else{
          graphics.pixel(Point(y, x));
        }

        // update this pixel by averaging the below pixels
        float average = (get(x, y) + get(x, y + 2) + get(x, y + 1) + get(x - 1, y + 1) + get(x + 1, y + 1)) / 5.0f;

        // damping factor to ensure flame tapers out towards the top of the displays
        average *= landscape ? 0.95f : 0.99f;

        // update the heat map with our newly averaged value
        set(x, y, average);
      }
    }

    galactic_unicorn.update(&graphics);

    // clear the bottom row and then add a new fire seed to it
    for(int x = 0; x < width; x++) {
      set(x, height - 1, 0.0f);
    }

    // add a new random heat source
    int source_count = landscape ? 5 : 1;
    for(int c = 0; c < source_count; c++) {
      int px = (rand() % (width - 4)) + 2;
      set(px    , height - 2, 1.0f);
      set(px + 1, height - 2, 1.0f);
      set(px - 1, height - 2, 1.0f);
      set(px    , height - 1, 1.0f);
      set(px + 1, height - 1, 1.0f);
      set(px - 1, height - 1, 1.0f);
    }
    
}

void EmptyComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty component");

}


}  // namespace empty_component
}  // namespace esphome
