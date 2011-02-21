/*
 * i2c.cpp
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

#include "i2c.h"

extern byte dev_ctl_ready;
extern byte mtr_ctl_ready;

void receiveEvent(int numBytes)
{
    byte received = 0;
    for (int i=0; i<numBytes; i++) { received = Wire.receive(); }

    if (received>10)
        mtr_ctl_ready = received-10;
    else
        dev_ctl_ready = received;
}

void i2cInit()
{
    Wire.begin(MST_CTL_ADDR);
    Wire.onReceive(receiveEvent);
}

void sendPack2Dev(byte op_mode, byte in_mode, byte x_h, byte x_l,
                  byte y_h, byte y_l, byte p_button)
{
    i2c_pack data_out;
    if(dev_ctl_ready)
    {
        data_out.op_mode = op_mode;
        data_out.in_mode = in_mode;
        data_out.x_h = x_h;
        data_out.x_l = x_l;
        data_out.y_h = y_h;
        data_out.y_l = y_l;
        data_out.p_button = p_button;

        Wire.beginTransmission(DEV_CTL_ADDR);
        Wire.send((byte *)&data_out, sizeof(data_out));
        Wire.endTransmission();
        dev_ctl_ready = 0;
    }
}

void sendPack2Motors(byte op_mode, byte in_mode, byte x_h, byte x_l,
                     byte y_h, byte y_l, byte p_button)
{
    i2c_pack data_out;
    if(mtr_ctl_ready)
    {
        data_out.op_mode = op_mode;
        data_out.in_mode = in_mode;
        data_out.x_h = x_h;
        data_out.x_l = x_l;
        data_out.y_h = y_h;
        data_out.y_l = y_l;
        data_out.p_button = p_button;

        Wire.beginTransmission(MTR_CTL_ADDR);
        Wire.send((byte *)&data_out, sizeof(data_out));
        Wire.endTransmission();
        mtr_ctl_ready = 0;
    }
}
