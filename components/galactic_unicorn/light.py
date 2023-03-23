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
GalacticUnicornOutput = galactic_unicorn_ns.class_(
    "GalacticUnicornOutput", light.AddressableLight
)



CONFIG_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(GalacticUnicornOutput)
        }
    ).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    out_type = GalacticUnicornOutput
    rhs = out_type.new()
    var = cg.Pvariable(config[CONF_OUTPUT_ID], rhs, out_type)
    await cg.register_component(var, config)
    await light.register_light(var, config)
