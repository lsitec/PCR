/*
 * mouse.cpp
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

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

#include <WProgram.h>
#include "mouse.h"

byte HID_TABLE[7] = {0x00,0x8B,0x8D,0x8E,0x8C,0x8F,0x8A};

Mouse::Mouse()
{
    this->_action = IDLE;
    this->_mode = BT_DATA;
}

Mouse::Mouse(byte mode)
{
    this->_action = IDLE;
    this->_mode = mode;
}

void Mouse::setAction(byte action)
{
    this->_action = action;
}

byte Mouse::getAction() const
{
    return this->_action;
}

void Mouse::setMode(byte mode)
{
    this->_mode = mode;
}

byte Mouse::getMode() const
{
    return this->_mode;
}

boolean Mouse::isActive() const
{
    return (this->_mode == BT_DATA || this->_mode == USB);
}

void Mouse::updateAction(int delta_x, int delta_y, boolean p_button)
{
    if (delta_y < MNEG) this->_action = DOWN;
    else if (delta_y > MPOS) this->_action = UP;
    else if (delta_x < MNEG) this->_action = LEFT;
    else if (delta_x > MPOS) this->_action = RIGHT;
    else if (p_button) this->_action = LCLICK;
    else this->_action = IDLE;
}

void Mouse::executeAction() const
{
    if (this->isActive())
    {
        if (this->_action == LCLICK)
            this->click(this->_action);
        else
            this->move(this->_action);
    }
}

MouseBT::MouseBT(byte mode) : Mouse(mode) {}

void MouseBT::init() const
{
    digitalWrite(bt_dtrPin, LOW);

    digitalWrite(bt_rstPin, HIGH);
    delay(100);
    digitalWrite(bt_rstPin, LOW);
}

void MouseBT::move(byte action) const
{
    if (this->_action != IDLE)
    {
        Serial.print(HID_TABLE[action], BYTE);
    }
    delay(30);
}

void MouseBT::move(byte action, byte ammount) const
{
    for(byte i=0;i<ammount;i++) {
        this->move(action);
    }
}

void MouseBT::click(byte button) const
{
    Serial.print(HID_TABLE[button], BYTE);
    delay(20);
    Serial.print(HID_TABLE[5], BYTE);
    delay(200);
}

void MouseBT::changeToDataMode()
{
    digitalWrite(bt_dtrPin, HIGH);
    delay(25);
    digitalWrite(bt_dtrPin, LOW);
    this->_mode = BT_DATA;
}

void MouseBT::changeToComMode()
{
    digitalWrite(bt_dtrPin, LOW);
    delay(25);
    digitalWrite(bt_dtrPin, HIGH);
    this->_mode = BT_COM;
}

void MouseBT::preAction()
{
    if (Serial.available() > 0)
        this->readSerialData();
}

void MouseBT::postAction()
{
}

#define BUFSIZE 20
char ser_in[BUFSIZE];
int ser_index = 0;

void clear_ser_in()
{
    for(int i=0;i<BUFSIZE;i++)
        ser_in[i] = ' ';
    ser_index = 0;
}

void MouseBT::readSerialData()
{
    while (Serial.available())
    { 
        char c = Serial.read();
        if (c != '\n' && c != '\r')
        {
            ser_in[ser_index] = c;
            ser_index++;
            if (ser_index >= BUFSIZE)
                ser_index = 0;
            continue;
        }
        ser_in[ser_index] = 0;
    }
    this->processSerialData(ser_in);
}

void MouseBT::processSerialData(char * data)
{
    if (strstr(data, "RING") != 0)
    {
        this->_mode = BT_DATA;
        clear_ser_in();
    }
    else if (strstr(data, "NO CARRIER") != 0)
    {
        this->_mode = BT_NP;
        clear_ser_in();
    }
}

MouseUSB::MouseUSB(byte mode) : Mouse(mode) {}

void MouseUSB::init() const
{
    initUsbMouse();
}

void MouseUSB::preAction()
{
}

void MouseUSB::postAction()
{
    sendHidReport();
}

void MouseUSB::move(byte action) const
{
    updateHidReport(action, 0);
}

void MouseUSB::move(byte action, byte ammount) const
{
    for(byte i=0;i<ammount;i++) {
        this->move(action);
    }
}

void MouseUSB::click(byte button) const
{
    updateHidReport(IDLE, 1);
}
