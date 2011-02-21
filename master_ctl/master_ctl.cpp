/*
 * master_ctl.cpp
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

#include "master_ctl.h"

void setup() {
    configPins();
    initDisplay();
    detectInputMode();
    menuInit();
    i2cInit();
}

unsigned long last_millis = 0;

void updateUsageTime()
{
    unsigned long cur_time = millis();
    if(cur_time - last_millis > 59999)
    {
        last_millis = cur_time;
        minutes_on++;
            int usage_time = word(EEPROM.read(TIME_CTR_ADDR), EEPROM.read(TIME_CTR_ADDR+1));
            usage_time++;
            EEPROM.write(TIME_CTR_ADDR, highByte(usage_time));
            EEPROM.write(TIME_CTR_ADDR+1, lowByte(usage_time));
    }
}

void loop() {
    inputAbstraction(&input);

    switch (mode) {
        case DRIVE_MODE:
            strcpy_P(screen.title, strDrive);
            strcpy_P(screen.content[0], (char*)pgm_read_word(&(profiles_table[drive_profile])));
            strcpy_P(screen.content[1], strNull);
            strcpy_P(screen.content[2], (char*)pgm_read_word(&(drive_states[mtr_ctl_ready])));

            sendPack2Motors(DRIVE_MODE, drive_profile, highByte(input.deltaX), lowByte(input.deltaX),
                            highByte(input.deltaY), lowByte(input.deltaY), input.p2);
            break;

        case MOUSE_MODE:
            strcpy_P(screen.title, strMouse);
            strcpy_P(screen.content[0], strNull);
            if (dev_ctl_ready)
                strcpy_P(screen.content[1], (char*)pgm_read_word(&(mouse_states[dev_ctl_ready])));
            strcpy_P(screen.content[2], strNull);

            sendPack2Dev(MOUSE_MODE, DIGITAL, highByte(input.deltaX), lowByte(input.deltaX),
                         highByte(input.deltaY), lowByte(input.deltaY), input.p2);
            break;

        case MENU_MODE:
            menu.update(input.deltaX, input.deltaY);
            delay(150);
            break;

        case CONFIG_MODE:
            enterPassword(input.deltaX, input.deltaY);
            delay(300);
            break;

        case PROFILES_MODE:
            updateCustomProfile(input.deltaX, input.deltaY);
            delay(300);
            break;
    }
    if (mode != DRIVE_MODE)
    {
        sendPack2Motors(DRIVE_MODE, drive_profile, highByte(PENTA_MED), lowByte(PENTA_MED),
                        highByte(PENTA_MED), lowByte(PENTA_MED), false);
    }

    menu.turnOn_Off(input.dpad_ok, input.menu);
    if (menu.isOn())
        mode = MENU_MODE;

    fullDisplay(screen);
    updateUsageTime();
}

int main()
{
    init();
    setup();
    for(;;)
    {
        loop();
    }
    return 0;
}
