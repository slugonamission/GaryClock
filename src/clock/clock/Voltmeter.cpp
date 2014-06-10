#include "voltmeter.h"

Voltmeter::Voltmeter() {

}

void Voltmeter::begin(int pin) {
	pwmPin = pin;
	pinMode(pwmPin, OUTPUT);
}

void Voltmeter::move(int desiredPosition)
{
	analogWrite(pwmPin, desiredPosition);
	position = desiredPosition;
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


void Voltmeter::moveMultipleDamped(Voltmeter meters[], int count, int desiredPosition) {
	int damping[count];
	for (int i = 0; i < count; i++)
		damping[i] = meters[i].getPosition() - desiredPosition;

	for (int i = 0; i < count; i++)
		meters[i].move(desiredPosition);
	delay(DAMP_DELAY);
	for (int i = 0; i < count; i++)
		meters[i].move(desiredPosition + damping[i]);
	delay(DAMP_AMOUNT);
	for (int i = 0; i < count; i++)
		meters[i].move(desiredPosition);
}

void Voltmeter::moveMultipleDamped(Voltmeter meters[], int count, int desiredPositions[]) {
	int damping[count];
	for (int i = 0; i < count; i++)
		damping[i] = meters[i].getPosition() - desiredPositions[i];

	for (int i = 0; i < count; i++)
		meters[i].move(desiredPositions[i]);
	delay(DAMP_DELAY);
	for (int i = 0; i < count; i++)
		meters[i].move(desiredPositions[i] + damping[i]);
	delay(DAMP_AMOUNT);
	for (int i = 0; i < count; i++)
		meters[i].move(desiredPositions[i]);
}
