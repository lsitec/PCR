/*
 * display.h
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

#ifndef  DISPLAY_H
#define  DISPLAY_H

#include "pins.h"
#include "string.h"
#include "types.h"
#include "menu.h"

#define ROWS      4
#define COLUMNS   16

extern Screen screen;

void initDisplay();
void showLine(const char *, int, int);
void fullDisplay(Screen& screen);
void set_title(Screen& screen, const char* title);
void clear_content(Screen& screen);
void busy_screen();

#endif
