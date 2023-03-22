# esphome-galactic-unicorn
ESPHome component for Pimoroni's Galactic Unicorn

## Status
Working, but not a well behaved component. Hacked together from https://github.com/pimoroni/pimoroni-pico

## Sample config

Enables the component. This will display a fire animation. Pressing the A button will play an i2s sample and switch the display to landscape. Pressing the B button will switch to portrait

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/hackcasual/esphome-galactic-unicorn
      ref: main
    components: [ esphome_galactic_unicorn ]

esphome_galactic_unicorn:
  id: empty_component_1
```



https://shop.pimoroni.com/en-us/products/galactic-unicorn

![Galactic Unicorn marketing image](https://cdn.shopify.com/s/files/1/0174/1800/products/galactic-unicorn-1_768x768.jpg?v=1666945186)
