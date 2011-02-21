/*
 * ir.h
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

#ifndef IR_H
#define IR_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <WProgram.h>
#include <Wire.h>
#include "globals.h"
#include "pins.h"

#define TIMER_RESET  TCNT1 = 0
#define SAMPLE_SIZE  74
#define TIME_OUT     65500

void irInit();
void IRrx(byte device, byte button);
void IRtx(byte device, byte button);

#endif
