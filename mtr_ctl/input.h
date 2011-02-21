/*
 * input.h
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
#ifndef  INPUT_H
#define  INPUT_H

#include "globals.h"
#include "pins.h"
#include "types.h"

#define REF_MIN 109
#define REF_MAX 814

#define PENTA_MIN 0
#define PENTA_MAX 1023
#define PENTA_MED 512

#define SAMPLE_PERIOD 10
#define INPUT_SAMPLES 16

extern Input input;

void inputAbstraction(Input *input);
void detectInputMode();
boolean bool_read_pin(int pin);

#endif
