#include <GyverPower.h>
#include <powerConstants.h>

#include <stdio.h>

#include "src/epd1in54_V2.h"
#include "src/epdpaint.h"

#include "commands.h"
#include "turns.h"
#include "prescaler.h"

#define BLUETOOTH_VCC_PIN 2

Epd display;
uint8_t partialRefreshesCount;

uint8_t readCommand;
DirectionCommandData *directionCommandData;

void setup()
{
    powerOnSetup();
}

void powerOnSetup()
{
    pinMode(BLUETOOTH_VCC_PIN, OUTPUT);
    digitalWrite(BLUETOOTH_VCC_PIN, HIGH);

    power.setSleepMode(POWERDOWN_SLEEP);
    power.setSystemPrescaler(SYSTEM_PRESCALER);
    Serial.begin(38400 * SYSTEM_CLOCK_DIVIDER);
    Serial.println(F("powerOnSetup(): serial_set_up"));
    Serial.print(F("powerOnSetup(): prescaler_set: divider="));
    Serial.println((uint32_t)SYSTEM_CLOCK_DIVIDER);

    displayClear();
    Serial.println(F("powerOnSetup(): display_cleared"));
}

void tryToReadCommand()
{
    if (Serial.available())
    {
        uint8_t command = Serial.read();
        bool unknownCommand = false;
        bool commandReadSuccessfully = false;

        switch (command)
        {
        case COMMAND_DIRECTION:
            Serial.println(F("tryToReadCommand(): command_read: command=DIRECTION"));
            commandReadSuccessfully = readDirectionCommandData();
            break;

        case COMMAND_CLEAR:
            Serial.println(F("tryToReadCommand(): command_read: command=CLEAR"));
            commandReadSuccessfully = true;
            break;

        case COMMAND_FRAME:
            Serial.println(F("tryToReadCommand(): command_read: command=FRAME"));
            commandReadSuccessfully = true;
            break;

        default:
            unknownCommand = true;
            Serial.print(F("tryToReadCommand(): unknown_command_read: command="));
            Serial.println(command, HEX);
            break;
        }

        if (!unknownCommand)
        {
            if (!commandReadSuccessfully)
            {
                Serial.println(F("tryToReadCommand(): command_data_read_failed"));
            }
            else
            {
                readCommand = command;
            }
        }
    }
}

bool readDirectionCommandData()
{
    uint8_t size = sizeof(DirectionCommandData);
    if (Serial.available() < size)
    {
        Serial.print(F("readDirectionCommandData(): insufficient_bytes_available: required="));
        Serial.println(size);
        return false;
    }

    uint8_t dataBuffer[size];
    for (uint8_t i = 0; i < size; i++)
    {
        dataBuffer[i] = Serial.read();
    }
    directionCommandData = (DirectionCommandData *)dataBuffer;

    Serial.print(F("readDirectionCommandData(): data_read: turnType="));
    Serial.print(directionCommandData->turnType);
    Serial.print(F(", distanceM="));
    Serial.println(directionCommandData->distanceM);

    return true;
}

void initDisplayAndClearMemory(bool partial)
{
    display.Init(partial);
    display.ClearMemory();
}

void powerOffDisplay()
{
    display.Sleep();
    display.PowerOff();
}

void displayClear()
{
    initDisplayAndClearMemory(false);

    Serial.println(F("displayClear(): display_initialized"));

    // Reliable cleanup.
    for (int i = 0; i < 3; i++)
    {
        display.DisplayFrameInversed();
    }

    powerOffDisplay();

    Serial.println(F("displayClear(): done"));
}

void displayShowDirection()
{
    bool partial = partialRefreshesCount != 5;
    partialRefreshesCount = (partialRefreshesCount + 1) % 6;

    initDisplayAndClearMemory(partial);

    Serial.print(F("displayShowDirection(): display_initialized: partial="));
    Serial.println(partial);

    const uint8_t turnImageTop = 12;
    const uint8_t distanceStringTop = EPD_HEIGHT - 40 - 12;

#pragma region Display turn
    switch (directionCommandData->turnType)
    {
    // C, continue
    case 1:
        display.SetFrameMemory(turn_c.data_pgm, true, (EPD_WIDTH - turn_c.width) / 2, turnImageTop,
                               turn_c.width, turn_c.height, false);
        break;
    // TL, turn left
    case 2:
        display.SetFrameMemory(turn_tl.data_pgm, true, (EPD_WIDTH - turn_tl.width) / 2, turnImageTop,
                               turn_tl.width, turn_tl.height, false);
        break;
    // TSLL, turn slightly left
    case 3:
        display.SetFrameMemory(turn_tsll.data_pgm, true, (EPD_WIDTH - turn_tsll.width) / 2, turnImageTop,
                               turn_tsll.width, turn_tsll.height, false);
        break;
    // TSHLL, turn sharp left
    case 4:
        display.SetFrameMemory(turn_tshl.data_pgm, true, (EPD_WIDTH - turn_tshl.width) / 2, turnImageTop,
                               turn_tshl.width, turn_tshl.height, false);
        break;
    // TR, turn right
    case 5:
        display.SetFrameMemory(turn_tl.data_pgm, true, (EPD_WIDTH - turn_tl.width) / 2, turnImageTop,
                               turn_tl.width, turn_tl.height, true);
        break;
    // TSLR, turn slightly right
    case 6:
        display.SetFrameMemory(turn_tsll.data_pgm, true, (EPD_WIDTH - turn_tsll.width) / 2, turnImageTop,
                               turn_tsll.width, turn_tsll.height, true);
        break;
    // TSHLL, turn sharp right
    case 7:
        display.SetFrameMemory(turn_tshl.data_pgm, true, (EPD_WIDTH - turn_tshl.width) / 2, turnImageTop,
                               turn_tshl.width, turn_tshl.height, true);
        break;
    // TU, U-turn
    case 10:
        display.SetFrameMemory(turn_tu.data_pgm, true, (EPD_WIDTH - turn_tu.width) / 2, turnImageTop,
                               turn_tu.width, turn_tu.height, false);
        break;
    // TRU, right U-turn
    case 11:
        display.SetFrameMemory(turn_tu.data_pgm, true, (EPD_WIDTH - turn_tu.width) / 2, turnImageTop,
                               turn_tu.width, turn_tu.height, true);
        break;
    // Not supported – small black square
    default:
        unsigned char empty[] = {0xFF, 0XFF};
        display.SetFrameMemory(empty, false, 98, turnImageTop, 4, 4, false);
        break;
    }
#pragma endregion

#pragma region Display distance
    char distanceString[20];

    // For more than 945 meters show kilometers
    if (directionCommandData->distanceM > 945)
    {
        // Round the distance in kilometers to the first decimal place.
        uint8_t intPart = directionCommandData->distanceM / 1000;
        uint16_t decimalPart = directionCommandData->distanceM % 1000 + 1000;
        while (decimalPart > 100)
        {
            uint8_t rem = decimalPart % 10;
            decimalPart /= 10;
            if (rem >= 5)
            {
                decimalPart++;
            }
        }

        // If the decimal part rounding leads to the int part increment.
        if (decimalPart == 20)
        {
            intPart++;
        }

        decimalPart %= 10;

        if (decimalPart != 0)
        {
            sprintf(distanceString, "%d.%d km", intPart, decimalPart);
        }
        else
        {
            sprintf(distanceString, "%d km", intPart);
        }
    }
    else if (directionCommandData->distanceM > 2)
    {
        sprintf(distanceString, "%d m", directionCommandData->distanceM);
    }
    else
    {
        sprintf(distanceString, "now");
    }

    unsigned char distanceStringWidth = Paint::MeasureString(distanceString, &FontSegoeUI36);
    unsigned char frameBuffer[1024];
    Paint paint(frameBuffer, EPD_WIDTH, FontSegoeUI36.height);
    paint.Clear(UNCOLORED);
    paint.DrawStringAt((paint.GetWidth() - distanceStringWidth) / 2, 0, distanceString, &FontSegoeUI36, COLORED);
    display.SetFrameMemory(paint.GetImage(), false, 0, 160, paint.GetWidth(), paint.GetHeight(), false);

#pragma endregion

    display.DisplayFrameInversed();

    powerOffDisplay();

    Serial.println(F("displayShowDirection(): done"));
}

void displayShowFrameFromSerial()
{
    display.Init(false);

    uint32_t w, h;
    w = (EPD_WIDTH % 8 == 0) ? (EPD_WIDTH / 8) : (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;
    uint32_t bytesToRead = w * h;

    Serial.print(F("displayShowFrameFromSerial(): display_initialized, bytes_to_read="));
    Serial.println(bytesToRead);

    uint32_t bytesRead = 0;

    display.SendCommand(WRITE_BLACK_RAM);
    for (int i = 0; i < bytesToRead; i++)
    {
        while (Serial.available() == 0)
        {
            delay(SYSTEM_CLOCK_DIVIDER);
        }

        display.SendData(Serial.read());
        bytesRead++;
    }

    Serial.print(F("displayShowFrameFromSerial(): frame_written"));

    display.DisplayFrame();
    powerOffDisplay();

    Serial.println(F("displayShowFrameFromSerial(): done"));
}

void loop()
{
    tryToReadCommand();

    switch (readCommand)
    {
    case COMMAND_DIRECTION:
        displayShowDirection();
        break;

    case COMMAND_CLEAR:
        displayClear();
        break;

    case COMMAND_FRAME:
        displayShowFrameFromSerial();
        break;

    default:
        break;
    }

    readCommand = 0;
}
