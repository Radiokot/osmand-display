# OsmAnd Display
A wireless E-Ink display for OsmAnd navigation directions.
It receives directions from OsmAnd through the [Android app](https://github.com/Radiokot/osmand-display-app) and shows them on the display. The communication is done over the BLE serial.

<p float="left">
  <img src="https://user-images.githubusercontent.com/5675681/210539138-c739653d-2bbe-474f-b061-48d86704e82c.png" width="400" alt="Prototyping"/>
  <img src="https://user-images.githubusercontent.com/5675681/212536029-b758ba95-dd6e-4a5c-be77-41d6c6c35408.png" width="225" alt="On a bike"/>
</p>

## Key components
- **Arduino Pro Mini** – 5V version but the frequency is divided in 4
- **1.54inch e-Paper Module (B)** – no quite good option for the use-case, it is slow and temperature dependant. The display is used in BW mode as BWR full refresh takes **15 seconds**
- **JDY-23** BLE 5.0 module
- **Li-Po battery**
- **TP4056** battery charging module
- **AMS1117** voltage stabilizer

## Commands
| Name | Description | Code | Payload | Example |
| -----|-------------|------|---------|---------|
|Direction|Shows given direction|`0x10`|1 byte [OsmAnd turn type](https://github.com/osmandapp/OsmAnd/blob/master/OsmAnd-java/src/main/java/net/osmand/router/TurnType.java); 4 bytes of unsigned short distance in meters, **Little endian**| `0x10 0x02 0x04 0x1F 0x00 0x00` – Turn left in 7940 m.|
|Clear|Clears the display|`0x20`|-|`0x20`|
|Frame prepare|Prepares the display for writing the frame data|`0x30`|-|`0x30`|
|Frame data|Writes given data to the display memory. Has no log output. Must be called after `Frame prepare`|`0x31`|1 byte data size, no more than 62; frame data|`0x31 0x01 0xFF` – Writes 1 byte of 8 black dots|
|Frame show|Shows whatever is in the display memory. Must be called after `Frame prepare`|`0x32`|-|`0x32`|
