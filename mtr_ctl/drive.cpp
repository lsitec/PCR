/*
 * drive.cpp
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

#include "drive.h"

DriveProfile profile[NPROFILES] = {
           {"Outdoor fast" , false, 35, 180, 15, 2, 50 , 0.6, 0.5},
           {"Outdoor slow" , false, 35, 130, 15, 2, 50 , 0.7, 0.5},
           {"Indoor fast"  , true , 25, 100, 5 , 2, 50 , 0.8, 0.7},
           {"Indoor slow"  , false, 25, 80 , 5 , 2, 50 , 1.0, 0.7},
           {"Custom"       , false, 25, 80 , 5 , 2, 50 , 1.0, 0.7},
};

int j_table_pos [9] =
    { MPOS, 589, 602, 623, 653, 699, 767, 869, POS };
int j_table_neg [9] =
    { MNEG, 431, 418, 398, 368, 322, 255, 153, NEG };

int current_drive_state = Dead;
int last_drive_state = Dead;

int last_x = PENTA_MED, last_y = PENTA_MED;

int delta_x, delta_y;

unsigned long emergencyTimer = millis();

unsigned long brake_timer;
boolean start_direction;
unsigned long stop_timer;

float speed_ratio = SPEED_RATIO_MAX;
byte global_speed_limit = GLOBAL_SPEED_LIMIT_MAX;

int index = 0;
int pwm1_readings[PWM_SAMPLES];
int pwm1_total = 0;
int pwm1_average = 0;
int pwm2_readings[PWM_SAMPLES];
int pwm2_total = 0;
int pwm2_average = 0;

int updateState(int x, int y, boolean p_button) {
    if (p_button && !isStopped()){
        emergencyTimer = millis();
        return Emergency;
    }
    unsigned int timeout = millis() - emergencyTimer;
    if (timeout < EMERGENCY_STOP && last_drive_state == Emergency)
        return Emergency;

    int next_state;
    if (y < MNEG || last_drive_state == Back && y<MNEG+HYSTERESIS)
        next_state = Back;
    else if (y > MPOS || last_drive_state == Front && y>MPOS-HYSTERESIS)
        next_state = Front;
    else if (x < MNEG || last_drive_state == Left && x<MNEG+HYSTERESIS)
        next_state = Left;
    else if (x > MPOS || last_drive_state == Right && x>MPOS-HYSTERESIS)
        next_state = Right;
    else
        next_state = Dead;

    if (isStopped() || next_state == last_drive_state)
        return next_state;
    else
        return Dead;
}

int updateDirection(int value) {
    if (value < MNEG)
        return Left;
    else if (value > MPOS)
        return Right;
    else
        return Dead;
}

int updateSpeed(int value) {
    if (value < MNEG)
        return Back;
    else if (value > MPOS)
        return Front;
    else
        return Dead;
}

int calculateNegPWM(int val) {
    int active = getActiveProfile();
    int maxspeed = profile[active].maxSpeed;
    int resolution = speed_ratio * maxspeed;
    int minimal = profile[active].initialSpeed;

    int result;
    int resolution_step = (resolution - minimal) / 8;

    if (val <= NEG)
        return min(resolution, global_speed_limit);

    for (int i=0; i<9; i++){
        if (j_table_neg[i] >= val && val > j_table_neg[i+1]){
            result = minimal + i * resolution_step;
        }
    }

    result = min(result, global_speed_limit);

    if (result < minimal)
        return minimal;
    if (result > resolution)
        return resolution;
    else
        return result;
}

int calculatePosPWM(int val) {
    int active = getActiveProfile();
    int maxspeed = profile[active].maxSpeed;
    int resolution = speed_ratio * maxspeed;
    int minimal = profile[active].initialSpeed;
    int result;
    int resolution_step = (resolution - minimal) / 8;

    if (val >= POS)
        return min(resolution, global_speed_limit);

    for (int i=0; i<9; i++){
        if (j_table_pos[i] <= val && val < j_table_pos[i+1]){
            result = minimal + i * resolution_step;
        }
    }

    result = min(result, global_speed_limit);

    if (result < minimal)
        return minimal;
    if (result > resolution)
        return resolution;
    else
        return result;
}

int calculateNegPWM(int val, int limit){
    int pwm = calculateNegPWM(val);

    if (pwm <= limit)
        return pwm;
    else
        return limit;
}

int calculatePosPWM(int val, int limit){
    int pwm = calculatePosPWM(val);

    if (pwm <= limit)
        return pwm;
    else
        return limit;
}


int drive(const int x_input, const int y_input, boolean p_button){
  current_drive_state = updateState(x_input, y_input, p_button);

  delta_x = do_hysteresis(x_input, last_x);
  delta_y = do_hysteresis(y_input, last_y);
  last_x = x_input;
  last_y = y_input;

  int pwm_x = 0;
  int pwm_y = 0;

  switch (current_drive_state) {
    case Back:
        if (start_direction){
            start_direction = false;
            digitalWrite(inv1Pin, HIGH);
            digitalWrite(inv2Pin, HIGH);
            brake_timer = millis();
        }
        else if (millis() - brake_timer > BRAKE_DELAY){
            digitalWrite(en1Pin, HIGH);
            digitalWrite(en2Pin, HIGH);
            digitalWrite(brakePin, HIGH);
            digitalWrite(emergencyPin, HIGH);

            int rear_speed = getRearSpeedLimit();
            pwm_y = calculateNegPWM(delta_y, rear_speed);

            int direction = updateDirection(delta_x);
            int diff1, diff2, lateral;

            switch (direction) {
                case Dead:
                    diff1 = 0;
                    diff2 = 0;
                    break;

                case Right:
                    lateral = calculatePosPWM(delta_x);

                    diff1 = calculateDiagonalDiff(pwm_y, lateral);
                    diff2 = 0;
                    break;

                case Left:
                    lateral = calculateNegPWM(delta_x);

                    diff1 = 0;
                    diff2 = calculateDiagonalDiff(pwm_y, lateral);
                    break;
            }

            do_pwm_average(pwm_y-diff1, pwm_y-diff2);
            controlMotors(0,  pwm1_average);
            controlMotors(1,  pwm2_average);
        }

        break;

    case Front:
        if (start_direction){
            start_direction = false;
            digitalWrite(inv1Pin,  LOW);
            digitalWrite(inv2Pin,  LOW);
            brake_timer = millis();
        }
        else if (millis() - brake_timer > BRAKE_DELAY){
            digitalWrite(en1Pin, HIGH);
            digitalWrite(en2Pin, HIGH);
            digitalWrite(brakePin, HIGH);
            digitalWrite(emergencyPin, HIGH);

            pwm_y = calculatePosPWM(delta_y);

            int direction = updateDirection(delta_x);
            int diff1, diff2, lateral;

            switch (direction) {
                case Dead:
                    diff1=0;
                    diff2=0;
                    break;

                case Right:
                    lateral = calculatePosPWM(delta_x);

                    diff1 = calculateDiagonalDiff(pwm_y, lateral);
                    diff2 = 0;
                    break;

                case Left:
                    lateral = calculateNegPWM(delta_x);

                    diff1 = 0;
                    diff2 = calculateDiagonalDiff(pwm_y, lateral);
                    break;
            }

            do_pwm_average(pwm_y-diff1, pwm_y-diff2);
            controlMotors(0,  pwm1_average);
            controlMotors(1,  pwm2_average);
        }

        break;

    case Left:
        if (start_direction){
            start_direction = false;
            digitalWrite(inv2Pin,  HIGH);
            digitalWrite(inv1Pin,  LOW);
            brake_timer = millis();
        }
        else if (millis() - brake_timer > BRAKE_DELAY){
            digitalWrite(en1Pin, HIGH);
            digitalWrite(en2Pin, HIGH);
            digitalWrite(brakePin, HIGH);
            digitalWrite(emergencyPin, HIGH);

            int turn_speed = getTurnSpeedLimit();
            pwm_x = calculateNegPWM(delta_x, turn_speed);

            do_pwm_average(pwm_x, pwm_x);
            controlMotors(0, pwm1_average);
            controlMotors(1, pwm2_average);
        }

        break;

    case Right:
        if (start_direction){
            start_direction = false;
            digitalWrite(inv1Pin,  HIGH);
            digitalWrite(inv2Pin,  LOW);
            brake_timer = millis();
        }
        else if (millis() - brake_timer > BRAKE_DELAY){
            digitalWrite(en1Pin, HIGH);
            digitalWrite(en2Pin, HIGH);
            digitalWrite(brakePin, HIGH);
            digitalWrite(emergencyPin, HIGH);

            int turn_speed = getTurnSpeedLimit();
            pwm_x = calculatePosPWM(delta_x, turn_speed);

            do_pwm_average(pwm_x, pwm_x);
            controlMotors(0,  pwm1_average);
            controlMotors(1,  pwm2_average);
        }

        break;

    case Dead:
        do_pwm_average(0,0);
        controlMotors(0, 0);
        controlMotors(1, 0);

        if (isStopped()){
            if (millis() - stop_timer >= STOP_DELAY){
                digitalWrite(en1Pin, LOW);
                digitalWrite(en2Pin, LOW);
                digitalWrite(brakePin, LOW);
                digitalWrite(emergencyPin, LOW);
            }
            start_direction = true;
        }
        else{
            digitalWrite(en1Pin, HIGH);
            digitalWrite(en2Pin, HIGH);
            digitalWrite(brakePin, HIGH);
            digitalWrite(emergencyPin, HIGH);
            stop_timer = millis();
        }
        break;

    case Emergency:
      digitalWrite(emergencyPin, LOW);
      controlMotors(0, 0);
      controlMotors(1, 0);
      digitalWrite(en1Pin, LOW);
      digitalWrite(en2Pin, LOW);
      digitalWrite(brakePin, LOW);

      break;
  }
  last_drive_state = current_drive_state;
  return last_drive_state;
}

int getActiveProfile(){

    int retVal;
    for (int i=0; i<NPROFILES; i++){
        if (profile[i].active)
            retVal = i;
    }

    return retVal;
}

char* getProfileName(int n){
    return profile[n].name;
}

void changeActiveProfile(int newprofile){
    int current = getActiveProfile();

    profile[current].active = false;

    if (newprofile>=0 && newprofile<NPROFILES){
        profile[newprofile].active = true;
    }
    else{
        if (newprofile<0)
            newprofile = NPROFILES - 1;
        if (newprofile>=NPROFILES)
            newprofile = 0;

        profile[newprofile].active = true;
    }

    int stepUp = profile[newprofile].stepUp;
    int brakeRatio = profile[newprofile].brakeRatio;
    int stepTime = profile[newprofile].stepTime;

    changeMotorParameters(0, stepUp, brakeRatio, stepTime);
    changeMotorParameters(1, stepUp, brakeRatio, stepTime);

    byte last = EEPROM.read(LAST_ACTIVE_PROFILE);
    if (last != newprofile)
        EEPROM.write(LAST_ACTIVE_PROFILE, newprofile);

    return;
}

boolean isStopped(){
    boolean motor1 = motorStopped(0);
    boolean motor2 = motorStopped(1);

    return motor1 && motor2;
}

void initDriveMode(){
    TCCR2B = TCCR2B & 0b11111000 | 0x01;

    for (int thisReading = 0; thisReading < PWM_SAMPLES; thisReading++){
        pwm1_readings[thisReading] = 0;
        pwm2_readings[thisReading] = 0;
    }

    recoverCustomSettings();

    return;
}

int calculateDiagonalDiff(const int& forward, const int& lateral){
    int diff = lateral / (forward >> PWM_TURN_RATIO);
    
    if (forward - diff < 0)
        return forward;
    else
        return diff;
}

int getRearSpeedLimit() {
    int active = getActiveProfile();

    return profile[active].maxSpeed * profile[active].rearSpeedRatio;
}

int getTurnSpeedLimit() {
    int active = getActiveProfile();

    return profile[active].maxSpeed * profile[active].turnSpeedRatio;
}

void changeSpeedRatio(float ratio) {
    if (ratio < SPEED_RATIO_MIN)
        speed_ratio = SPEED_RATIO_MIN;
    else if (ratio > SPEED_RATIO_MAX)
        speed_ratio = SPEED_RATIO_MAX;
    else
        speed_ratio = ratio;
    return;
}

int get_pos_table_index(int val) {
    int index=-1;
    for (int i=0; i<9; i++){
        if (j_table_pos[i] <= val && val < j_table_pos[i+1]){
            index = i;
        }
    }
    return index;
}

int get_neg_table_index(int val) {
    int index=-1;
    for (int i=0; i<9; i++){
        if (j_table_neg[i] >= val && val > j_table_neg[i+1]){
            index = i;
        }
    }
    return index;
}

int do_hysteresis(int val, const int last_val) {
    int val_index, last_index;

    if (val > PENTA_MED){
        val_index = get_pos_table_index(val);
        last_index = get_pos_table_index(last_val);

        int diff = j_table_pos[last_index] - val;

        if (val_index == last_index-1 && diff < HYSTERESIS)
            return j_table_pos[last_index];
        else
            return val;
    }
    else{
        val_index = get_neg_table_index(val);
        last_index = get_neg_table_index(last_val);

        int diff = val - j_table_neg[last_index];

        if (val_index == last_index-1 && diff < HYSTERESIS)
            return j_table_neg[last_index];
        else
            return val;
    }
}

void do_pwm_average(int pwm1, int pwm2) {
    pwm1_total = pwm1_total - pwm1_readings[index];
    pwm2_total = pwm2_total - pwm2_readings[index];
    pwm1_readings[index] = pwm1;
    pwm2_readings[index] = pwm2;
    pwm1_total= pwm1_total + pwm1_readings[index];
    pwm2_total= pwm2_total + pwm2_readings[index];
    index = index + 1;

    if (index >= PWM_SAMPLES)
        index = 0;

    pwm1_average = pwm1_total / PWM_SAMPLES;
    pwm2_average = pwm2_total / PWM_SAMPLES;
}

uint8_t custom_speed_index = 0;
void increaseCustomMaxspeed() {
    custom_speed_index++;
    changeCustomMaxSpeed();
}

void decreaseCustomMaxspeed() {
    custom_speed_index--;
    changeCustomMaxSpeed();
}

void changeCustomMaxSpeed() {
    DriveProfile& custom = profile[NPROFILES-1];
    int step = (MAX_CUSTOM_MAXSPEED - MIN_CUSTOM_MAXSPEED) / (CUSTOM_ZONES-1);

    custom_speed_index = constrain(custom_speed_index, 0, CUSTOM_ZONES-1);
    custom.maxSpeed = MIN_CUSTOM_MAXSPEED + step * custom_speed_index;

    byte recorded = EEPROM.read(CUSTOM_MAXSPEED);
    if (recorded != custom.maxSpeed)
        EEPROM.write(CUSTOM_MAXSPEED, custom.maxSpeed);
}

uint8_t custom_stepup_index = 0;
void increaseCustomStepUp() {
    custom_stepup_index++;
    changeCustomStepUp();
}

void decreaseCustomStepUp() {
    custom_stepup_index--;
    changeCustomStepUp();
}

void changeCustomStepUp() {
    DriveProfile& custom = profile[NPROFILES-1];
    int step = (MAX_CUSTOM_STEPUP - MIN_CUSTOM_STEPUP ) / (CUSTOM_ZONES-1);

    custom_stepup_index = constrain(custom_stepup_index, 0, CUSTOM_ZONES-1);
    custom.stepUp = MIN_CUSTOM_STEPUP + step * custom_stepup_index;

    byte recorded = EEPROM.read(CUSTOM_STEPUP);
    if (recorded != custom.stepUp)
        EEPROM.write(CUSTOM_STEPUP, custom.stepUp);
}

void recoverCustomSettings(){
    byte step;
    DriveProfile& custom = profile[NPROFILES-1];

    byte speed = EEPROM.read(CUSTOM_MAXSPEED);
    if (!speed)
        speed = MIN_CUSTOM_MAXSPEED;

    step = (MAX_CUSTOM_MAXSPEED - MIN_CUSTOM_MAXSPEED) / (CUSTOM_ZONES-1);
    custom_speed_index = (speed - MIN_CUSTOM_MAXSPEED) / step;

    byte stepup = EEPROM.read(CUSTOM_STEPUP);
    if (!stepup)
        speed = MIN_CUSTOM_STEPUP;

    step = (MAX_CUSTOM_STEPUP - MIN_CUSTOM_STEPUP ) / (CUSTOM_ZONES-1);
    custom_stepup_index = (speed - MIN_CUSTOM_STEPUP) / step;

    custom.maxSpeed = speed;
    custom.stepUp = stepup;

    byte active = EEPROM.read(LAST_ACTIVE_PROFILE);
    changeActiveProfile(active);

    byte limit = EEPROM.read(GLOBAL_SPEED_LIMIT);
    if (!limit)
        global_speed_limit = GLOBAL_SPEED_LIMIT_MAX;
}

void increaseGlobalSpeedLimit() {
    global_speed_limit++;
    changeGlobalSpeedLimit();
}

void decreaseGlobalSpeedLimit() {
    global_speed_limit--;
    changeGlobalSpeedLimit();
}

void changeGlobalSpeedLimit() {
    global_speed_limit = constrain(global_speed_limit, GLOBAL_SPEED_LIMIT_MIN, GLOBAL_SPEED_LIMIT_MAX);

    byte recorded = EEPROM.read(GLOBAL_SPEED_LIMIT);
    if (recorded != global_speed_limit)
        EEPROM.write(GLOBAL_SPEED_LIMIT, global_speed_limit);
}
