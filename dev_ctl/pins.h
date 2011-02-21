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

const static uint8_t sdaPin = 4;
const static uint8_t sclPin = 5;

const static uint8_t bt_txdPin = 0;
const static uint8_t bt_rxdPin = 1;
const static uint8_t bt_dtrPin = 4;
const static uint8_t bt_rstPin = 5;

const static uint8_t ir_txdPin = 7;
const static uint8_t ir_rxdPin = 6;

const static uint8_t usb_dm = 2;
const static uint8_t usb_dp = 9;

void configPins();

#endif
