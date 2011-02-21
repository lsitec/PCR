/*
 * pins.h
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

#ifndef PINS_H
#define PINS_H
#include <stdint.h>

const static byte speedPin = 0;
const static byte dirPin = 1;
const static byte refPin = 2;

const static byte p_frontPin = 14;
const static byte p_backPin = 15;
const static byte p_leftPin = 16;
const static byte p_rightPin = 17;

const static byte p_buttonPin = 7;
const static byte detectPin = 8;

const static byte dpad_okPin = 9;
const static byte dpad_upPin = 12;
const static byte dpad_downPin = 11;
const static byte dpad_leftPin = 10;
const static byte dpad_rightPin = 13;
const static byte p2Pin = 6;

const static byte lcdRSPin = 4;
const static byte lcdEnablePin = 5;
const static byte lcdDataPin4 = 0;
const static byte lcdDataPin5 = 1;
const static byte lcdDataPin6 = 2;
const static byte lcdDataPin7 = 3;

const static byte sdaPin = 4;
const static byte sclPin = 5;

void configPins();

#endif
