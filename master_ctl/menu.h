/*
|| @file menu.h
|| Based on:
|| @file MenuBackend.h
|| @version 1.1-lite
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
|| @note some minor changes Adrian Brzezinski adrb@wp.pl
||
|| @description
|| | Provide an easy way of making menus
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef MENU_h
#define MENU_h

#include <WProgram.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Password.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "globals.h"
#include "display.h"
#include "i2c.h"

#define NULL_ENTRY (MenuItem*)0
#define NULL_FUNC (FunctionPtr)0

extern const char strDrive[];
extern const char strMouse[];
extern const char strIr[];
extern const char strCustomProfile[];

extern const char strNull[];
extern const char sel_item[];
extern const char strRecording[];

extern const char* profiles_table[];
extern const char* drive_states[];
extern const char* mouse_states[];
extern const char* ir_states[];
extern const char* custom_prof_params[];

typedef struct MenuItem;

typedef void (*FunctionPtr)(MenuItem*);

struct MenuItem {
    MenuItem *previous;
    MenuItem *next;
    MenuItem *parent;
    MenuItem *sibling;

    const char *name;

    FunctionPtr enter_func;
    byte func_param;
    boolean active;
};

struct MenuChangeEvent {
    const MenuItem &from;
    const MenuItem &to;
};

struct MenuUseEvent {
    const MenuItem &item;
};

typedef void (*cb_change)(MenuChangeEvent);
typedef void (*cb_use)(MenuUseEvent);

class MenuBackend {
    public:
        MenuBackend(MenuItem* initial_mi, cb_use menuUse,
                    cb_change menuChange = 0);

        MenuItem &getCurrent();

        void next();
        void previous();
        void parent();
        void sibling();
        void use();
        boolean setCurrent( MenuItem *next );
        void turnOn_Off(boolean dpad, boolean penta);
        void start();
        void stop();
        void update(int delta_x, int delta_y);
        byte executeAction();
        boolean isOn();
        boolean on;

    private:
        MenuItem *current;
        cb_change cb_menuChange;
        cb_use cb_menuUse;
};

extern MenuBackend menu;
extern byte mode;
extern byte drive_profile;
extern int minutes_on;

void createMenuItem(MenuItem& mi, const char* name, MenuItem* previous,
                    MenuItem* next, MenuItem* parent, MenuItem* sibling,
                    FunctionPtr func, byte param, boolean activated);
void menuInit();
void menuUse();
void menuChange();
void restoreCustomProfile();
void updateCustomProfile(int delta_x, int delta_y);
void enterPassword(int, int);
void verifyPassword();

#endif
