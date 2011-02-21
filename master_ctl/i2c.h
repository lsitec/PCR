/*
 * i2c.h
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
#ifndef  I2C_H
#define  I2C_H

#include <WProgram.h>
#include <Wire.h>
#include "globals.h"
#include "pins.h"
#include "types.h"

void i2cInit();
void sendPack2Motors(byte op_mode, byte in_mode, byte x_h=0, byte x_l=0,
                     byte y_h=0, byte y_l=0, byte p_button=0);

void sendPack2Dev(byte op_mode, byte in_mode, byte x_h=0, byte x_l=0,
                  byte y_h=0, byte y_l=0, byte p_button=0);

#endif
