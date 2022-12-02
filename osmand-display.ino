#include <stdio.h>

#include "src/epd1in54_V2.h"
#include "src/epdpaint.h"

#include "commands.h"
#include "turns.h"

DirectionCommandData* directionCommandData;

void setup()
{
    powerOnSetup();
}

void powerOnSetup()
{
    Serial.begin(9600);
    Serial.println(F("powerOnSetup: serial_set_up"));
    displayClear();
    Serial.println(F("powerOnSetup: display_cleared"));
}

void tryToReadCommand()
{
    if (Serial.available())
    {
        uint8_t command = Serial.read();

        switch (command)
        {
        case COMMAND_DIRECTION:
            Serial.println(F("tryToReadCommand: command_read: command=DIRECTION"));
            if (!readDirectionCommandData())
            {
                Serial.println(F("tryToReadCommand: command_data_read_failed"));
            }
            displayShowDirection();
            break;

        default:
            Serial.print(F("tryToReadCommand: unknown_command_read: command="));
            Serial.println(command, HEX);
            break;
        }
    }
}

bool readDirectionCommandData()
{
    uint8_t size = sizeof(DirectionCommandData);
    if (Serial.available() < size)
    {
        Serial.print(F("readDirectionCommandData: insufficient_bytes_available: required="));
        Serial.println(size);
        return false;
    }

    uint8_t dataBuffer[size];
    for (uint8_t i = 0; i < size; i++)
    {
        dataBuffer[i] = Serial.read();
    }
    directionCommandData = (DirectionCommandData *)dataBuffer;

    Serial.print(F("readDirectionCommandData: data_read: turnType="));
    Serial.print(directionCommandData->turnType);
    Serial.print(F(", distanceM="));
    Serial.println(directionCommandData->distanceM);

    return true;
}

void displayClear()
{
    Epd epd;
    epd.Init();
    epd.ClearMemory();
    // Reliable cleanup.
    for (int i = 0; i < 3; i++)
    {
        epd.DisplayFrameInversed();
    }
    epd.Sleep();
    epd.PowerOff();
}

void displayShowDirection()
{
    Epd epd;
    epd.Init();
    epd.ClearMemory();

    const uint8_t turnImageTop = 12;
    const uint8_t distanceStringTop = EPD_HEIGHT - 40 - 12;

#pragma region Display turn
    switch (directionCommandData->turnType)
    {
    // C, continue
    case 1:
        epd.SetFrameMemory(turn_c.data_pgm, true, (EPD_WIDTH - turn_c.width) / 2, turnImageTop,
                           turn_c.width, turn_c.height, false);
        break;
    // TL, turn left
    case 2:
        epd.SetFrameMemory(turn_tl.data_pgm, true, (EPD_WIDTH - turn_tl.width) / 2, turnImageTop,
                           turn_tl.width, turn_tl.height, false);
        break;
    // TSLL, turn slightly left
    case 3:
        epd.SetFrameMemory(turn_tsll.data_pgm, true, (EPD_WIDTH - turn_tsll.width) / 2, turnImageTop,
                           turn_tsll.width, turn_tsll.height, false);
        break;
    // TSHLL, turn sharp left
    case 4:
        epd.SetFrameMemory(turn_tshl.data_pgm, true, (EPD_WIDTH - turn_tshl.width) / 2, turnImageTop,
                           turn_tshl.width, turn_tshl.height, false);
        break;
    // TR, turn right
    case 5:
        epd.SetFrameMemory(turn_tl.data_pgm, true, (EPD_WIDTH - turn_tl.width) / 2, turnImageTop,
                           turn_tl.width, turn_tl.height, true);
        break;
    // TSLR, turn slightly right
    case 6:
        epd.SetFrameMemory(turn_tsll.data_pgm, true, (EPD_WIDTH - turn_tsll.width) / 2, turnImageTop,
                           turn_tsll.width, turn_tsll.height, true);
        break;
    // TSHLL, turn sharp right
    case 7:
        epd.SetFrameMemory(turn_tshl.data_pgm, true, (EPD_WIDTH - turn_tshl.width) / 2, turnImageTop,
                           turn_tshl.width, turn_tshl.height, true);
        break;
    // TU, U-turn
    case 10:
        epd.SetFrameMemory(turn_tu.data_pgm, true, (EPD_WIDTH - turn_tu.width) / 2, turnImageTop,
                           turn_tu.width, turn_tu.height, false);
        break;
    // TRU, right U-turn
    case 11:
        epd.SetFrameMemory(turn_tu.data_pgm, true, (EPD_WIDTH - turn_tu.width) / 2, turnImageTop,
                           turn_tu.width, turn_tu.height, true);
        break;
    // Not supported – small black square
    default:
        unsigned char empty[] = {0xFF, 0XFF};
        epd.SetFrameMemory(empty, false, 98, turnImageTop, 4, 4, false);
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
    epd.SetFrameMemory(paint.GetImage(), false, 0, 160, paint.GetWidth(), paint.GetHeight(), false);

#pragma endregion

    epd.DisplayFrameInversed();

    epd.Sleep();
    epd.PowerOff();
}

void loop()
{
    tryToReadCommand();
}
