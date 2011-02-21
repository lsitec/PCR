/*
 * pins.cpp
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

#include <WProgram.h>
#include "pins.h"

void configPins(){

    pinMode(emergencyPin, OUTPUT);
    pinMode(inv1Pin, OUTPUT);
    pinMode(en1Pin, OUTPUT);
    pinMode(pwm1Pin, OUTPUT);
    pinMode(inv2Pin, OUTPUT);
    pinMode(en2Pin, OUTPUT);
    pinMode(pwm2Pin, OUTPUT);
    pinMode(brakePin, OUTPUT);
}
