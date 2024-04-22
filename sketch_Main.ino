#include <TM1637Display.h> // Inkluder biblioteket til Grove 4-cifret display

// Definer den analoge pin, hvor flex sensoren er tilsluttet
const int flexPin1 = A2; // Flex sensor tilsluttet til A2
const int flexPin2 = A3; // Flex sensor tilsluttet til A3
const int flashlightPin = A1; // Pin, hvor lommelygten er tilsluttet
#define trigPin 2 // HC-SR04 trigger pin
#define echoPin 3 // HC-SR04 echo pin
#define CLK 4 // Clock pin til Grove 4-cifret display
#define DIO 5 // Data pin til Grove 4-cifret display

// Variabel til at holde styr på lommelygtens tilstand
bool flashlightState = false;

TM1637Display display(CLK, DIO); // Initialiser Grove 4-cifret display

void setup() {
  Serial.begin(9600);
  pinMode(flexPin1, INPUT); // Flex sensor pin som input
  pinMode(flexPin2, INPUT); // Flex sensor pin som input
  pinMode(flashlightPin, OUTPUT); // Lommelygten pin som output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(flashlightPin, HIGH); // Sørg for at lommelygten starter i slukket tilstand

  display.setBrightness(0x0a); // Sæt lysstyrkeniveauet for displayet (0x00 til 0x0f)
  display.clear(); // Ryd displayet
}

void loop() {
  // Læs den analoge værdi fra flex sensorerne
  int flexValue1 = analogRead(flexPin1);
  int flexValue2 = analogRead(flexPin2);

  // Udskriv flex sensor aflæsningerne til Serial Monitor
  Serial.print("Flex Sensor 1 Value: ");
  Serial.println(flexValue1);
  Serial.print("Flex Sensor 2 Value: ");
  Serial.println(flexValue2);

  // Tjek om flex sensor 1 aflæsning er under 400 (juster tærsklen efter behov)
  if (flexValue1 < 350) {
    // Skift lommelygtens tilstand mellem konstant og slukket
    flashlightState = !flashlightState;

    // Indstil lommelygtepin baseret på den opdaterede tilstand
    digitalWrite(flashlightPin, flashlightState ? HIGH : LOW);

    // Tilføj en ekstra forsinkelse for at forhindre hurtig skiften
    delay(500);
  }

  // Tjek om flex sensor 2 aflæsning er under 220 (juster tærsklen efter behov)
  if (flexValue2 < 220) {
    // Mål afstand og skift koden
    long duration, distance;

    // Ryd triggerpin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sæt triggerpin høj i 10 mikrosekunder
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Mål varigheden af pulsen på echo pin
    duration = pulseIn(echoPin, HIGH);

    // Beregn afstanden
    distance = duration * 0.034 / 2; // Lydens hastighed er 34.000 cm/s, divideret med 2, da lyden rejser til objektet og tilbage

    // Udskriv afstanden til serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Opdater Grove 4-cifret display med afstandsværdien
    display.showNumberDec(distance, false); // Vis afstandsværdien
    delay(1000); // Vent 1 sekund før næste måling
  }
}
