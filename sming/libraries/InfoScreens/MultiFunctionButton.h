#ifndef INCLUDE_ButtonActions_
#define INCLUDE_ButtonActions_

/*
 * ButtonCLick.cpp
 *
 *  Created on: Jun 1, 2015
 *      Author: ilan
 */

#include "../../include/SmingCore.h"

//This class makes use of:
/* 4-Way Button:  Click, Double-Click, Press+Hold, and Press+Long-Hold Test Sketch

 By Jeff Saltzman
 Oct. 13, 2009

 1) Click:  rapid press and release
 2) Double-Click:  two clicks in quick succession
 3) Press and Hold:  holding the button down
 4) Long Press and Hold:  holding the button for a long time
 */

/*
 * alon24
 *
 *  added options to Hold and get click repeats (100ms)
 */
enum MultiFunctionButtonAction { BTN_CLICK=1, BTN_DOUBLE_CLICK=2, BTN_LONG_CLICK=3, BTN_HOLD_CLICK=4, BTN_TRIPPLE_CLICK=5};

typedef Delegate<void(MultiFunctionButtonAction)> ButtonActionDelegate;

class MultiFunctionButton
{

//=================================================
//  MULTI-CLICK:  One Button, Multiple Events

// Button timing variables
	int debounce = 20; // ms debounce period to prevent flickering when pressing or releasing the button
	int DCgap = 250;       // max ms between clicks for a double click event
	int holdTime = 400; // ms hold period: how long to wait for press+hold event
	int longHoldTime = 3000; // ms long hold period: how long to wait for press+hold event
	long pressAndHoldRepeatGap = 100;

public:

	MultiFunctionButton() {};

	MultiFunctionButton(int buttonPin, ButtonActionDelegate handler = null, bool pressAndHold = true)
	{
		m_buttonPin = buttonPin;
		this->delegatedActionEvent = handler;
		if (handler) {
			buttonTimer.initializeMs(80, TimerDelegate(&MultiFunctionButton::actOnButton, this)).start();
		}

		enableClickAndHold(pressAndHold);
	};

	// init to set button, handler is optional here
	void initBtn(int buttonPin, ButtonActionDelegate handler = null, bool pressAndHold = true) {
		m_buttonPin = buttonPin;
		if (handler) {
			setOnButtonEvent(handler);
		}

		enableClickAndHold(pressAndHold);
	}

	void enableClickAndHold(bool pressAndHold) {
		this->pressAndHold = pressAndHold;
	}

	//Delegated call when event is triggered
	void setOnButtonEvent(ButtonActionDelegate handler)
	{
		delegatedActionEvent  = handler;
		inResetMode = true;
		if (!buttonTimer.isStarted()) {
			buttonTimer.initializeMs(80, TimerDelegate(&MultiFunctionButton::actOnButton, this)).start();
		}
	}

	int checkButton()
	{
		int event = 0;
		buttonVal = digitalRead(m_buttonPin);

		if (buttonVal == HIGH && inResetMode) {
			inResetMode = false;
		}

		if (inResetMode) {
			return 0;
		}
		// Button pressed down
		if (buttonVal == LOW && buttonLast == HIGH
				&& (millis() - upTime) > debounce)
		{
			downTime = millis();
			ignoreUp = false;
			waitForUp = false;
			singleOK = true;
			holdEventPast = false;
			longHoldEventPast = false;
			if ((millis() - upTime)
					< DCgap&& DConUp == false && DCwaiting == true)
				DConUp = true;
			else
				DConUp = false;
			DCwaiting = false;
		}
		// Button released
		else if (buttonVal == HIGH && buttonLast == LOW
				&& (millis() - downTime) > debounce)
		{
			if (not ignoreUp)
			{
				upTime = millis();
				if (DConUp == false)
					DCwaiting = true;
				else
				{
					event = 2;
					DConUp = false;
					DCwaiting = false;
					singleOK = false;
				}
			}
		}
		// Test for normal click event: DCgap expired
		if (buttonVal == HIGH && (millis() - upTime) >= DCgap
				&& DCwaiting == true && DConUp == false && singleOK == true
				&& event != 2)
		{
			event = 1;
			DCwaiting = false;
		}

		if (pressAndHold) {
			// Test for hold
			long current = millis();
			if (buttonVal == LOW && ((current - downTime) >= holdTime) && (current - pressAndHoldRepeatGap)>= lastPressAndHoldTime)
			{
				lastPressAndHoldTime = current;
				if (delegatedActionEvent) {
					delegatedActionEvent(MultiFunctionButtonAction::BTN_CLICK);
				}
			}
		}
		else {
			// Test for hold
			if (buttonVal == LOW && (millis() - downTime) >= holdTime)
			{
				// Trigger "normal" hold
				if (not holdEventPast)
				{
					event = 3;
					waitForUp = true;
					ignoreUp = true;
					DConUp = false;
					DCwaiting = false;
					//downTime = millis();
					holdEventPast = true;
				}
				// Trigger "long" hold
				if ((millis() - downTime) >= longHoldTime)
				{
					if (not longHoldEventPast)
					{
						event = 4;
						longHoldEventPast = true;
					}
				}
			}
		}

		buttonLast = buttonVal;
		return event;
	}

	void actOnButton()
	{
		// Get button event and act accordingly
		int b = checkButton();
		MultiFunctionButtonAction act;
		switch (b) {
			case 1:
				act = BTN_CLICK;
				break;
			case 2:
				act = BTN_DOUBLE_CLICK;
				break;
			case 3:
				act = BTN_LONG_CLICK;
				break;
			case 4:
				act = BTN_HOLD_CLICK;
				break;
			case 5:
				act = BTN_TRIPPLE_CLICK;
				break;
		}

		if ((b==1 || b==2 || b==3 || b==4 || b==5) && delegatedActionEvent) {
			delegatedActionEvent(act);
		}
	}
private:
	ButtonActionDelegate delegatedActionEvent;
	Timer buttonTimer;
	TimerDelegate holdTimerDelegate;
	long downTime = -1;         // time the button was pressed down
	long upTime = -1;           // time the button was released
	long lastPressAndHoldTime = -1; // time the pressAndHold was activated last
	int m_buttonPin =-1;

	//After changing handlers, need to be able to init until first down, becuase if switch was made becuase of long click, then
	// button is still down
	bool inResetMode = false;

	// Button variables
	boolean buttonVal = HIGH;   // value read from button
	boolean buttonLast = HIGH; // buffered value of the button's previous state
	boolean DCwaiting = false; // whether we're waiting for a double click (down)
	boolean TCwaiting = false; // whether we're waiting for a tripple click (down)
	boolean DConUp = false; // whether to register a double click on next release, or whether to wait and click
	boolean TConUp = false; // whether to register a tripple click on next release, or whether to wait and click
	boolean singleOK = true;    // whether it's OK to do a single click
	boolean doubleOK = true;    // whether it's OK to do a double click
	boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered
	boolean waitForUp = false; // when held, whether to wait for the up event
	boolean holdEventPast = false; // whether or not the hold event happened already
	boolean longHoldEventPast = false; // whether or not the long hold event happened already

	bool pressAndHold = true; //This will fire click events on button hold (so will not fire BTN_LONG_CLICK and BTN_HOLD_CLICK)
};

#endif /* INCLUDE_ButtonActions_ */
