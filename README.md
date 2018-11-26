# TTN Ulm Workshop


## Accounts

Zum Abruf der über LoRaWAN übertragenen Daten nutzen wir das The Things Network, kurz TTN. Dort ist ein Account nötig, der auf dieser Seite erstellt werden kann: https://account.thethingsnetwork.org/register. Es können beliebige, auch anonymisierte Daten verwendet werden.




## Münster-Platine

- Flashen (Datenübertragung) und Laden
- Laden mit USB
- Akku via JST Stecker
- Arduino Pro Mini
- RFM95 für Lora

### Sensoren

Ein kurzer Überblick über die Sensoren und Anschlüsse an den Arduino Pro Mini. 

1. BME280
    * Anschließen: VIN an VCC, GND an GND, SCL an PIN 5, SDA an Pin 4.
2. Magnetschalter
    * Anschließen: Ein Kabel an Pin 3, das andere an GND. 
3. Wassersensor
    * Anschliesen: Gelb an Pin 2, Rot an VCC, Schwarz an GND. Weiss nicht anschließen.
4. Bodenfeuchtesensor

### Arduino IDE
* Board einstellen
    * Tools -> Board -> Arduino Pro or Pro Mini
    * CPU: Atmega328, 3.3V, 8Mhz

### Code

Den gesamten Code, den wir im folgenden auf den Mikrocontroller spielen, ist ein einer einzigen Datei enthalten. Diese kann kopiert (Copy & Paste) oder heruntergeladen werden und danach lokal in die Arduino IDE eingefügt werden.

()[]

### Flashen ("den Arduino programmieren").

