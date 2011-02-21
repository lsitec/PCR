/*
 * drive.h
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
#ifndef DRIVE_H
#define DRIVE_H

#include <WProgram.h>
#include "types.h"
#include "motors.h"
#include "pins.h"
#include "globals.h"
#include <EEPROM.h>

#define Back 11
#define Front 12
#define Left 13
#define Right 14
#define Dead 15
#define Emergency 16

#define EMERGENCY_STOP 4000
#define BRAKE_DELAY 1
#define STOP_DELAY 1000
#define NPROFILES 5
#define MAX_REAR_SPEED 100
#define MAX_TURN_SPEED 70
#define SPEED_RATIO_MAX 1.0
#define SPEED_RATIO_MIN 0.5
#define HYSTERESIS 20
#define PWM_SAMPLES 32
#define GLOBAL_SPEED_LIMIT_MAX 255
#define GLOBAL_SPEED_LIMIT_MIN 64

#define CUSTOM_ZONES 10
#define MAX_CUSTOM_MAXSPEED 213
#define MIN_CUSTOM_MAXSPEED 60
#define MAX_CUSTOM_STEPUP 20
#define MIN_CUSTOM_STEPUP 2

#define PWM_TURN_RATIO 5

boolean isStopped();
void changeActiveProfile(int newprofile);
int getActiveProfile();
char* getProfileName(int n);
int drive(const int x_input, const int y_input, boolean p_button);
int calculateNegPWM(int val);
int calculateNegPWM(int val, int limit);
int calculatePosPWM(int val);
int calculatePosPWM(int val, int limit);
void initDriveMode();
DriveProfile* getActiveProfilePointer();
int calculateDiagonalDiff(const int& forward, const int& lateral);
int getRearSpeedLimit();
int getTurnSpeedLimit();
void changeSpeedRatio(float ratio);
int do_hysteresis(int val, const int last_val);
void do_pwm_average(int pwm1, int pwm2);
void increaseCustomMaxspeed();
void decreaseCustomMaxspeed();
void increaseCustomStepUp();
void decreaseCustomStepUp();
void changeCustomMaxSpeed();
void changeCustomStepUp();
void recoverCustomSettings();
void increaseGlobalSpeedLimit();
void decreaseGlobalSpeedLimit();
void changeGlobalSpeedLimit();

enum EepromAddress {
    CUSTOM_MAXSPEED,
    CUSTOM_STEPUP,
    CUSTOM_REAR_SPEED,
    CUSTOM_TURN_SPEED,
    LAST_ACTIVE_PROFILE,
    SPEED_RATIO,
    GLOBAL_SPEED_LIMIT,
};

#endif
