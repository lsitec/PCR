/*
 * input.cpp
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
#include "input.h"

int inputMode;

long speed_total = PENTA_MED * INPUT_SAMPLES;
long direction_total = PENTA_MED * INPUT_SAMPLES;
int speed_readings[INPUT_SAMPLES] = {
PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED,
PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED,
};
int direction_readings[INPUT_SAMPLES] = {
PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED,
PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED, PENTA_MED,
};
int index = 0;
int speed_average;
int direction_average;

boolean bool_read_pin(int pin)
{
    boolean button_state = ( digitalRead(pin) == LOW ) ? true : false;
    return button_state;
}

void calculate_average(const int speed, const int direction){
    speed_total = speed_total - speed_readings[index];
    direction_total = direction_total - direction_readings[index];

    speed_readings[index] = speed;
    direction_readings[index] = direction;

    speed_total = speed_total + speed_readings[index];
    direction_total = direction_total + direction_readings[index];

    index = index + 1;

    if (index >= INPUT_SAMPLES)
        index = 0;

    speed_average = speed_total / INPUT_SAMPLES;
    direction_average = direction_total / INPUT_SAMPLES;
}

void detectInputMode(){
    int reference = analogRead(refPin);

    if ( REF_MIN <= reference && reference <= REF_MAX)
        inputMode = ANALOG;
    else{
        inputMode = DIGITAL;
        pinMode(p_frontPin, INPUT);
        pinMode(p_backPin, INPUT);
        pinMode(p_leftPin, INPUT);
        pinMode(p_rightPin, INPUT);
        pinMode(p_buttonPin, INPUT);
    }

    return;
}

boolean isPresent(){
    int detect = digitalRead(detectPin);

    if (detect == LOW)
        return true;
    else
        return false;
}

void inputAbstraction(Input *input){
    if (!isPresent() && inputMode == DIGITAL){
        input->deltaX = PENTA_MED;
        input->deltaY = PENTA_MED;
        input->menu = false;
        return;
    }

    if (inputMode == ANALOG){
            int speed = analogRead(speedPin);
            int direction = analogRead(dirPin);
            int reference = analogRead(refPin);
            float y = (float) (speed - reference) /(float) reference;
            float x = (float) (direction - reference) / (float) reference;

            int newSpeed = (int) 2500*y + 500;
            int newDirection = (int) 2500*x + 500;

            if (newSpeed < PENTA_MIN) newSpeed = PENTA_MIN;
            if (newSpeed > PENTA_MAX) newSpeed = PENTA_MAX;

            if (newDirection < PENTA_MIN) newDirection = PENTA_MIN;
            if (newDirection > PENTA_MAX) newDirection = PENTA_MAX;

            calculate_average(newSpeed, newDirection);

            input->deltaY = speed_average;
            input->deltaX = direction_average;
    }
    else {
        int front = digitalRead(p_frontPin);
        int back = digitalRead(p_backPin);
        int left = digitalRead(p_leftPin);
        int right = digitalRead(p_rightPin);

        if (front==LOW && back==LOW)
            input->deltaY = PENTA_MED;
        else{
            if (front==LOW)
                input->deltaY = PENTA_MAX;
            else if (back==LOW)
                input->deltaY = PENTA_MIN;
            else
                input->deltaY = PENTA_MED;
        }
        if (left==LOW && right==LOW)
            input->deltaX = PENTA_MED;
        else{
            if (left==LOW)
                input->deltaX = PENTA_MIN;
            else if (right==LOW)
                input->deltaX = PENTA_MAX;
            else
                input->deltaX = PENTA_MED;
        }

        input->menu = bool_read_pin(p_buttonPin);
    }
    input->p2 = bool_read_pin(p2Pin);
    input->dpad_up = bool_read_pin(dpad_upPin);
    input->dpad_down = bool_read_pin(dpad_downPin);
    input->dpad_left = bool_read_pin(dpad_leftPin);
    input->dpad_right = bool_read_pin(dpad_rightPin);
    input->dpad_ok = bool_read_pin(dpad_okPin);

    return;
}

int getInputMode(){
    return inputMode;
}

int getJoystickRef(){
    return analogRead(refPin);
}
