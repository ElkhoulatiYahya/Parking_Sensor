
// ---- Pinout ----
int tonePin  = 4;   // Buzzer (Red Jumper)
int trigPin  = 9;   // HC-SR04 Trig (Violet Jumper)
int echoPin  = 10;  // HC-SR04 Echo (Yellow Jumper)

int clockPin = 11;  // 74HC595 SH_CP (White Jumper)
int latchPin = 12;  // 74HC595 ST_CP (Blue Jumper)
int dataPin  = 13;  // 74HC595 DS (Green Jumper)

// ---- LED Patterns (8 LEDs) ----
// Each bit = LED state (1 = ON, 0 = OFF)
byte possible_patterns[9] = {
  B00000000,  // 0 LEDs
  B00000001,  // 1 LED
  B00000011,  // 2 LEDs
  B00000111,  // 3 LEDs
  B00001111,  // 4 LEDs
  B00011111,  // 5 LEDs
  B00111111,  // 6 LEDs
  B01111111,  // 7 LEDs
  B11111111   // 8 LEDs
};

// ---- Variables ----
int proximity = 0;
long duration;
int distance;

void setup() {
  // Serial Monitor (debug)
  Serial.begin(9600);

  // Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Shift register
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Buzzer
  pinMode(tonePin, OUTPUT);
}

void loop() {
  // ---- Measure distance ----
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1; // Convert to cm

  // ---- Map distance → proximity (0..8) ----
  proximity = map(distance, 0, 45, 8, 0);

  // Clamp value
  if (proximity < 0) proximity = 0;
  if (proximity > 8) proximity = 8;

  // ---- Debug ----
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | LEDs: ");
  Serial.println(proximity);

  // ---- Buzzer alerts ----
  if (proximity >= 3 && proximity <= 4) {
    tone(tonePin, 2000, 200); // Medium frequency
  }
  else if (proximity >= 5 && proximity <= 6) {
    tone(tonePin, 500, 200);  // Lower frequency
  }
  else if (proximity >= 7 && proximity <= 8) {
    tone(tonePin, 100, 200);  // Very low frequency
  }

  // ---- LEDs display via 74HC595 ----
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, possible_patterns[proximity]);
  digitalWrite(latchPin, HIGH);

  delay(600);
  noTone(tonePin);
}
