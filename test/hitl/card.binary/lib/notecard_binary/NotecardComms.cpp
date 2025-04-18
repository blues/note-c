
#include "NotecardComms.h"
#include <Arduino.h>

size_t aux_serial_baudrate = NOTECARD_IF_AUX_SERIAL_BAUDRATE;

#ifndef FEATHER_AUX_EN_PIN
#define FEATHER_AUX_EN_PIN D5
#endif

bool set_aux_serial_baudrate(size_t baudrate, NotecardInterface nif)
{
    pinMode(FEATHER_AUX_EN_PIN, OUTPUT);
    digitalWrite(FEATHER_AUX_EN_PIN, HIGH);

    if (nif==NOTECARD_IF_AUX_SERIAL) {
        dbgSerial.println("WARNING: trying to change aux serial baudrate over aux serial.");
    }
    initialize_notecard_interface(nif);

    J* req = NoteNewRequest("card.aux.serial");
    JAddStringToObject(req, "mode", "req");
    JAddNumberToObject(req, "rate", baudrate);
    J* rsp = NoteRequestResponseWithRetry(req, 10);
    bool success = false;
    if (rsp==nullptr) {
        dbgSerial.println("No response to `card.aux.serial`.");
    } else if (!JIsNullString(rsp, "err")) {
        dbgSerial.println("Error response to `card.aux.serial`.");
    }

    const char* mode = JGetString(rsp, "mode");
    size_t rate = JGetNumber(rsp, "rate");
    if (strcmp(mode, "req")) {
        dbgSerial.print("expected 'mode':'req', got ");
        dbgSerial.println(mode);
    } else if (rate != baudrate) {
        dbgSerial.print("expected 'rate':");
        dbgSerial.print(baudrate);
        dbgSerial.print(", got ");
        dbgSerial.println(rate);
    } else {
        aux_serial_baudrate = baudrate;
        success = true;
    }
    JDelete(rsp);
    return success;
}

bool initialize_notecard_interface(NotecardInterface iface)
{
    // Initialize the hardware I/O channel to the Notecard
    switch (iface) {
    default:
        dbgSerial.println("Unknown Notecard interface given.");
        return false;

    case NOTECARD_IF_AUX_SERIAL:
        // workaround a bug in note-arduino that doesn't call Serial.begin() when the serial port
        // is changed.
        NOTECARD_IF_AUX_SERIAL_PORT.end();
        NOTECARD_IF_AUX_SERIAL_PORT.begin(aux_serial_baudrate);
        notecard.begin(NOTECARD_IF_AUX_SERIAL_PORT, aux_serial_baudrate);
        return true;

    case NOTECARD_IF_SERIAL:
        notecard.begin(NOTECARD_IF_SERIAL_PORT, NOTECARD_IF_SERIAL_BAUDRATE);
        return true;

    case NOTECARD_IF_I2C:
        notecard.begin(NOTECARD_IF_I2C_ADDRESS);
        return true;
    }
}

#if 0 // Notecard.end() is vaporware ;-)
bool uninitialize_notecard_interface(NotecardInterface iface)
{
    // Initialize the hardware I/O channel to the Notecard
    switch (iface) {
    default:
        dbgSerial.println("Unknown Notecard interface given.");
        return false;

    case NOTECARD_IF_AUX_SERIAL:
        notecard.end(NOTECARD_IF_AUX_SERIAL_PORT, aux_serial_baudrate);
        return true;

    case NOTECARD_IF_SERIAL:
        notecard.end(NOTECARD_IF_SERIAL_PORT, NOTECARD_IF_SERIAL_BAUDRATE);
        return true;

    case NOTECARD_IF_I2C:
        notecard.end(NOTECARD_IF_I2C_ADDRESS);
        return true;
    }
}
#endif

extern "C" {
    uint32_t _cobsEncode(uint8_t *ptr, uint32_t length, uint8_t eop, uint8_t *dst);
}

size_t readDataUntilTimeout(Stream& serial, size_t timeout, uint8_t* buf, size_t bufLen, size_t dataLen, size_t& duration)
{

    // encode the data to match what's on the wire
    const size_t dataShift = (bufLen - dataLen);
    memmove(buf + dataShift, buf, dataLen);
    size_t encLen = _cobsEncode(buf + dataShift, dataLen, '\n', buf);
    buf[encLen] = '\n';



    size_t count = 0;
    size_t begin = millis();
    size_t matchIndex = 0;  // current index being matched in the input buffer
    for (;;) {

        size_t start = millis();
        while (!serial.available()) {
            if ((millis()-start)>timeout) {
                duration = millis()-begin;
                return count;
            }
        }

        while (serial.available()) {
            uint8_t ch = serial.read();
            if (buf[matchIndex]==ch) {
                matchIndex++;
                if (matchIndex>=bufLen) {
                    dbgSerial.print("matched the complete image at offset ");
                    dbgSerial.println(count, DEC);
                }
            } else {
                if (matchIndex>10) {
                    dbgSerial.print("matched ");
                    dbgSerial.print(matchIndex, DEC);
                    dbgSerial.print(" bytes at offset ");
                    dbgSerial.println(count, DEC);
                }
                matchIndex = 0;
            }
            count++;
        }
    }
}


HardwareSerial Serial2(A0,A3);
HardwareSerial Serial3(A5,A4); // A5 is RX, A4 is TX


