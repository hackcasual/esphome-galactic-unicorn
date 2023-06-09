#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace esphome_galactic_unicorn {

class EmptyComponent : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
};


}  // namespace empty_component
}  // namespace esphome