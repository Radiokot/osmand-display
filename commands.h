#define COMMAND_DIRECTION 0x10

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
