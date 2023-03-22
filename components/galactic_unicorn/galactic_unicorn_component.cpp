#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"

#include "galactic_unicorn.hpp"



#include "esphome/core/log.h"
#include "galactic_unicorn_component.hpp"


extern uint8_t left_channel_bin[];
extern uint32_t left_channel_bin_len;
extern uint8_t right_channel_bin[];
extern uint32_t right_channel_bin_len;

using namespace pimoroni;

GalacticUnicorn galactic_unicorn;

namespace esphome {
namespace galactic_unicorn {

static const char *TAG = "galactic_unicorn.component";


void GalacticUnicornOutput::setup() override {
  ESP_LOGCONFIG(TAG, "Setup");
  
  for (int i = 0; i < this->size(); i++) {
    (*this)[i] = Color(0, 0, 0, 0);
  }
}

void GalacticUnicornOutput::write_state(light::LightState *state) override {
  //stdio_init_all();
  ESP_LOGCONFIG(TAG, "Write_state");
  this->mark_shown_();
}

}  // namespace empty_component
}  // namespace esphome
