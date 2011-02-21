/*  
 * master_ctl.h
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

#ifndef MAIN_H
#define MAIN_H

#include <WProgram.h>
#include <EEPROM.h>
#include "types.h"
#include "pins.h"
#include "display.h"
#include "globals.h"
#include "input.h"
#include "menu.h"

Input input;
Screen screen;
byte mode = DRIVE_MODE;
byte drive_profile = 0;
boolean dev_ctl_ready = 3;
boolean mtr_ctl_ready = 1;
int minutes_on = 0;

void receiveEvent(int numBytes);

#endif
