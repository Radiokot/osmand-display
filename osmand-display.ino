#include "commands.h"

void setup()
{
    powerOnSetup();
}

void powerOnSetup()
{
    Serial.begin(9600);
    Serial.println(F("powerOnSetup: serial_set_up"));
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
    DirectionCommandData *data = (DirectionCommandData *)dataBuffer;

    Serial.print(F("readDirectionCommandData: data_read: turnType="));
    Serial.print(data->turnType);
    Serial.print(F(", distanceM="));
    Serial.println(data->distanceM);

    return true;
}

void loop()
{
    tryToReadCommand();
}
