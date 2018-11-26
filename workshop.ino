/***********************************************************************

Hello World für die Münsterplatine (TTN Ulm)
============================================

Einfaches Beispiel für die Übertragung der ersten LoRaWAN-Pakete. Dafür
muss nur der mit TODO markierte Block angepasst werden.

Mehr zu TTN und der Platine unter:
https://lora.ulm-digital.com/documentation/minster-node

Dieser Beispielcode basiert auf dem in der Arduino-LMIC Bibliothek,
Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman:
https://github.com/matthijskooijman/arduino-lmic
-> examples/ttn-abp/ttn-abp.ino

***********************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>


/* TODO TTN-Anmeldedaten für das Device eintragen
**************************************************/

/*************************************************
Man hat die Wahl zwischen zwei Aktivierungsmethoden:

Over-the-Air Activation (OTAA)
Die empfohlene und sicherste Variante, die die Schlüssel
beim Beitritt aushandelt.

Activation by Personalization (ABP)
Einfachere, leicht schnellere aber unsicherer Variante,
die die Schlüssel im Device verdrahtet.
**************************************************/
#define USE_OTAA true  // 'false' für ABP, 'true' für OTAA


#if USE_OTAA == false
//*****************
// ABP Methode
//*****************
// LoRaWAN NwkSKey = network session key, als MSB!!
static const PROGMEM u1_t NWKSKEY[16] = { 0x6F, 0xFB, 0x60, 0xFE, 0x42, 0x3D, 0x1D, 0x49, 0xBF, 0x11, 0x5B, 0x1E, 0x63, 0xF7, 0x52, 0xBB }; // Hier...

// LoRaWAN AppSKey = application session key, als MSB!!
static const u1_t PROGMEM APPSKEY[16] = { 0x03, 0x87, 0xAA, 0x9E, 0xA3, 0x8C, 0x97, 0xB9, 0xA0, 0x12, 0x59, 0x02, 0xE3, 0x51, 0x77, 0xF5 }; // ...und hier

// LoRaWAN DevAddr = end-device address
static const u4_t DEVADDR = 0x26011025; // ... und hier.

// Das sieht dann ungefähr so aus:
//static const PROGMEM u1_t NWKSKEY[16] = { 0x1B, 0x1B, 0xF8, 0x14, 0x3B, 0x77, 0x26, 0x58, 0xF3, 0x88, 0x4E, 0x1C, 0xB1, 0xAB, 0x04, 0xGC };
//static const u1_t PROGMEM APPSKEY[16] = { 0xD1, 0xC3, 0xB9, 0x23, 0xF4, 0x65, 0x0A, 0x9A, 0x24, 0x90, 0x0A, 0x14, 0xA9, 0x8F, 0x1F, 0xA1 };
//static const u4_t DEVADDR = 0x26011F9I;

// unused, but needed variables
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
#endif


#if USE_OTAA == true
//*****************
// OTAA Methode
//*****************
// Device EUI, als lsb
static const u1_t PROGMEM DEVEUI[8] = { 0xFE, 0x27, 0x9C, 0x0D, 0xA5, 0x2F, 0xCE, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// Application EUI, als lsb
static const u1_t PROGMEM APPEUI[8] = { 0xD7, 0x4E, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

static const u1_t PROGMEM APPKEY[16] = { 0xC6, 0x3A, 0x30, 0xC7, 0x58, 0xB7, 0xCA, 0xA8, 0x67, 0x74, 0x71, 0x66, 0xA5, 0xB2, 0x44, 0x68 };
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}
#endif



//************************
// Sensoren
//************************

#define USE_WATER_SENSOR false
#if USE_WATER_SENSOR == true
#define WATER_SENSOR_PIN 2
#endif

#define USE_MAGNET_SENSOR false
#if USE_MAGNET_SENSOR == true
#define MAGNET_SENSOR 3
#endif

#define USE_BME280_SENSOR true
#if USE_BME280_SENSOR == true
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme; // I2C
#define SEALEVELPRESSURE_HPA (1010.00)
#endif



/* Sonstige Einstellungen
**************************/

// Sendeintervall in Sekunden:
const unsigned TX_INTERVAL = 10;

// Datenrate bzw. spreading factor:
#define dataRate DR_SF7
// Sendeleistung:
int txPower = 14;

// Pin-Zuordung für LoRa-Modul (RFM95):
const lmic_pinmap lmic_pins = {
        .nss = 10,                // TTN Ulm Münsterplatine: 10
        .rxtx = LMIC_UNUSED_PIN,
        .rst = LMIC_UNUSED_PIN,
        .dio = {4, 5, 6},         // TTN Ulm Münsterplatinge: {4, 5 ,6}
};


static osjob_t sendjob;

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
                Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
                Serial.println(F("Received "));
                Serial.println(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {

        //*************************
        // Sensoren auslesen
        //*************************
        #if USE_WATER_SENSOR == true
        Serial.println(digitalRead(WATER_SENSOR_PIN));
        #endif

        #if USE_MAGNET_SENSOR == true
        if (digitalRead(MAGNET_SENSOR) == HIGH){
            Serial.println("Your Door is Open");
        } else {
            Serial.println("Your Door is Closed");
        }
        #endif

        #if USE_BME280_SENSOR == true
        Serial.print("Temperature = ");
        Serial.print(bme.readTemperature());
        Serial.println(" *C");

        Serial.print("Pressure = ");

        Serial.print(bme.readPressure() / 100.0F);
        Serial.println(" hPa");

        Serial.print("Approx. Altitude = ");
        Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial.println(" m");

        Serial.print("Humidity = ");
        Serial.print(bme.readHumidity());
        Serial.println(" %");
        #endif

        int aNumber = 16153;            // Höhe des Ulmer Münsters in cm.
                                        // 16153 entspricht 3F19 in hexadezimaler Darstellung

        byte payload[2];
        payload[0] = highByte(aNumber);
        payload[1] = lowByte(aNumber);

        LMIC_setTxData2(1, (uint8_t*)payload, sizeof(payload), 0);

        Serial.println(F("Packet queued"));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println(F("Starting"));

    //*************************
    // Sensoren initialisieren
    //*************************
    #if USE_WATER_SENSOR == true
    pinMode(WATER_SENSOR_PIN, INPUT);
    #endif

    #if USE_MAGNET_SENSOR == true
    pinMode(MAGNET_SENSOR, INPUT_PULLUP);
    #endif

    #if USE_BME280_SENSOR == true
    bool status;
    status = bme.begin(0x76);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    #endif


    //**********************
    // LMIC (TTN)
    //**********************
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();


    #if USE_OTAA == false
    #ifdef PROGMEM
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif
    #endif

    #if defined(CFG_eu868)
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    #elif defined(CFG_us915)
    LMIC_selectSubBand(1);
    #endif


    #if USE_OTAA == true
    // make OTAA JOINING work
    // https://github.com/matthijskooijman/arduino-lmic/issues/122
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(dataRate, txPower);

    // Start job
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}
