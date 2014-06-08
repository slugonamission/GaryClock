#include "voltmeter.h"

Voltmeter::Voltmeter(void) {

}

void Voltmeter::begin(int pin) {
  pwmPin = pin;

  pinMode(pwmPin, OUTPUT);
}

void Voltmeter::moveDamped(int desiredPosition) {
	moveDamped(desiredPosition, position);
}

void Voltmeter::moveDamped(int desiredPosition, int currentPosition) {
	int damping = currentPosition - desiredPosition;

	analogWrite(pwmPin, desiredPosition);
	delay(DAMP_DELAY);
	analogWrite(pwmPin, desiredPosition + damping);
	delay(DAMP_AMOUNT);
	analogWrite(pwmPin, desiredPosition);

	position = desiredPosition;
}

/*
void Voltmeter::moveMultipleDamped(int pins[], int count, int currentPosition, int desiredPosition) {
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
*/