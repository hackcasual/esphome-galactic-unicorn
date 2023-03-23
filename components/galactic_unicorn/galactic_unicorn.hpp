#pragma once

#include "hardware/pio.h"
#include "pico_synth.hpp"

namespace pimoroni {

  class GalacticUnicorn {
  public:
    static const int WIDTH  = 53;
    static const int HEIGHT = 11;

    // pin assignments
    static const uint8_t COLUMN_CLOCK           = 13;
    static const uint8_t COLUMN_DATA            = 14;
    static const uint8_t COLUMN_LATCH           = 15;
    static const uint8_t COLUMN_BLANK           = 16;

    static const uint8_t ROW_BIT_0              = 17;
    static const uint8_t ROW_BIT_1              = 18;
    static const uint8_t ROW_BIT_2              = 19;
    static const uint8_t ROW_BIT_3              = 20;
  
    static const uint8_t LIGHT_SENSOR           = 28;

    static const uint8_t MUTE                   = 22;

    static const uint8_t I2S_DATA               =  9;
    static const uint8_t I2S_BCLK               = 10;
    static const uint8_t I2S_LRCLK              = 11;

    static const uint8_t I2C_SDA                =  4;
    static const uint8_t I2C_SCL                =  5;

    static const uint8_t SWITCH_A               =  0;
    static const uint8_t SWITCH_B               =  1;
    static const uint8_t SWITCH_C               =  3;
    static const uint8_t SWITCH_D               =  6;

    static const uint8_t SWITCH_SLEEP           = 27;

    static const uint8_t SWITCH_VOLUME_UP       =  7;
    static const uint8_t SWITCH_VOLUME_DOWN     =  8;
    static const uint8_t SWITCH_BRIGHTNESS_UP   = 21;
    static const uint8_t SWITCH_BRIGHTNESS_DOWN = 26;

  private:
    static const uint32_t ROW_COUNT = 11;
    static const uint32_t BCD_FRAME_COUNT = 14;
    static const uint32_t BCD_FRAME_BYTES = 60;
    static const uint32_t ROW_BYTES = BCD_FRAME_COUNT * BCD_FRAME_BYTES;
    static const uint32_t BITSTREAM_LENGTH = (ROW_COUNT * ROW_BYTES);
    static const uint SYSTEM_FREQ = 22050;

  private:
    static PIO bitstream_pio;
    static uint bitstream_sm;
    static uint bitstream_sm_offset;

    static PIO audio_pio;
    static uint audio_sm;
    static uint audio_sm_offset;

    uint16_t brightness = 256;
    uint16_t volume = 127;

    // must be aligned for 32bit dma transfer
    alignas(4) uint8_t bitstream[BITSTREAM_LENGTH] = {0};
    const uint32_t bitstream_addr = (uint32_t)bitstream;
    static GalacticUnicorn* unicorn;
    static void dma_complete();

    static const uint NUM_TONE_BUFFERS = 2;
    static const uint TONE_BUFFER_SIZE = 4;
    int16_t tone_buffers[NUM_TONE_BUFFERS][TONE_BUFFER_SIZE] = {0};
    uint current_buffer = 0;

    PicoSynth synth;

    enum PlayMode {
      PLAYING_BUFFER,
      //PLAYING_TONE,
      PLAYING_SYNTH,
      NOT_PLAYING
    };
    PlayMode play_mode = NOT_PLAYING;

  public:
    ~GalacticUnicorn();

    void init();
    static inline void pio_program_init(PIO pio, uint sm, uint offset);

    void clear();

    void set_brightness(float value);
    float get_brightness();
    void adjust_brightness(float delta);

    void set_volume(float value);
    float get_volume();
    void adjust_volume(float delta);

    void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

    uint16_t light();

    bool is_pressed(uint8_t button);

    void play_sample(uint8_t *data, uint32_t length);
    void play_synth();
    void stop_playing();
    AudioChannel& synth_channel(uint channel);

  private:
    void partial_teardown();
    void dma_safe_abort(uint channel);
    void next_audio_sequence();
    void populate_next_synth();
  };

}

#include "esphome/core/macros.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/color.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/addressable_light.h"

#include "esphome/core/log.h"
static const char *TAG = "galactic_unicorn.component";

namespace esphome {
namespace galactic_unicorn {

class GalacticUnicornOutput : public light::AddressableLight {
 public:
  // ========== INTERNAL METHODS ==========
  void setup() override {
    this->effect_data_ = new uint8_t[this->size()];  // NOLINT
    this->pixelbuf = new uint8_t[4 * this->size()];
    this->unicorn_dev = new pimoroni::GalacticUnicorn();
    this->unicorn_dev->init();
    this->unicorn_dev->set_brightness(0.5);
    this->unicorn_dev->set_pixel(5, 5, 255, 0, 0);

    for (int i = 0; i < this->size(); i++) {
      (*this)[i] = Color(0, 0, 0, 0);
    }
  }

  void write_state(light::LightState *state) override {
    ESP_LOGD(TAG, "Write_state");
    
    uint index = 0;
    
    for (uint8_t r = 0; r < 11; r++) {
        for (uint8_t c = 0; c < 53; c++) {
            this->unicorn_dev->set_pixel(c, r, this->pixelbuf[index], this->pixelbuf[index + 1], this->pixelbuf[index + 2]);
            index+=4;
        }
    }
    
    this->mark_shown_();
  }


  int32_t size() const override { return 583; }
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }
  
  void clear_effect_data() override {
  }
 protected:

  light::ESPColorView get_view_internal(int32_t index) const override {  // NOLINT
    uint8_t *base = this->pixelbuf + 4ULL * index;
    
    //if (index == 5) {
    //    ESP_LOGD(TAG, "Getting good old 5 %d %d %d", base[0], base[1], base[2]);
    //}

    return light::ESPColorView(base, base + 1, base + 2,
                               nullptr, this->effect_data_ + index, &this->correction_);
  } 
  private:
      uint8_t *effect_data_{nullptr};
      uint8_t rgb_offsets_[4]{0, 1, 2, 3};
      uint8_t *pixelbuf{nullptr};
      pimoroni::GalacticUnicorn* unicorn_dev{nullptr};
};


}  // namespace empty_component
}  // namespace esphome