/*
 * JoystickHelper.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Andrew
 */

#ifndef LIB_JOYSTICKHELPER_H_
#define LIB_JOYSTICKHELPER_H_

/*
 * Button mapping for the DualAction joystick
 */
namespace DualAction {
	/*
	 * Standard buttons... shouldn't need any explanation
	 */
	const unsigned int BtnX = 1;
	const unsigned int BtnA = 2;
	const unsigned int BtnB = 3;
	const unsigned int BtnY = 4;
	const unsigned int LeftBumper = 5;
	const unsigned int RightBumper = 6;
	const unsigned int LeftTrigger = 7;
	const unsigned int RightTrigger = 8;
	const unsigned int Back = 9;
	const unsigned int Start = 10;

	/*
	 * When you push down on the left and right joystick, that registers
	 * as a button press
	 */
	const unsigned int LJoystickBtn = 11;
	const unsigned int RJoystickBtn = 12;

	/*
	 * Not buttons but the numbers for each axis... can be used with
	 * joystick.GetRawAxis
	 * DPad axis only return 0.0, -1.0, and 1.0
	 */
	const unsigned int LeftXAxis = 0;
	const unsigned int LeftYAxis = 1;
	const unsigned int RightXAxis = 2;
	const unsigned int RightYAxis = 3;
	const unsigned int DPadXAxis = 4;
	const unsigned int DPadYAxis = 5;
}

#endif /* LIB_JOYSTICKHELPER_H_ */
