/*
 * motors.cpp
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
#include "motors.h"

Motor motores[2] = {
    {0, 10, 2, 100, pwm1Pin, 0},
    {0, 10, 2, 100, pwm2Pin, 0}
};

void controlMotors(int n, int pwmGoal){
    Motor& motor = motores[n];
    
    unsigned long timer = millis();

    if (pwmGoal > motor.currentPwm){
        if (timer % motor.stepTime == 0 && motor.counter != timer){
            motor.counter = timer;
            int value = motor.currentPwm + motor.stepUp;

            if (0<pwmGoal && pwmGoal<MINIMAL_PWM)
                pwmGoal = MINIMAL_PWM;

            if (value > pwmGoal)
                motor.currentPwm = pwmGoal;
            else
                motor.currentPwm = value;
        }
    }

    if (pwmGoal < motor.currentPwm){
        if (timer % motor.stepTime == 0 && motor.counter != timer){
            motor.counter = timer;

            int value;
            int diff = motor.currentPwm - pwmGoal;

            if (diff >= FAST_BRAKING_LIMIT)
                value = motor.currentPwm - (diff >> FAST_BRAKING_RATIO);
            else if (FAST_BRAKING_LIMIT > diff && diff >= SLOW_BRAKING_LIMIT)
                value = motor.currentPwm - (diff >> SLOW_BRAKING_RATIO);
            else if (SLOW_BRAKING_LIMIT > diff)
                value = pwmGoal;

            if (0<value && value<MINIMAL_PWM)
                value = 0;

            if (value < 0)
                motor.currentPwm = 0;
            else if (value < pwmGoal)
                motor.currentPwm = pwmGoal;
            else
                motor.currentPwm = value;
        }
    }
    analogWrite(motor.pin, motor.currentPwm);
}

boolean motorStopped(int n){
    Motor& motor = motores[n];

    if (motor.currentPwm == 0)
        return true;
    else
        return false;
}

int motorPwm(int n){
    Motor& motor = motores[n];

    return motor.currentPwm;
}

void changeMotorParameters(int n, int stepUp, int brakeRatio, int stepTime){
    Motor& motor = motores[n];

    motor.stepUp = stepUp;
    motor.brakeRatio = brakeRatio;
    motor.stepTime = stepTime;

    return;
}
