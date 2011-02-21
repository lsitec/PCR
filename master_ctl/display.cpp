/*
 * display.cpp
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

#include <WProgram.h>
#include <LiquidCrystal.h>
#include "display.h"

LiquidCrystal lcd(lcdRSPin, lcdEnablePin, lcdDataPin4, lcdDataPin5, lcdDataPin6, lcdDataPin7);

Screen lastScreen = {   " ",
                        " ",
                        " ",
                        " "};

void initDisplay()
{
    lcd.begin(COLUMNS, ROWS);
    lcd.noCursor();

    showLine("Modulo de",0,0);
    showLine("controle",0,1);
    showLine("LSITEC - 2010",0,3);

    delay(500);
    lcd.clear();
}

void showLine(const char *mensagem, int coluna=0, int linha=0)
{
    lcd.setCursor(coluna, linha);
    lcd.print(mensagem);
}

void clearLine(int line)
{
    lcd.setCursor(0,line);
    for(int i=0;i<=COLUMNS;i++)
    {
        lcd.print(" ");
        lcd.setCursor(i,line);
    }
}

void fullDisplay(Screen& screen)
{
    if (strcmp(screen.title, lastScreen.title) != 0)
    {
        clearLine(0);
        lcd.setCursor(2,0);
        lcd.print(screen.title);
    }

    lcd.setCursor(0,0);
    lcd.print("*");
    lcd.setCursor(COLUMNS-1,0);
    lcd.print("*");

    for (int i=0; i<CONTENT_LINES ; i++ )
    {
        if (strcmp(screen.content[i], lastScreen.content[i]) != 0){
            clearLine(i+1);
            lcd.setCursor(0,i+1);
            lcd.print(screen.content[i]);
        }
    }

    strcpy(lastScreen.title, screen.title);
    for (int j=0; j<CONTENT_LINES; j++)
    {
        strcpy(lastScreen.content[j], screen.content[j]);
    }
}

void set_title(Screen& screen, char const* title)
{
    strcpy(screen.title, title);
}

void clear_content(Screen& screen)
{
    strcpy_P(screen.content[0], strNull);
    strcpy_P(screen.content[1], strNull);
    strcpy_P(screen.content[2], strNull);
}
