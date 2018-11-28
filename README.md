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
- Eine ungewöhnliche Form :)

## Allgemeine Informationen

### Sensoren im Workshop

Ein kurzer Überblick über die verwendeten Sensoren und Anschlüsse im Workshop.  Wir haben aktuell vier verschiedene Sensoren, ausgesucht im einen halbwegs breiten Anwendungszweck abzudecken. 

1. BME280 - Temperatur, Luftdruck, Luftfeuchtigkeit
    * Anschließen: VIN an VCC, GND an GND, SCL an PIN 5, SDA an Pin 4.
2. Magnetschalter - Tür auf/zu, etc.
    * Anschließen: Ein Kabel an Pin 3, das andere an GND. 
3. Wassersensor - Regen, Überschwemmung, etc.
    * Anschliesen: Gelb an Pin 2, Rot an VCC, Schwarz an GND. Weiss nicht anschließen.
4. Bodenfeuchtesensor - Feuchtigkeit in Blumenerde, etc.
    * Anschliesen: Gelb an PIN 7, Rot an VCC, Schwarz an GND.

Alle Sensoren werden über kleine Käbelchen (Jumper-Wires) an den Arduino, das ist der blaue Baustein auf der Platine, angeschlossen. Die Verdrahtung ("was an was") ist oben kurz beschrieben.

### Arduino IDE

Die Arduino IDE dient der Programmierung der Münsterplatine bzw. genauer dem Arduino. Diese muss installiert werden (Download [hier](https://www.arduino.cc/en/Main/Software)) und anschließend ist die Konfiguration des von uns verwendeten Boards nötig:

* Board einstellen via Menü
    * Tools -> Board, Dann dort *Arduino Pro or Pro Mini*
    * CPU: *Atmega328, 3.3V, 8Mhz*

Danach wird der Code in [diesem Sketch](https://github.com/verschwoerhaus/ttn-ulm-workshop/blob/master/workshop.ino), oder siehe nächster Abschnitt, in die Arduino IDE eingefügt (copy & paste). Eventuell dort schon vorhandener Code kann gelöscht werden.

### Code

Der gesamte Code ist ein einer einzigen Datei enthalten (*Sketch* genannt). Diese kann kopiert (Copy & Paste) oder heruntergeladen werden und danach lokal in die Arduino IDE eingefügt werden.

Den Sketch für unseren Workshop findet man hier:

[https://github.com/verschwoerhaus/ttn-ulm-workshop/blob/master/workshop.ino](https://github.com/verschwoerhaus/ttn-ulm-workshop/blob/master/workshop.ino)

## Ablauf

Unser Workshop im Überblick:

1. Kurzer Vortrag zu LoRaWAN, TTN und wozu das alles überhaupt da ist
2. Platine kennen lernen
3. Arduino, RFM95 und Antenne auflöten
4. Sensor auswählen
5. Sensor(en) anschließen
6. App und Device bei TTN erstellen und Keys kopieren
7. Sketch in Arduino IDE laden, Keys einfügen und auf Sensor anpassen (einfaches auskommentieren)
8. Flashen
9. Ankommende Daten in der TTN Console betrachten und ggf. konvertieren

Die folgenden Schritte sind absichtlich sehr kurz gehalten, manche Details sind dabei sicherlich erst unklar. Nachfragen hierzu können gerne direkt im Workshop gestellt werden. 

### Schritt 1: Vortrag

Die Folien gibt es zum nach Nachlesen (nach dem Workshop) [hier](todo).

###  Schritt 2 und 3: Löten
Es müssen drei Komponenten aufgelötet werden:
* Arduino Pro Mini
    1. Auflöten der Pin-Header auf den Arduino
    2. Auflöten des Arduinos mit den Pin-Headern auf die Platine
* RFM95 LoRaWAN Chip
    1. Ausrichtung: Wenn das Münster richtig herum vor einem liegt, ist der schwarze "Block" auf dem RFM95 rechts unten.
* Helix-Antenne
   * Diese ist etwas trickreich am mittleren Pad des Antennenkontakts aufzulöten. Hier unterstützen wir gerne. Ausrichtung der Antenne ist mehr oder weniger egal. 

### Schritt 4 und 5: Sensoren aussuchen und anschließen
Es kann einer (oder mehrere) der oben genannten Sensoren ausgewählt und angeschlossen werden. Die Belegung wird in der oberen Liste kurz aufgeführt und mit den Jumper-Kabeln verbunden.  

### Schritt 6: TTN App und Device 
1. Registrierung bei TTN https://account.thethingsnetwork.org/register 
2. App anlegen: Applicaitons -> Add Application
    * Application ID: Beliebiger Name wählbar
    * Handler registration: ttn-handler-eu
    * Rest auf Standard
3. Device anlegen
    * Devices -> Register device
    * Device ID: Beliebiger Name wählbar
    * Device EUI: Generieren lassen (Doppelpfeil anklicken)
    * Rest auf Standard   

### Schritt 7 und 8: Sketch bearbeiten
1. Die *Device EUI*, *Application EUI* und den *App Key* kopieren und an die entsprechenden Stellen im Sketch einfügen
2. Wir verwenden OTAA Aktivierung, d.h. im Sketch muss dafür nichts mehr getan werden. Falls ABP verwendet werden soll, kann die USE_OTAA Variable entsprechend auf *false* gesetzt werden.
3. Dann, entsprechend der Sensorwahl von oben, werden die Sensoren aktiviert:
    * Wassersensor: USE_WATER_SENSOR auf *true*
    * BME280: USE_BME280_SENSOR auf *true*
    * Türsensor: USE_MAGNET_SENSOR auf *true*
    * Feuchtesensor: USE_MOIST_SENSOR auf *true*
4. Platine via USB an den PC anschließen und in der Arduino IDE auf *Upload* klicken. 

### Schritt 9:  Daten abrufen
In der TTN Console können dann die versendeten Daten im Rohformat betrachtet werden (Tab *Data*).

Um die Daten für den Menschen lesbar auszugeben, kann für die angelegte TTN App ein Payload Decoder angegeben werden:

Für den BME280 Sensor:

    function Decoder(bytes, port) {

      var temp = (bytes[0] + (bytes[1] << 8) + (bytes[2] << 16)) / 100;
      var pressure = (bytes[3] + (bytes[4] << 8) + (bytes[5] << 16)) / 100;
      var hum = (bytes[6] + (bytes[7] << 8) + (bytes[8] << 16)) / 100;

      var decoded = {};
      decoded.temp = temp;
      decoded.pressure = pressure;
      decoded.hum = hum;

      return decoded;
    }

Für die anderen Sensoren:

    function Decoder(bytes, port) {
      var batt = (bytes[0] << 8) | bytes[1];
      var decoded = {};
      decoded.value = value;
      return decoded;
    }

Einzufügen unter *Payload Formats* in der Section *Decoder*. Danach erscheinen die Daten gut lesbar in der Console.

 

