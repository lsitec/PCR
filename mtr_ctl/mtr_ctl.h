/*  
 * mtr_ctl.h
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

#include <Wire.h>
#include "types.h"
#include "drive.h"
#include "globals.h"
#include "pins.h"
#include "input.h"

extern uint8_t custom_speed_index;
extern uint8_t custom_stepup_index;

void receiveEvent(int howMany);
void sendCustomProfile(void);

Input input;

#endif
