#ifndef __COMMANDS_H
#define __COMMANDS_H

#define COMMAND_DIRECTION 0x10
#define COMMAND_CLEAR 0x20
#define COMMAND_FRAME 0x30

typedef struct DirectionCommandData
{
    /**
     * OsmAnd turn type.
    */
    uint8_t turnType;

    /**
     * Distance to the turn in meters.
    */
    uint32_t distanceM;
} DirectionCommandData;

#endif