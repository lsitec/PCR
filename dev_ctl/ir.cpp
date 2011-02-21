/*
 * ir.cpp
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

#include "ir.h"

unsigned int TimerValue[SAMPLE_SIZE];
byte change_count;

unsigned int addr;
int endAddr;

void irInit()
{
    TCCR1A = 0x00;
    TCCR1B = 0x03;
    TIMSK1 = 0x00;
}

void writeI2cEEPROM(unsigned int eeaddress, byte data)
{
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.send(highByte(eeaddress));   // MSB
    Wire.send(lowByte(eeaddress)); // LSB
    Wire.send(data);
    Wire.endTransmission();
}

byte readEEPROM(unsigned int eeaddress)
{
    byte rdata = 0xFF;
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.send(highByte(eeaddress));   // MSB
    Wire.send(lowByte(eeaddress)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(EEPROM_ADDR,1);
    if (Wire.available())
        rdata = Wire.receive();
    return rdata;
}

void IRrx(byte device, byte button)
{
    boolean aux = 0;

    Serial.println("RX - Init");

    change_count = 0;
    while(digitalRead(ir_rxdPin) == HIGH) {}
    TIMER_RESET;
    TimerValue[change_count++] = TCNT1;

    while (change_count < SAMPLE_SIZE)
    {
        if (aux)
        {
            while(digitalRead(ir_rxdPin) == HIGH) {}
        }
        else
        {
            while(digitalRead(ir_rxdPin) == LOW) {}
        }
        TimerValue[change_count++] = TCNT1;
        aux ^= 1;
    }

    change_count = 0;
    addr = device*1024 + button*100;
    endAddr = addr + 2*SAMPLE_SIZE;

    while(addr < endAddr)
    {
        writeI2cEEPROM(addr++, highByte(TimerValue[change_count]));
        delayMicroseconds(3000);
        writeI2cEEPROM(addr++, lowByte(TimerValue[change_count]));
        delayMicroseconds(3000);
        change_count++;

        Serial.print(TimerValue[change_count-1]);
        Serial.print(" | ");
    }
    Serial.print("\n\nRX - End\n");
}

void IRtx(byte device, byte button)
{
    unsigned int pause;
    change_count = 0;
    boolean aux = 1;

    digitalWrite(ir_txdPin, LOW);

    Serial.println("TX - Init");

    addr = device*1024 + button*100;
    endAddr = addr + 2*SAMPLE_SIZE;

    while(addr < endAddr)
    {
        TimerValue[change_count] = word(readEEPROM(addr),readEEPROM(addr+1));
        addr += 2;
        change_count++;

        Serial.print(TimerValue[change_count-1]);
        Serial.print(" | ");
    }
    Serial.print("\n\nTX - End\n");
 
    change_count = 0;

    while(change_count < SAMPLE_SIZE)
    {
        if(change_count < SAMPLE_SIZE-1)
            pause = (TimerValue[change_count+1] - TimerValue[change_count])*4;
        else
            pause = 1;

        if(aux)
        {
            digitalWrite(ir_txdPin, HIGH);
        }
        else
        {
            digitalWrite(ir_txdPin, LOW);
        }
        delayMicroseconds(pause);
        aux ^= 1;
        change_count++;
    }
    digitalWrite(ir_txdPin, LOW);

    for(int i=0; i<10; i++)
        delayMicroseconds(1000);
}
