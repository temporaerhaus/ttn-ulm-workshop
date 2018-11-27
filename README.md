# TTN Ulm Workshop

Dieses Dokument begleitet unsere Ulmer LoRaWAN Workshop mit hilfreichen Informationen und begleitenden Anleitungen zur Soft- und Hardware sowie den benötigten Accounts bei TTN. 



## Die Münster-Platine

Unsere Münster-Platine ist eine LoRaWAN Node zum Übersenden von (in der Regel) Messdaten von Sensoren über ein LoRaWAN Netzwerk. In Ulm ist ein solches Netzwerk vorhanden (via mehreren Basisstationen in der Stadt), sowie auch in vielen anderen Städten und Ballungsgebieten und Deutschland und weltweit. [Diese Karte](https://www.thethingsnetwork.org/map) gibt einen Überblick. 

Unsere Platine hat folgende Features:

- Flashen (Datenübertragung) von Software via USB
- Laden des Akkus via USB
- Akku-Anschluss via JST Stecker
- Arduino Pro Mini (Atmel328) Mikrokontroller
- RFM95 LoRaWAN Chip
- Eine ungewöhnliche Form

### Sensoren im Workshop

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

