/*
 * motors.h
 * Copyright (C) 2010,2011 LSITEC

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MOTORS_H
#define MOTORS_H

#include <string.h>
#include "types.h"
#include "pins.h"

void controlMotors(int n, int pwmGoal);
boolean motorStopped(int n);
int motorPwm(int n);
void changeMotorParameters(int n, int stepUp, int brakeRatio, int stepTime);

#define MINIMAL_PWM 5
#define FAST_BRAKING_LIMIT 20
#define FAST_BRAKING_RATIO 1
#define SLOW_BRAKING_LIMIT 8
#define SLOW_BRAKING_RATIO 3

#endif
