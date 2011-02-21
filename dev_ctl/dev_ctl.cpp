/*
 * dev_ctl.cpp
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
#include "dev_ctl.h"

#define USB_MOUSE
#ifdef BT_MOUSE
    MouseBT mouse(BT_DATA);
#endif

#ifdef USB_MOUSE
    MouseUSB mouse(USB);
#endif

byte op_mode = MOUSE_MODE;
boolean new_msg = false;
byte drive_profile = 0;
int count = 0;

int i2c_delta_x = 512;
int i2c_delta_y = 512;
boolean p_button = false;

byte device = 0;
byte button = 0;
byte write = 0;

void setup()
{
    configPins();
    mouse.init();
    irInit();

    Wire.begin(DEV_CTL_ADDR);
    Wire.onReceive(receiveEvent);

    Serial.begin(9600);
}

byte i2c_response = 1;

void loop() {
    if (op_mode == MOUSE_MODE)
    {
        if (new_msg)
        {
            mouse.preAction();
            mouse.updateAction(i2c_delta_x, i2c_delta_y, p_button);
            mouse.executeAction();

            i2c_response = mouse.getMode();
        }
        mouse.postAction();
    }
    else if (op_mode == IR_MODE)
    {
        if (new_msg)
        {
            if(write)
                IRrx(device, button);
            else
                IRtx(device, button);

            i2c_response = device+1;
        }
    }
    else
        i2c_response = 3;
    count++;

    if (new_msg || count > 1000)
    {
        Wire.beginTransmission(MST_CTL_ADDR);
        Wire.send(i2c_response);
        Wire.endTransmission();

        count = 0;
        new_msg = false;
    }
}

void receiveEvent(int numBytes)
{
    byte data_in[7];

    for (int i=0;i<numBytes;i++) { data_in[i] = Wire.receive(); }

    op_mode = data_in[0];

    if (op_mode == MOUSE_MODE)
    {
        i2c_delta_x = word( data_in[2], data_in[3] );
        i2c_delta_y = word( data_in[4], data_in[5] );
        p_button = data_in[6];
    }
    else if (op_mode == IR_MODE)
    {
        device = data_in[2];
        button = data_in[3];
        write = data_in[4];
    }

    new_msg = true;
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
