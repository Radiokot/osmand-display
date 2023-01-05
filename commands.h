#ifndef __COMMANDS_H
#define __COMMANDS_H

#define COMMAND_DIRECTION       0x10
#define COMMAND_CLEAR           0x20
#define COMMAND_FRAME_PREPARE   0x30
#define COMMAND_FRAME_DATA      0x31
#define COMMAND_FRAME_SHOW      0x32

#define COMMAND_FRAME_DATA_MAX_SIZE 62

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