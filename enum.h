/*
 * enum.h
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */

#ifndef ENUM_H_
#define ENUM_H_

typedef enum DIRECTION {
	UP 			= 0x01,
	DOWN		= 0x02,
	LEFT		= 0x04,
	RIGHT		= 0x08,
	CENTER		= 0x10
} dir_t;

#endif /* ENUM_H_ */
