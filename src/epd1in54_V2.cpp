/*****************************************************************************
* | File      	:   epd1in54_V2.cpp
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper V2
* | Info        :   Modified by Radiokot
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "epd1in54_V2.h"

// Full refresh waveform.
// Datasheet with the description to be found.
const uint8_t LUT_FULL_REFRESH[159] PROGMEM = 
{
    0x80,	0x48,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x40,	0x48,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x80,	0x48,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x40,	0x48,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0xA,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x8,	0x1,	0x0,	0x8,	0x1,	0x0,	0x2,					
    0xA,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
    0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,			
    0x22,	0x17,	0x41,	0x0,	0x32,	0x20
};

const unsigned char LUT_PARTIAL_REFRESH[159] PROGMEM =
{
    0x0,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x80,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x40,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x8,	0x1,	0x0,	0x8,	0x1,	0x0,	0x1,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,
    0x02,	0x17,	0x41,	0xB0,	0x32,	0x28,
};

Epd::~Epd(){};

Epd::Epd() {};

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command)
{
    DigitalWrite(EPD_DC_PIN, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data)
{
    DigitalWrite(EPD_DC_PIN, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void)
{
    while (DigitalRead(EPD_BUSY_PIN) == 1)
    { // LOW: idle, HIGH: busy
        DelayMs(50);
    }
    DelayMs(200);
}

void Epd::Lut(const unsigned char *lut)
{
    SendCommand(WRITE_LUT_REGISTER);
    for (unsigned char i = 0; i < 153; i++)
        SendData(pgm_read_byte(lut + i));
    WaitUntilIdle();
}

void Epd::SetLut(const unsigned char *lut)
{
    Lut(lut);

    SendCommand(0x3f); // ???
    SendData(pgm_read_byte(lut + 153));

    SendCommand(GATE_DRIVING_VOLTAGE_CONTROL);
    SendData(pgm_read_byte(lut + 154));

    SendCommand(SOURCE_DRIVING_VOLTAGE_CONTROL);
    SendData(pgm_read_byte(lut + 155));
    SendData(pgm_read_byte(lut + 156));
    SendData(pgm_read_byte(lut + 157));

    SendCommand(WRITE_VCOM_REGISTER);
    SendData(pgm_read_byte(lut + 158));
}

int Epd::Init(bool partial)
{
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0)
    {
        return -1;
    }
    /* EPD hardware init start */
    PowerOn();
    Reset();

    WaitUntilIdle();
    SendCommand(SW_RESET);
    WaitUntilIdle();

    SendCommand(DRIVER_OUTPUT_CONTROL);
    SendData(0xC7);
    SendData(0x00);
    SendData(0x00);

    // Low direction.
    SendCommand(DATA_ENTRY_MODE_SETTING);
    SendData(0x03);

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((0 >> 3) & 0xFF);
    SendData((199 >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(0 & 0xFF);
    SendData((0 >> 8) & 0xFF);
    SendData(199 & 0xFF);
    SendData((199 >> 8) & 0xFF);

    SendCommand(BORDER_WAVEFORM_CONTROL);
    // Set black border.
    SendData(0b01010000);

    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    // Enable clock signal
    // Load temperature value
    // Load LUT with DISPLAY Mode 1
    // Disable clock signal
    SendData(0XB1);
    SendCommand(MASTER_ACTIVATION);

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    SendData(0x00);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(0xC7); // 199
    SendData(0x00);
    WaitUntilIdle();

    if (!partial)
    {
        SetLut(LUT_FULL_REFRESH);
    }
    else
    {
        SetLut(LUT_PARTIAL_REFRESH);
        SendCommand(0x37);
        SendData(0x00);
        SendData(0x00);
        SendData(0x00);
        SendData(0x00);
        SendData(0x00);
        SendData(0x40);
        SendData(0x00);
        SendData(0x00);
        SendData(0x00);
        SendData(0x00);
    }
    /* EPD hardware init end */

    return 0;
}

void Epd::Reset(void)
{
    DigitalWrite(EPD_RST_PIN, HIGH);
    DelayMs(20);
    // Module reset.
    DigitalWrite(EPD_RST_PIN, LOW);
    DelayMs(5);
    DigitalWrite(EPD_RST_PIN, HIGH);
    DelayMs(20);
}

void Epd::ClearMemory()
{
    int w, h;
    w = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;

    SendCommand(WRITE_BLACK_RAM);
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            SendData(0xff);
        }
    }
    // Looks useless for BW mode.
    // SendCommand(WRITE_RED_RAM);
    // for (int j = 0; j < h; j++)
    // {
    //     for (int i = 0; i < w; i++)
    //     {
    //         SendData(0xff);
    //     }
    // }
}

void Epd::SetMemoryArea(int x_start, int y_start, int x_end, int y_end)
{
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void Epd::SetMemoryPointer(int x, int y)
{
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    WaitUntilIdle();
}

void Epd::DisplayFrame(void)
{
    SendCommand(DISPLAY_UPDATE_CONTROL_1);
    // Take the RAM as it is during the update.
    SendData(0x00);
    SendData(0x00);
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    // Enable clock signal
    // Enable Analog
    // Display with DISPLAY Mode 1
    // Disable Analog
    // Disable clock signal
    SendData(0xC7);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

void Epd::DisplayFrameInversed(void)
{
    SendCommand(DISPLAY_UPDATE_CONTROL_1);
    // Inverse the RAM content during the update.
    SendData(0x08);
    SendData(0x00);
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    // Enable clock signal
    // Enable Analog
    // Display with DISPLAY Mode 1
    // Disable Analog
    // Disable clock signal
    SendData(0xC7);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

unsigned char reverseBits(unsigned char b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void Epd::SetFrameMemory(
    const unsigned char *image_buffer,
    bool from_program_space,
    int x,
    int y,
    int image_width,
    int image_height,
    bool mirror_x)
{
    int x_end;
    int y_end;

    DigitalWrite(EPD_RST_PIN, LOW); // module reset
    DelayMs(2);
    DigitalWrite(EPD_RST_PIN, HIGH);
    DelayMs(2);
    // I don't know why is it needed.
    // SendCommand(BORDER_WAVEFORM_CONTROL);
    // SendData(0x80);

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0)
    {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= EPD_WIDTH)
    {
        x_end = EPD_WIDTH - 1;
    }
    else
    {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= EPD_HEIGHT)
    {
        y_end = EPD_HEIGHT - 1;
    }
    else
    {
        y_end = y + image_height - 1;
    }

    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);

    SendCommand(WRITE_BLACK_RAM);
    for (int j = 0; j < y_end - y + 1; j++)
    {
        if (!mirror_x)
        {
            for (int i = 0; i < (x_end - x + 1) / 8; i++)
            {
                uint16_t index = i + j * (image_width / 8);
                SendData((!from_program_space) ? image_buffer[index] : pgm_read_byte(&image_buffer[index]));
            }
        }
        else
        {
            for (int i = (x_end - x + 1) / 8 - 1; i >= 0; i--)
            {
                uint16_t index = i + j * (image_width / 8);
                unsigned char data = (!from_program_space) ? image_buffer[index] : pgm_read_byte(&image_buffer[index]);
                SendData(reverseBits(data));
            }
        }
    }
}

void Epd::Sleep()
{
    SendCommand(DEEP_SLEEP_MODE);
    SendData(0x01);
    DelayMs(200);
    DigitalWrite(EPD_RST_PIN, LOW);
}

void Epd::PowerOn()
{
    DigitalWrite(EPD_VCC_PIN, HIGH);
}

void Epd::PowerOff()
{
    DigitalWrite(EPD_VCC_PIN, LOW);
}

/* END OF FILE */
