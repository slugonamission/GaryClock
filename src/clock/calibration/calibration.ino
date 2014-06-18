/**
 * For calibrating the voltmeters. Prints values over Serial. Use 'q, w and e' to increase and 'a, s and d' to decrease.
 **/

#define RIGHT_PIN  10
#define LEFT_PIN  9
#define MID_PIN 5

uint8_t values[] = {0, 0, 0};


void setup() { 
  // Let the needle settle at 0
  delay(1000);

  Serial.begin(9600);

  pinMode(RIGHT_PIN, OUTPUT);
  pinMode(LEFT_PIN, OUTPUT);
  pinMode(MID_PIN, OUTPUT);

  analogWrite(LEFT_PIN, values[0]);
  Serial.print(values[0]);
  Serial.println(F("L"));
  analogWrite(MID_PIN, values[1]);
  Serial.print(values[1]);
  Serial.println(F("M"));
  analogWrite(RIGHT_PIN, values[2]);
  Serial.print(values[2]);
  Serial.println(F("R"));
} 


void loop() {
  if (Serial.available() > 0) {
    uint8_t input = Serial.read();

    if (input == 'q') {
      values[0]++;
      analogWrite(LEFT_PIN, values[0]);
      Serial.print(values[0]);
      Serial.println(F("L"));
    }
    else if (input == 'a') {
      values[0]--;
      analogWrite(LEFT_PIN, values[0]);
      Serial.print(values[0]);
      Serial.println(F("L"));
    }
    else if (input == 'w') {
      values[1]++;
      analogWrite(MID_PIN, values[1]);
      Serial.print(values[1]);
      Serial.println(F("M"));
    }
    else if (input == 's') {
      values[1]--;
      analogWrite(MID_PIN, values[1]);
      Serial.print(values[1]);
      Serial.println(F("M"));
    }
    else if (input == 'e') {
      values[2]++;
      analogWrite(RIGHT_PIN, values[2]);
      Serial.print(values[2]);
      Serial.println(F("R"));
    }
    else if (input == 'd') {
      values[2]--;
      analogWrite(RIGHT_PIN, values[2]);
      Serial.print(values[2]);
      Serial.println(F("R"));
    }
    else {
      Serial.println(F("Use 'q, w and e' to increase and 'a, s and d' to decrease"));
    }
  }
}
