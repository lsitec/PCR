/*
 * types.h
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
#ifndef TYPES_H
#define TYPES_H

typedef struct{
    int deltaX;
    int deltaY;
    boolean menu;

    boolean p2;

    boolean dpad_left;
    boolean dpad_right;
    boolean dpad_up;
    boolean dpad_down;
    boolean dpad_ok;
} Input;

#define CONTENT_LINES 3
#define CONTENT_LENGTH 16

typedef struct{
    char title[14];
    char content[CONTENT_LINES][CONTENT_LENGTH];
} Screen;

typedef struct{
    int maxSpeed;
    char name[CONTENT_LENGTH];
    boolean active;
	int stepUp;
	int stepDown;
	int stepTime;
    int initialSpeed;
} DriveProfile;

typedef struct Mode{
    byte id;
    char name[CONTENT_LENGTH];
    boolean active;
} Mode;

typedef struct{
    char string[CONTENT_LENGTH];
    boolean selected;
} lcdLine;

typedef struct{
    int currentPwm;
    int stepUp;
	int stepDown;
	int stepTime;
    int pin;
    unsigned int counter;
} Motor;

typedef struct{
    byte op_mode;
    byte in_mode;
    byte x_h;
    byte x_l;
    byte y_h;
    byte y_l;
    boolean p_button;
} i2c_pack;

#endif
