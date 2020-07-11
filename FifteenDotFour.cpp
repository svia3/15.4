
#include <FifteenDotFour.h>

FifteenDotFour::FifteenDotFour() {}

void FifteenDotFour::begin()
{
    /* Create the buffers */
    buffer_init(&rx_buffer, RXTX_BUFFER_LENGTH);
    buffer_init(&tx_buffer, RXTX_BUFFER_LENGTH);
}

int FifteenDotFour::available(void)
{
    return buffer_get_size(&rx_buffer);
}

int FifteenDotFour::read(void)
{
    return buffer_read(&rx_buffer);
}

int FifteenDotFour::read(uint8_t* user_buf, size_t size)
{
    return buffer_read_multiple(user_buf, &rx_buffer, size);
}

size_t FifteenDotFour::write(const uint8_t w_byte)
{
    return buffer_write(&tx_buffer, w_byte);
}

size_t FifteenDotFour::write(const uint8_t* user_buf, size_t size)
{
    return buffer_write_multiple(&tx_buffer, user_buf, size);
}

void FifteenDotFour::peek(void)
{
    /* put this in buffer_c */
    return buffer_peek(&rx_buffer);
}

void FifteenDotFour::flush()
{
    buffer_flush(&tx_buffer);
    buffer_flush(&rx_buffer);   /* flush the tx_buffer too? */
}
