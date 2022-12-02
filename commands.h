#define COMMAND_DIRECTION 0x10

typedef struct DirectionCommandData
{
    uint8_t turnType;
    uint32_t distanceM;
} DirectionCommandData;
