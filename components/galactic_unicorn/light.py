import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import (
    CONF_CHANNEL,
    CONF_CLOCK_PIN,
    CONF_DATA_PIN,
    CONF_METHOD,
    CONF_NUM_LEDS,
    CONF_PIN,
    CONF_TYPE,
    CONF_VARIANT,
    CONF_OUTPUT_ID,
    CONF_INVERT,
)

galactic_unicorn_ns = cg.esphome_ns.namespace("galactic_unicorn")
GalacticUnicornOutputBase = galactic_unicorn_ns.class_(
    "GalacticUnicornOutputBase", light.AddressableLight
)

GalacticUnicornOutput = galactic_unicorn_ns.class_(
    "GalacticUnicornOutput", GalacticUnicornOutputBase
)


CONFIG_SCHEMA = cv.All(
    cv.only_with_arduino,
    light.ADDRESSABLE_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(GalacticUnicornOutputBase),
            cv.Optional(CONF_INVERT, default="no"): cv.boolean,
            cv.Required(CONF_NUM_LEDS): cv.positive_not_null_int,
        }
    ).extend(cv.COMPONENT_SCHEMA),
)