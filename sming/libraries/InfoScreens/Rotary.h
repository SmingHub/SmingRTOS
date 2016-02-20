/*
 * Rotary.h
 *
 *  Created on: Dec 5, 2015
 *      Author: iklein
 */

#ifndef INCLUDE_UTILS_ROTARY_H_
#define INCLUDE_UTILS_ROTARY_H_

#include "../../include/SmingCore.h"
#include "MultiFunctionButton.h"

enum class RotaryAction {
	PREV = 0,
	NEXT = 1,
	NONE = 2
};

typedef Delegate<void()> RotaryButtonActionDelegate;
typedef Delegate<void(RotaryAction)> RotaryWheelActionDelegate;

class Rotary {

private:
	MultiFunctionButton* btn = NULL;
	int encoderCLK =12;
	int encoderDT =13;
	volatile int lastEncoded = 0;
	volatile long encoderValue = 0;
	long lastencoderValue = 0;
	int lastValue =-1000;
	ButtonActionDelegate delegatedActionEvent = NULL;
	RotaryWheelActionDelegate delegatedWheelEvent = NULL;
	Timer rotaryTimer;
	int rotaryChangeCountResolution=4;

	RotaryAction act = RotaryAction::NONE;
//	long lastReadTime =0;
public:
	Rotary(){
		rotaryTimer.initializeMs(100, TimerDelegate(&Rotary::updateInternalRotaryState, this)).start();
	};

	~Rotary(){
		delete (btn);
		rotaryTimer.stop();
	};

	Rotary(int btnPin, int encoderCLK, int encoderDT) {
		Rotary();
		this->init(encoderCLK, encoderDT);
		this->initBtn(btnPin);
	};

	void init(int encoderCLK, int encoderDT) {
		this->encoderCLK = encoderCLK;
		this->encoderDT = encoderDT;

		attachInterrupt(encoderCLK, RotaryButtonActionDelegate(&Rotary::updateEncoder, this), (GPIO_INT_TYPE)GPIO_PIN_INTR_ANYEDGE);
		attachInterrupt(encoderDT, RotaryButtonActionDelegate(&Rotary::updateEncoder, this), (GPIO_INT_TYPE)GPIO_PIN_INTR_ANYEDGE);
	};

	MultiFunctionButton* initBtn(int buttonPin, ButtonActionDelegate handler = null, bool pressAndHold = true) {
		if (!btn) {
			btn = new MultiFunctionButton();
		}

		pinMode(buttonPin, INPUT);
		digitalWrite(buttonPin, HIGH); //turn pullup resistor on
		btn->initBtn(buttonPin, handler, pressAndHold);
		return btn;
	};

	//Delegated call when event is triggered
	void setOnWheelEvent(RotaryWheelActionDelegate handler) {
		debugf("set setOnWheelEvent");
		delegatedWheelEvent  = handler;
	}

	MultiFunctionButton* getButton() {
		return btn;
	}

	void IRAM_ATTR updateEncoder() {
		int MSB = digitalRead(encoderCLK); //MSB = most significant bit
		int LSB = digitalRead(encoderDT); //LSB = least significant bit

		int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
		int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

		if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
		if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

		lastEncoded = encoded; //store this value for next time
	};

	void IRAM_ATTR updateInternalRotaryState() {
		int tmp = encoderValue;
		if (tmp > lastencoderValue && (tmp - lastencoderValue >4)) {
			act = RotaryAction::PREV;
			debugf("Prev");
			lastencoderValue = tmp;

		}
		else if (tmp < lastencoderValue && (lastencoderValue - tmp >4)){
			act = RotaryAction::NEXT;
			debugf("Next");
			lastencoderValue = tmp;
		}

		if (!delegatedWheelEvent) {
			debugf("no delegatedWheelEvent");
			return;
		}

		if (act != RotaryAction::NONE) {
			delegatedWheelEvent(act);
		}
		act = RotaryAction::NONE;
	}

	void btnClicked(MultiFunctionButtonAction event) {
		switch (event) {
			case BTN_CLICK:
				debugf("click");
//				moveRight();
				break;
			case BTN_DOUBLE_CLICK:
				debugf("BTN_DOUBLE_CLICK");
//				moveLeft();
				break;
			case BTN_LONG_CLICK:
				debugf("BTN_LONG_CLICK");
				break;
			case BTN_HOLD_CLICK:
				debugf("BTN_HOLD_CLICK");
				break;
		}
	}
};

#endif /* INCLUDE_UTILS_ROTARY_H_ */
