/*
 * mtr_ctl.cpp
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
#include "mtr_ctl.h"

byte op_mode = DRIVE_MODE;
boolean new_msg = false;
byte drive_profile = 0;

int i2c_delta_x = 512;
int i2c_delta_y = 512;
int p_button = false;

void setup() {
    configPins();
    initDriveMode();
    detectInputMode();

    Wire.begin(MTR_CTL_ADDR);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(sendCustomProfile);
}

byte i2c_response = 10;

unsigned long comm_timeout = 0;

void loop() {
    if ( millis() - comm_timeout > 1000 ) {
        inputAbstraction(&input);
        drive(input.deltaX, input.deltaY, input.menu);
    }
    else if (new_msg) {
        comm_timeout = millis();
        switch (op_mode) {
            case DRIVE_MODE:
                i2c_response = drive(i2c_delta_x,i2c_delta_y,p_button);

                Wire.beginTransmission(MST_CTL_ADDR);
                Wire.send(i2c_response);
                Wire.endTransmission();
                break;

            case PROFILES_MODE:
                EEPROM.write(CUSTOM_MAXSPEED, custom_speed_index);
                EEPROM.write(CUSTOM_STEPUP, custom_stepup_index);
                i2c_response = 11;
                break;

            case CONFIG_MODE:
                break;
        }
        new_msg = false;
    }
    else {
        if (op_mode == DRIVE_MODE) {
            drive(i2c_delta_x,i2c_delta_y,p_button);
        }
    }
}

void sendCustomProfile(void)
{
    byte b[2] = { custom_speed_index, custom_stepup_index };
    Wire.send(b,2);
}

void receiveEvent(int numBytes)
{
    byte data_in[7];

    for (int i=0;i<numBytes;i++) { data_in[i] = Wire.receive(); }

    op_mode = data_in[0];

    if (op_mode == DRIVE_MODE)
    {
        drive_profile = data_in[1];
        i2c_delta_x = word( data_in[2], data_in[3] );
        i2c_delta_y = word( data_in[4], data_in[5] );
        p_button = data_in[6];
    }
    else if (op_mode == PROFILES_MODE)
    {
        custom_speed_index = data_in[2];
        custom_stepup_index = data_in[3];
    }

    new_msg = true;
}

int main() {
    init();
    setup();
    for(;;){
        loop();
    }
    return 0;
}
