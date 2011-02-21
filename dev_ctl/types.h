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
} Input;

#define CONTENT_LINES 3
#define CONTENT_LENGTH 16
typedef struct{
    char title[14];
    char content[CONTENT_LINES][CONTENT_LENGTH];
} Screen;

typedef struct{
    boolean active;

    uint8_t initialSpeed;
    uint8_t maxSpeed;
    uint8_t stepUp;
    uint8_t brakeRatio;
    uint8_t stepTime;
    float   rearSpeedRatio;
    float   turnSpeedRatio;
} DriveProfile;

typedef struct{
    char string[CONTENT_LENGTH];
    boolean selected;
} lcdLine;

typedef struct{
    uint8_t currentPwm;
    uint8_t stepUp;
    uint8_t brakeRatio;
    uint8_t stepTime;
    uint8_t pin;
    unsigned long counter;
} Motor;

#endif
