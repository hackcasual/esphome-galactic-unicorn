#pragma once

#include "esphome/core/macros.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/color.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/addressable_light.h"

namespace esphome {
namespace galactic_unicorn {

class GalacticUnicornOutput : public light::AddressableLight {
 public:
  // ========== INTERNAL METHODS ==========
  void setup() override;
  void write_state(light::LightState *state) override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  int32_t size() const override { return 583; }
};


}  // namespace empty_component
}  // namespace esphome