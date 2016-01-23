/*
 * MockSolenoid.h
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKSOLENOID_H_
#define MOCKWPILIB_MOCKSOLENOID_H_

class Solenoid {
public:
	Solenoid(int chan);
	virtual ~Solenoid();

	void Set(bool pos);
	bool Get();
private:
	bool m_pos;
};

#endif /* MOCKWPILIB_MOCKSOLENOID_H_ */
