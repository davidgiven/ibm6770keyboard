#pragma once

#include <libmaple/usart.h>

class KeyboardInterface
{
private:
    static constexpr int BAUDRATE = 186453;

public:
    KeyboardInterface(
        usart_dev* usart, uint8_t rxPin, uint8_t txPin, uint8_t resetPin):
        _usart(usart),
        _serial(usart, txPin, rxPin),
        _rxPin(rxPin),
        _txPin(txPin),
        _resetPin(resetPin)
    {
    }

    void begin()
    {
        _serial.begin(BAUDRATE);
        // pinMode(_txPin, OUTPUT_OPEN_DRAIN);
        //  pinMode(_rxPin, INPUT);

        pinMode(_resetPin, OUTPUT_OPEN_DRAIN);
        digitalWrite(_resetPin, 1);
        delay(200);
        digitalWrite(_resetPin, 0);
        delay(200);
    }

    void txc(uint8_t b)
    {
        setUsartFlags(SERIAL_8N1);
        rawTx(b);
    }

    void tx(uint8_t b)
    {
        rawTx(b);
    }

private:
    void rawTx(uint8_t b)
    {
        _serial.write(b);
        _serial.flush();

        /* As the rx and tx lines are wired together, when the byte is
         * transmitted it'll get picked up by the receiver. Read and discard
         * this ready for the real response. */

        rx();
        setUsartFlags(SERIAL_8E1);
    }

public:
    uint8_t txrxc(uint8_t b)
    {
        txc(b);
        return rx();
    }

    uint8_t txrx(uint8_t b)
    {
        tx(b);
        return rx();
    }

    uint8_t rx()
    {
        while (!_serial.available())
            ;
        return _serial.read();
    }

public:
    void initHardware()
    {
        while (cmdPoll() != 0x88)
            ;

        cmdInit();
    }

    bool keysAvailable() const
    {
        return !(_flags & 0x80);
    }

public:
    uint8_t cmdInit()
    {
        txrxc(0x11);
        txrx(0);
        return txrx(0);
    }

    uint8_t cmdPoll()
    {
        txrxc(0x22);
        _flags = txrx(0);
        txrx(0);
        return _flags;
    }

    uint8_t cmdGetKey()
    {
        txrxc(0x33);
        uint8_t k = txrx(0);
        txrx(0);
        return k;
    }

void cmdSoundControl(uint8_t op)
{
txrxc(0x88);
txrx(op);  
}

    uint8_t getFlags() const
    {
        return _flags;
    }

private:
    void setUsartFlags(uint8_t flags)
    {
        _usart->regs->CR1 = (_usart->regs->CR1 & 0B1110000111111111) |
                            ((uint32_t)(flags & 0x0F) << 9);
        _usart->regs->CR2 = (_usart->regs->CR2 & 0B1100111111111111) |
                            ((uint32_t)(flags & 0x30) << 8);
    }

private:
    usart_dev* _usart;
    HardwareSerial _serial;
    uint8_t _rxPin;
    uint8_t _txPin;
    uint8_t _resetPin;
    uint8_t _flags;
};
