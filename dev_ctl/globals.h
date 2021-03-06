/*  
 *  globals.h
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

#ifndef GLOBALS_H
#define GLOBALS_H

const static byte DRIVE_MODE = 1;
const static byte PROFILES_MODE = 2;
const static byte MOUSE_MODE = 3;
const static byte IR_MODE = 4;
const static byte CONFIG_MODE = 5;
const static byte MENU_MODE = 6;

const static byte PROF_OUT_FAST = 0;
const static byte PROF_OUT_SLOW = 1;
const static byte PROF_IN_FAST = 2;
const static byte PROF_IN_SLOW = 3;

#define ANALOG 0
#define DIGITAL 1

#define MST_CTL_ADDR 0x10
#define DEV_CTL_ADDR 0x20
#define EEPROM_ADDR  0x50

#define MNEG 440
#define MPOS 580
#define NEG 1
#define POS 1023
#define RES_PWM 255

#define PENTA_MED 512

#endif
