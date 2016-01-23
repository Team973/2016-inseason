/*
 * MockSpeedController.h
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKSPEEDCONTROLLER_H_
#define MOCKWPILIB_MOCKSPEEDCONTROLLER_H_

#include <stdint.h>

class SpeedController {
public:
	SpeedController();
	virtual ~SpeedController();

	virtual void Set(float speed, uint8_t syncGroup = 0);
	virtual float Get();
	virtual void SetInverted(bool inverted);
	virtual void Disable();
	virtual bool GetInverted();

private:
	double m_power;
	bool m_inverted;
	bool m_disabled;
};

#endif /* MOCKWPILIB_MOCKSPEEDCONTROLLER_H_ */
