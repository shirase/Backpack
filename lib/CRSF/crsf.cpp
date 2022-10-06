#include "crsf.h"

static uint8_t crsfInBuffer[CRSF_FRAME_SIZE_MAX];
static uint8_t crsfInBufferOffset = 0;
static crsfPacket_t crsfPacket;

bool CRSF::crsfProcessReceivedByte(uint8_t byte)
{
    if (crsfInBufferOffset == 0) {
        if (byte != CRSF_TELEMETRY_SYNC_BYTE) {
            return false;
        }
    }

    crsfInBuffer[crsfInBufferOffset++] = byte;

    if (crsfInBufferOffset < 2) {
        return false;
    }
    else if (crsfInBufferOffset == 2) {
        if (CRSF_FRAME_SIZE(crsfInBuffer[CRSF_TELEMETRY_LENGTH_INDEX]) > CRSF_FRAME_SIZE_MAX) {
            crsfInBufferOffset = 0;
            return false;
        }
    }

    if (crsfInBufferOffset == CRSF_FRAME_SIZE(crsfInBuffer[CRSF_TELEMETRY_LENGTH_INDEX])) {
        crsfInBufferOffset = 0;

        crsfPacket.type = crsfInBuffer[CRSF_TELEMETRY_TYPE_INDEX];
        crsfPacket.payloadSize = crsfInBuffer[CRSF_TELEMETRY_LENGTH_INDEX];
        crsfPacket.payload = crsfInBuffer;

        return true;
    }

    return false;
}

crsfPacket_t* CRSF::crsfGetReceivedPacket()
{
    return &crsfPacket;
}
