#include "include/drivers/PioI2C.h"
#include "hardware/gpio.h"
#include "i2c.pio.h"
#include "private/pio_i2c.h"


IO::PioI2C::PioI2C(PIO pio, uint sda, uint scl) : pio(pio), sda(sda), scl(scl) {}

void IO::PioI2C::init(uint baudrate) {
    sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &i2c_program);
    i2c_program_init(pio, sm, offset, sda, scl, baudrate);
}

int IO::PioI2C::write(uint8_t addr, const uint8_t *src, size_t len, bool nostop) {
    return pio_i2c_write_blocking(pio, sm, addr, src, len);
}
