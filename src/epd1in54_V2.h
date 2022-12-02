/*****************************************************************************
* | File      	:   epd1in54_V2.h
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

#ifndef epd1in54_V2_H
#define epd1in54_V2_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH 200
#define EPD_HEIGHT 200

// Commands
#define DRIVER_OUTPUT_CONTROL 0x01
#define GATE_DRIVING_VOLTAGE_CONTROL 0x03
#define SOURCE_DRIVING_VOLTAGE_CONTROL 0x04
#define BOOSTER_SOFT_START_CONTROL 0x0C
#define GATE_SCAN_START_POSITION 0x0F
#define DEEP_SLEEP_MODE 0x10
#define DATA_ENTRY_MODE_SETTING 0x11
#define SW_RESET 0x12
#define TEMPERATURE_SENSOR_CONTROL 0x1A
#define MASTER_ACTIVATION 0x20
#define DISPLAY_UPDATE_CONTROL_1 0x21
#define DISPLAY_UPDATE_CONTROL_2 0x22
#define WRITE_BLACK_RAM 0x24
#define WRITE_RED_RAM 0x26
#define WRITE_VCOM_REGISTER 0x2C
#define WRITE_LUT_REGISTER 0x32
#define SET_DUMMY_LINE_PERIOD 0x3A
#define SET_GATE_TIME 0x3B
#define BORDER_WAVEFORM_CONTROL 0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION 0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION 0x45
#define SET_RAM_X_ADDRESS_COUNTER 0x4E
#define SET_RAM_Y_ADDRESS_COUNTER 0x4F

class Epd : EpdIf
{
public:
    Epd();
    ~Epd();

    /**
     * Init the display after sleep or power off.
     * Calls PowerOn in prior.
     *
     * @returns 0 in case of successful init, -1 otherwise.
     */
    int Init(bool partial);

    /**
     * Sends given command to the display.
     */
    void SendCommand(unsigned char command);

    /**
     * Sends given data to the display.
     */
    void SendData(unsigned char data);

    /**
     * Waits until the display is idling (BUSY pin LOW).
     */
    void WaitUntilIdle(void);

    /**
     * Resets the module with the RESET pin.
     * Causes all the RAM values reset to their POR state.
     */
    void Reset(void);

    /**
     * Fills the whole frame memory with "no color".
    */
    void ClearMemory(void);

    /**
     * Sets given buffer to the frame memory at given coordinates.
     * 
     * @param from_program_space if set, the buffer will be read with pgm_read
    */
    void SetFrameMemory(
        const unsigned char *image_buffer,
        bool from_program_space,
        int x,
        int y,
        int image_width,
        int image_height,
        bool mirror_x);
        
    /**
     * Updates the display with the RAM content.
     * there are 2 memory areas embedded in the e-paper display
     * but once this function is called,
     * the the next action of SetFrameMemory or ClearMemory will
     * set the other memory area.
    */
    void DisplayFrame(void);

    /**
     * Displays the frame but inverts the RAM content,
     * so "no color" is shown as "color" and vice-versa.
    */
    void DisplayFrameInversed(void);

    /**
     * Sends the display to the deep sleep.
     * After that, the display can be awaken by Init.
    */
    void Sleep(void);

    /**
     * Enables display power (VCC pin).
    */
    void PowerOn();

    /**
     * Disables display power (VCC pin).
    */
    void PowerOff();

private:
    void Lut(const unsigned char *lut);
    void SetLut(const unsigned char *lut);
    void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
    void SetMemoryPointer(int x, int y);
};

#endif /* EPD1IN54B_H */

/* END OF FILE */
