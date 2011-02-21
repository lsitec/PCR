/*
 * mouse.h
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

#ifndef  MOUSE_H
#define  MOUSE_H

#include "globals.h"
#include "pins.h"
#include "types.h"
#include "hid_usb.h"

#define NC 1 // not connected
#define BT_NP 2 //not paired
#define BT_DATA 3
#define BT_COM 4
#define USB 5

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define LCLICK 5
#define IDLE 6
#define C_MODE 7

class Mouse
{
    protected:
        byte _action;

    public:
        byte _mode;

        Mouse();
        Mouse(byte mode);

        void setAction(byte action);
        byte getAction() const;
        void setMode(byte mode);
        byte getMode() const;
        boolean isActive() const;
        void updateAction(int delta_x, int delta_y, boolean p_button);
        void executeAction() const;

        virtual void init() const = 0;
        virtual void preAction() = 0;
        virtual void postAction() = 0;
        virtual void move(byte action) const = 0;
        virtual void move(byte action, byte ammount) const = 0;
        virtual void click(byte button) const = 0;
};

class MouseBT: public Mouse
{
    public:
        MouseBT();
        MouseBT(byte mode);
        void init() const;
        void preAction();
        void postAction();
    private:
        void move(byte action) const;
        void move(byte action, byte ammount) const;
        void click(byte button) const;

        void changeToDataMode();
        void changeToComMode();
        void processSerialData(char * data);
        void readSerialData();
};

class MouseUSB: public Mouse
{
    public:
        MouseUSB();
        MouseUSB(byte mode);
        void init() const;
        void preAction();
        void postAction();
    private:
        void move(byte action) const;
        void move(byte action, byte ammount) const;
        void click(byte button) const;
};

#endif
