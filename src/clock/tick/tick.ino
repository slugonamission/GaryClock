/**
 * Testing damping on the Voltmeter - stops some of the intertia from the heavy needle
 **/

#define SECS_PIN  10
#define MINS_PIN  9
#define HOURS_PIN 5
int allHands[] = {SECS_PIN, MINS_PIN, HOURS_PIN};

#define TICK_TIME 4000

// Damping parameters
#define USE_DAMPING   1
#define DAMP_DELAY    120
#define DAMP_AMOUNT   69
#define DAMP_DURATION (DAMP_DELAY + DAMP_AMOUNT)


void setup() { 
  // Let the needle settle at 0
  delay(1000);

/*
  moveDamped(HOURS_PIN, 0, 127);
  moveDamped(MINS_PIN, 0, 127);
  moveDamped(SECS_PIN, 0, 127);

  delay(10000);*/
} 


void loop() {

  // 5V ticks
  //ticks(1);

  // 2.5V ticks
  ticks(2);
 /*
  // 1V ticks
  ticks(5);

  // 0.5V ticks
  ticks(10);

  // 0.25V ticks
  ticks(20);

  // 0.125V ticks
  //ticks(40);


  // Ticking clock test - 0 to 59 then reset
  ticksUp(59);
  #if(USE_DAMPING)
    moveMultipleDamped(allHands, 3, 255, 0);
    delay(1000 - DAMP_DURATION);
  #else
    analogWrite(HOURS_PIN, 0);
    analogWrite(MINS_PIN, 0);
    analogWrite(SECS_PIN, 0);
    delay(1000);
  #endif


  // Smooth clock test - 1 minute sweep then reset
  for(int fadeValue = 1; fadeValue <= 255; fadeValue++) {
    analogWrite(HOURS_PIN, fadeValue);
    analogWrite(MINS_PIN, fadeValue);
    analogWrite(SECS_PIN, fadeValue);
    delay(235);
  }
  #if(USE_DAMPING)
    moveMultipleDamped(allHands, 3, 255, 0);
    delay(235 - DAMP_DURATION);
  #else
    analogWrite(HOURS_PIN, 0);
    analogWrite(MINS_PIN, 0);
    analogWrite(SECS_PIN, 0);
    delay(235);
  #endif
  */
}


void ticks(int count) {
  ticksUp(count);
  ticksDown(count);
}


void ticksUp(int count) {
  for(int fadeValue = 1; fadeValue <= count; fadeValue++) {
    #if(USE_DAMPING)
      moveMultipleDamped(allHands, 3, (fadeValue - 1) * 255/count, fadeValue * 255/count);
      delay(TICK_TIME - DAMP_DURATION);
    #else
      analogWrite(HOURS_PIN, fadeValue * 255/count);
      analogWrite(MINS_PIN, fadeValue * 255/count);
      analogWrite(SECS_PIN, fadeValue * 255/count);
      delay(TICK_TIME);
    #endif
  } 
}


void ticksDown(int count) {
  for(int fadeValue = count - 1; fadeValue >= 0; fadeValue--) { 
    #if(USE_DAMPING)
      moveMultipleDamped(allHands, 3, (fadeValue + 1) * 255/count, fadeValue * 255/count);
      delay(TICK_TIME - DAMP_DURATION);
    #else
      analogWrite(HOURS_PIN, fadeValue * 255/count);
      analogWrite(MINS_PIN, fadeValue * 255/count);
      analogWrite(SECS_PIN, fadeValue * 255/count);
      delay(TICK_TIME);
    #endif
  } 
}


void moveDamped(int pin, int currentPosition, int desiredPosition) {
  int damping = currentPosition - desiredPosition;

  analogWrite(pin, desiredPosition);
  delay(DAMP_DELAY);
  analogWrite(pin, desiredPosition + damping);
  delay(DAMP_AMOUNT);
  analogWrite(pin, desiredPosition);
}

void moveMultipleDamped(int pins[], int count, int currentPosition, int desiredPosition) {
  int damping = currentPosition - desiredPosition;

  for (int i = 0; i < count; i++)
    analogWrite(pins[i], desiredPosition);
  delay(DAMP_DELAY);
  for (int i = 0; i < count; i++)
    analogWrite(pins[i], desiredPosition + damping);
  delay(DAMP_AMOUNT);
  for (int i = 0; i < count; i++)
    analogWrite(pins[i], desiredPosition);
}

