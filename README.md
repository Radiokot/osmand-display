# OsmAnd Display

## Commands
| Name | Description | Code | Payload | Example |
| -----|-------------|------|---------|---------|
|Direction|Shows given direction|`0x10`|1 byte [OsmAnd turn type](https://github.com/osmandapp/OsmAnd/blob/master/OsmAnd-java/src/main/java/net/osmand/router/TurnType.java); 4 bytes of unsigned short distance in meters, **Little endian**| `0x10 0x02 0x04 0x1F 0x00 0x00` – Turn left in 7940 m.|
|Clear|Clears the display|`0x20`|-|`0x20`|
|Frame prepare|Prepares the display for writing the frame data|`0x30`|-|`0x30`|
|Frame data|Writes given data to the display memory. Has no log output. Must be called after `Frame prepare`|`0x31`|1 byte data size, no more than 64; frame data|`0x31 0x01 0xFF` – Writes 1 byte of 8 black dots|
|Frame show|Shows whatever is in the display memory. Must be called after `Frame prepare`|`0x32`|-|`0x32`|
