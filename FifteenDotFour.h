#ifndef FifteenDotFour_h
#define FifteenDotFour_h             // include guard
//#include <Stream.h>
//#include <Energia.h>
#include <utils/buffer_c/buffer.h>

#define MTU         512
/* Tx buffer -> MTU, Rx buffer -> MTU * 2 */

class FifteenDotFour //: public Stream
{
//    private:
    protected:
        /* Buffer Handling implementing buffer_c API */
        buffer_t rx_buffer;
        buffer_t tx_buffer;
        bool m_device;

    public:
        /* Constructors */
        FifteenDotFour();
        FifteenDotFour(bool dev); // INTERFACE

        /* Member Functions */
        virtual void begin();
        virtual bool beginTransmission();
        virtual bool endTransmission();
        /* ------------------------------------------------- */
        /*                  Buffer Handling                  */
        /* ------------------------------------------------- */
        /**
         * Call buffer_get_size() to calculate number of bytes in buffer.
         *
         * @return              number of bytes available in the buffer
         */
        virtual int available(void);

        /**
         * Call buffer_read to read a single byte.
         *
         * @return              read byte
         */
        virtual int read(void);

        /**
         * Call buffer_read_multiple() to read multiple bytes from rx_buffer
         * into user defined uint8_t array.
         *
         * @param  user_buf     user buffer pass by pointer
         * @param  size         amount of bytes to be read from rx_buffer
         * @return              number of bytes read
         */
        virtual int read(uint8_t* user_buf, size_t size);    // pop a certain amount of bytes from queue

        /**
         * Call buffer_write() to write a single byte to tx_buffer.
         *
         * @param  w_byte       written byte
         * @return              true/false successful write
         */
        virtual size_t write(const uint8_t w_byte);

        /**
         * Call buffer_write_mutliple() to write multiple bytes from a user
         * defined uint8_t array into the tx_buffer.
         *
         * @param  user_buf     uint8_t user array to push onto tx_buffer
         * @param  size         size of the bytes to push onto tx_buffer
         * @return              true/false successful write
         */
        virtual size_t write(const int8_t* user_buf, size_t size);

        /**
         * Flush the 15.4 class variable rx_buffer.
         */
        virtual void flush(void);

        /**
         * Peek
         */
        virtual uint8_t peek(void);
        /* ------------------------------------------------- */
};
#endif
