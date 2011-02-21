/*
 *  menu.cpp
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

#include <menu.h>

Password password = Password("1000");

char menu_buffer[16] = "";

const char sel_item[] PROGMEM = "> ";
const char unsel_item[] PROGMEM = "  ";
const char active_mark[] PROGMEM = " *";
const char space[] PROGMEM = " ";
const char ok[] PROGMEM = "OK";
const char strRecording[] PROGMEM = "Gravando...";

const char strBusy[] PROGMEM = "Disp. ocupado";

const char strBack[] PROGMEM = "Tras";
const char strFront[] PROGMEM = "Frente";
const char strLeft[] PROGMEM = "Esquerda";
const char strRight[] PROGMEM = "Direita";
const char strIdle[] PROGMEM = "Parado";
const char strEr[] PROGMEM = "Emergencia";

PROGMEM const char* drive_states[] =
{
    strBusy,
    strBack,
    strFront,
    strLeft,
    strRight,
    strIdle,
    strEr
};

const char strMouseNotConn[] PROGMEM = "Desconectado";
const char strMouseNotPaired[] PROGMEM = "Nao pareado";
const char strMouseBtData[] PROGMEM = "Bluetooth ativo";
const char strMouseBtCom[] PROGMEM = "BT Com.";
const char strMouseUsb[] PROGMEM = "Conectado USB";

PROGMEM const char* mouse_states[] =
{
    strBusy,
    strMouseNotConn,
    strMouseNotPaired,
    strMouseBtData,
    strMouseBtCom,
    strMouseUsb
};

const char strMenu[] PROGMEM = "Menu";
const char strDrive[] PROGMEM = "Direcao";
const char strProfiles[] PROGMEM = "Perfis";
const char strMouse[] PROGMEM = "Mouse";
const char strIr[] PROGMEM = "IR";
const char strConfig[] PROGMEM = "Config";

const char strOutFast[] PROGMEM = "Outdoor fast";
const char strOutSlow[] PROGMEM = "Outdoor slow";
const char strInFast[] PROGMEM = "Indoor fast";
const char strInSlow[] PROGMEM = "Indoor slow";
const char strCustom1[] PROGMEM = "Custom";

PROGMEM const char* profiles_table[] =
{
    strOutFast,
    strOutSlow,
    strInFast,
    strInSlow,
    strCustom1
};

const char strIrDev1[] PROGMEM = "Disp. 1";
const char strIrDev2[] PROGMEM = "Disp. 2";
const char strIrDev3[] PROGMEM = "Disp. 3";

const char strIrSig1[] PROGMEM = "Sinal 1";
const char strIrSig2[] PROGMEM = "Sinal 2";
const char strIrSig3[] PROGMEM = "Sinal 3";
const char strIrRec[] PROGMEM = "Gravar";

const char strIrRx[] PROGMEM = "Recebendo";
const char strIrTx[] PROGMEM = "Transmitindo";

PROGMEM const char* ir_states[] =
{
    strBusy,
    strIrRx,
    strIrTx
};

const char strConfUnlock[] PROGMEM = "Desbloquear";
const char strConfLock[] PROGMEM = "Bloquear";
const char strConfClk[] PROGMEM = "Tempo de uso";
const char strConfReset[] PROGMEM = "Reiniciado";
const char strConfClkRes[] PROGMEM = "Reinic. tempo";
const char strHours[] PROGMEM = " minutos";
const char strCustomProfile[] PROGMEM = "Ajus. perfil";

const char strCustomMaxSpeed[] PROGMEM = "Vel. max.    ";
const char strCustomStepUp[] PROGMEM =   "Aceler.      ";
const char strCustomStepDown[] PROGMEM = "Desaceler.   ";

PROGMEM const char* custom_prof_params[] =
{
    strCustomMaxSpeed,
    strCustomStepUp,
    ok
};

const char strInd0[] PROGMEM = "^   ";
const char strInd1[] PROGMEM = " ^  ";
const char strInd2[] PROGMEM = "  ^ ";
const char strInd3[] PROGMEM = "   ^";
const char strInd4[] PROGMEM = "     ^^";


const char strNull[] PROGMEM = "";

PROGMEM const char* indicators[] =
{
    strInd0,
    strInd1,
    strInd2,
    strInd3,
    strInd4
};

MenuItem miDrive;
MenuItem miProfiles;
MenuItem miMouse;
MenuItem miIr;
MenuItem miConfig;

MenuItem miOutSlow;
MenuItem miOutFast;
MenuItem miInSlow;
MenuItem miInFast;
MenuItem miCustom1;

MenuItem miDev1;
MenuItem miDev2;
MenuItem miDev3;

MenuItem miDev1Sig1;
MenuItem miDev1Sig2;
MenuItem miDev1Sig3;
MenuItem miDev1Rec;

MenuItem miDev1Sig1Rec;
MenuItem miDev1Sig2Rec;
MenuItem miDev1Sig3Rec;

MenuItem miDev2Sig1;
MenuItem miDev2Sig2;
MenuItem miDev2Sig3;
MenuItem miDev2Rec;

MenuItem miDev2Sig1Rec;
MenuItem miDev2Sig2Rec;
MenuItem miDev2Sig3Rec;

MenuItem miDev3Sig1;
MenuItem miDev3Sig2;
MenuItem miDev3Sig3;
MenuItem miDev3Rec;

MenuItem miDev3Sig1Rec;
MenuItem miDev3Sig2Rec;
MenuItem miDev3Sig3Rec;

MenuItem miConfLock;
MenuItem miConfUnlock;
MenuItem miConfClk;
MenuItem miConfClkRes;

MenuItem miCustomProfile;

void createMenuItem(MenuItem& mi, const char* name, MenuItem* previous,
                    MenuItem* next, MenuItem* parent, MenuItem* sibling,
                    FunctionPtr func, byte param=0, boolean active=false)
{
    mi.name = name;
    mi.previous = previous;
    mi.next = next;
    mi.parent = parent;
    mi.sibling = sibling;
    mi.enter_func = func;
    mi.func_param = param;
    mi.active = active;
}

void changeMode(MenuItem* mi)
{
    mode = mi->func_param;
    menu.on = false;
}

void resetUsageTime(MenuItem* mi)
{
    EEPROM.write(TIME_CTR_ADDR, 0);
    EEPROM.write(TIME_CTR_ADDR+1, 0);
    minutes_on = 0;

    strcpy_P(screen.title, strConfClkRes);
    strcpy_P(screen.content[0], strNull);
    strcpy_P(screen.content[1], strConfClk);
    strcpy_P(screen.content[2], strConfReset);
}

void showUsageTime(MenuItem* mi)
{
    int usage_time = word(EEPROM.read(TIME_CTR_ADDR), EEPROM.read(TIME_CTR_ADDR+1));
    strcpy_P(screen.title, strConfClk);
    strcpy_P(screen.content[0], strNull);
    itoa(usage_time, screen.content[1], 10);
    strcat_P(screen.content[1], strHours);
    strcpy_P(screen.content[2], strNull);
}

void changeProfile(MenuItem* mi)
{
    MenuItem* temp = mi->next;

    drive_profile = mi->func_param;
    mi->active = true;

    strcpy_P(menu_buffer, mi->name);
    while(strcmp_P(menu_buffer, temp->name) != 0)
    {
        temp->active = false;
        temp = temp->next;
    }

    mode = DRIVE_MODE;
    menu.on = false;
}

void sendIrRxMsg(MenuItem* mi)
{
    sendPack2Dev(IR_MODE,DIGITAL,(mi->func_param/10),(mi->func_param%10),true);
}

void sendIrTxMsg(MenuItem* mi)
{
    sendPack2Dev(IR_MODE,DIGITAL,(mi->func_param/10),(mi->func_param%10),false);
}

void menuUse(MenuUseEvent used)
{
}

void menuChange(MenuChangeEvent changed)
{
    if (changed.to.parent)
        strcpy_P(screen.title, changed.to.parent->name);
    else
        strcpy_P(screen.title, strMenu);

    strcpy_P(screen.content[0], sel_item);
    strcat_P(screen.content[0], changed.to.name);
    if (changed.to.active)
        strcat_P(screen.content[0], active_mark);

    strcpy_P(screen.content[1], strNull);
    strcpy_P(screen.content[2], strNull);

    if(changed.to.next)
    {
        strcpy_P(screen.content[1], unsel_item);
        strcat_P(screen.content[1], changed.to.next->name);
        if (changed.to.next->active)
            strcat_P(screen.content[1], active_mark);

        if(changed.to.next->next)
        {
            strcpy_P(screen.content[2], unsel_item);
            strcat_P(screen.content[2], changed.to.next->next->name);
            if (changed.to.next->next->active)
                strcat_P(screen.content[2], active_mark);
        }
    }
}

MenuBackend menu = MenuBackend(&miDrive,menuUse,menuChange);

void menuInit()
{
    createMenuItem(miDrive, strDrive, &miConfig, &miProfiles, NULL_ENTRY,
                    NULL_ENTRY, changeMode, DRIVE_MODE);
    createMenuItem(miProfiles, strProfiles, &miDrive, &miMouse, NULL_ENTRY,
                    &miOutFast, NULL_FUNC);
    createMenuItem(miMouse, strMouse, &miProfiles, &miIr, NULL_ENTRY,
                    NULL_ENTRY, changeMode, MOUSE_MODE);
    createMenuItem(miIr, strIr, &miMouse, &miConfig, NULL_ENTRY,
                    &miDev1, NULL_FUNC);
    createMenuItem(miConfig, strConfig, &miIr, &miDrive, NULL_ENTRY,
                    &miConfUnlock, NULL_FUNC);

    createMenuItem(miOutFast, strOutFast, &miInSlow, &miOutSlow, &miProfiles,
                    NULL_ENTRY, changeProfile, PROF_OUT_FAST, false);
    createMenuItem(miOutSlow, strOutSlow, &miOutFast, &miInFast, &miProfiles,
                    NULL_ENTRY, changeProfile, PROF_OUT_SLOW, false);
    createMenuItem(miInFast, strInFast, &miOutSlow, &miInSlow, &miProfiles,
                    NULL_ENTRY, changeProfile, PROF_IN_FAST, true);
    createMenuItem(miInSlow, strInSlow, &miInFast, &miCustom1, &miProfiles,
                    NULL_ENTRY, changeProfile, PROF_IN_SLOW, false);
    createMenuItem(miCustom1, strCustom1, &miInSlow, &miOutFast, &miProfiles,
                    NULL_ENTRY, changeProfile, PROF_CUSTOM_1, false);


    createMenuItem(miDev1, strIrDev1, &miDev3, &miDev2, &miIr,
                    &miDev1Sig1, NULL_FUNC);
    createMenuItem(miDev2, strIrDev2, &miDev1, &miDev3, &miIr,
                    &miDev2Sig1, NULL_FUNC);
    createMenuItem(miDev3, strIrDev3, &miDev2, &miDev1, &miIr,
                    &miDev3Sig1, NULL_FUNC);

    createMenuItem(miDev1Sig1, strIrSig1, &miDev1Rec, &miDev1Sig2, &miDev1,
                    NULL_ENTRY, sendIrTxMsg, 0);
    createMenuItem(miDev1Sig2, strIrSig2, &miDev1Sig1, &miDev1Sig3, &miDev1,
                    NULL_ENTRY, sendIrTxMsg, 1);
    createMenuItem(miDev1Sig3, strIrSig3, &miDev1Sig2, &miDev1Rec, &miDev1,
                    NULL_ENTRY, sendIrTxMsg, 2);
    createMenuItem(miDev1Rec, strIrRec, &miDev1Sig3, &miDev1Sig1, &miDev1,
                    &miDev1Sig1Rec, NULL_FUNC);

    createMenuItem(miDev1Sig1Rec, strIrSig1, &miDev1Sig3Rec, &miDev1Sig2Rec, &miDev1Rec,
                    NULL_ENTRY, sendIrRxMsg, 0);
    createMenuItem(miDev1Sig2Rec, strIrSig2, &miDev1Sig1Rec, &miDev1Sig3Rec, &miDev1Rec,
                    NULL_ENTRY, sendIrRxMsg, 1);
    createMenuItem(miDev1Sig3Rec, strIrSig3, &miDev1Sig2, &miDev1Sig1Rec, &miDev1Rec,
                    NULL_ENTRY, sendIrRxMsg, 2);

    createMenuItem(miDev2Sig1, strIrSig1, &miDev2Rec, &miDev2Sig2, &miDev2,
                    NULL_ENTRY, sendIrTxMsg, 0);
    createMenuItem(miDev2Sig2, strIrSig2, &miDev2Sig1, &miDev2Sig3, &miDev2,
                    NULL_ENTRY, sendIrTxMsg, 1);
    createMenuItem(miDev2Sig3, strIrSig3, &miDev2Sig2, &miDev2Sig1, &miDev2,
                    NULL_ENTRY, sendIrTxMsg, 2);
    createMenuItem(miDev2Rec, strIrRec, &miDev2Sig3, &miDev2Sig1, &miDev2,
                    &miDev2Sig1Rec, NULL_FUNC);

    createMenuItem(miDev2Sig1Rec, strIrSig1, &miDev2Sig3Rec, &miDev2Sig2Rec, &miDev2Rec,
                    NULL_ENTRY, sendIrRxMsg, 0);
    createMenuItem(miDev2Sig2Rec, strIrSig2, &miDev2Sig1Rec, &miDev2Sig3Rec, &miDev2Rec,
                    NULL_ENTRY, sendIrRxMsg, 1);
    createMenuItem(miDev2Sig3Rec, strIrSig3, &miDev2Sig2, &miDev2Sig1Rec, &miDev2Rec,
                    NULL_ENTRY, sendIrRxMsg, 2);

    createMenuItem(miDev3Sig1, strIrSig1, &miDev3Rec, &miDev3Sig2, &miDev3,
                    NULL_ENTRY, sendIrTxMsg, 20);
    createMenuItem(miDev3Sig2, strIrSig2, &miDev3Sig1, &miDev3Sig3, &miDev3,
                    NULL_ENTRY, sendIrTxMsg, 21);
    createMenuItem(miDev3Sig3, strIrSig3, &miDev3Sig2, &miDev3Sig1, &miDev3,
                    NULL_ENTRY, sendIrTxMsg, 22);
    createMenuItem(miDev3Rec, strIrRec, &miDev3Sig3, &miDev3Sig1, &miDev3,
                    &miDev3Sig1Rec, NULL_FUNC);

    createMenuItem(miDev3Sig1Rec, strIrSig1, &miDev3Sig3Rec, &miDev3Sig2Rec, &miDev3Rec,
                    NULL_ENTRY, sendIrRxMsg, 0);
    createMenuItem(miDev3Sig2Rec, strIrSig2, &miDev3Sig1Rec, &miDev3Sig3Rec, &miDev3Rec,
                    NULL_ENTRY, sendIrRxMsg, 1);
    createMenuItem(miDev3Sig3Rec, strIrSig3, &miDev3Sig2, &miDev3Sig1Rec, &miDev3Rec,
                    NULL_ENTRY, sendIrRxMsg, 2);

    createMenuItem(miConfUnlock, strConfUnlock, NULL_ENTRY, NULL_ENTRY, &miConfig,
                    NULL_ENTRY, changeMode, CONFIG_MODE);

    createMenuItem(miConfLock, strConfLock, &miCustomProfile, &miConfClk, &miConfig,
                    NULL_ENTRY, changeMode, CONFIG_MODE);
    createMenuItem(miConfClk, strConfClk, &miConfLock, &miConfClkRes, &miConfig,
                    NULL_ENTRY, showUsageTime);
    createMenuItem(miConfClkRes, strConfClkRes, &miConfClk, &miCustomProfile, &miConfig,
                    NULL_ENTRY, resetUsageTime);
    createMenuItem(miCustomProfile, strCustomProfile, &miConfClkRes, &miConfLock, &miConfig,
                    NULL_ENTRY, changeMode, PROFILES_MODE);
}

MenuBackend::MenuBackend(MenuItem* initial_mi, cb_use menuUse, cb_change menuChange)
{
    on = false;
    current = initial_mi;
    cb_menuChange = menuChange;
    cb_menuUse = menuUse;
}

MenuItem& MenuBackend::getCurrent()
{
    return *current;
}

void MenuBackend::next()
{
    setCurrent((MenuItem*)current->next);
}

void MenuBackend::previous()
{
    setCurrent((MenuItem*)current->previous);
}

void MenuBackend::parent()
{
    setCurrent((MenuItem*)current->parent);
}

void MenuBackend::sibling()
{
    if(current->enter_func)
        current->enter_func(current);
    setCurrent((MenuItem*)current->sibling);
}

void MenuBackend::use()
{
    if (cb_menuUse) {
        MenuUseEvent mue = { *current };
        cb_menuUse(mue);
    }
}

void MenuBackend::turnOn_Off(boolean dpad, boolean penta)
{
    if ((dpad || penta))
    {
        if (on) { stop(); }
        else { start(); }
    }
}

boolean MenuBackend::isOn()
{
    return on;
}

boolean MenuBackend::setCurrent( MenuItem *next )
{
    if (!next)
        return false;

    if (cb_menuChange) {
        MenuChangeEvent mce = { *current, *next };
        cb_menuChange(mce);
    }

    current = next;

    return true;
}

void MenuBackend::start()
{
    on = true;
    restoreCustomProfile();
    setCurrent(&miDrive);
}

void MenuBackend::stop()
{
    on = false;
}

void MenuBackend::update(int delta_x, int delta_y)
{
    if (delta_y < MNEG) next();
    else if (delta_y > MPOS) previous();
    else if (delta_x < MNEG) parent();
    else if (delta_x > MPOS) sibling();
}

const static byte NUM_P_PARAMS = 2;
byte p_params_eeprom[NUM_P_PARAMS] = {0,0};
byte p_params[NUM_P_PARAMS] = {0,0};
byte p_index = 0;

void restoreCustomProfile()
{
    /*
    Wire.requestFrom(MTR_CTL_ADDR, 2);
    p_params[0] = Wire.receive(); // Max Speed
    p_params[1] = Wire.receive(); // Step Up
    p_index = 0;
    */
}

void updateCustomProfile(int delta_x, int delta_y)
{
    if (delta_y < MNEG)
    {
        p_index = (p_index<NUM_P_PARAMS) ? p_index+1 : 0;
    }
    else if (delta_y > MPOS)
    {
        p_index = (p_index!=0) ? p_index-1 : NUM_P_PARAMS;
    }
    else if (delta_x < MNEG)
    {
        p_params[p_index] = (p_params[p_index]!=0) ? p_params[p_index]-1 : 9;
    }
    else if (delta_x > MPOS && p_index != NUM_P_PARAMS)
    {
        p_params[p_index] = (p_params[p_index]!=9) ? p_params[p_index]+1 : 0;
    }
    else if (delta_x > MPOS && p_index == NUM_P_PARAMS)
    {
        sendPack2Motors(PROFILES_MODE, DIGITAL, p_params[0], p_params[1]);
        delay(300);
        menu.start();
        return;
    }
    strcpy_P(screen.title, strCustomProfile);
    for (int i=0;i<NUM_P_PARAMS;i++)
    {
        if(i==p_index)
            strcpy_P(screen.content[i], sel_item);
        else
            strcpy_P(screen.content[i], unsel_item);

        itoa(p_params[i], menu_buffer, 10);
        strcat_P(screen.content[i], (char*)pgm_read_word(&(custom_prof_params[i])));
        strcat(screen.content[i], menu_buffer);
    }
    if(p_index==NUM_P_PARAMS)
        strcpy_P(screen.content[NUM_P_PARAMS], sel_item);
    else
        strcpy_P(screen.content[NUM_P_PARAMS], unsel_item);
    strcat_P(screen.content[NUM_P_PARAMS], ok);
}

char pw_guess[PW_SIZE] = "";
byte pw_display[PW_SIZE] = {0,0,0,0};
byte pw_index = 0;
boolean lock_status = false;
boolean check_pw = false;

void enterPassword(int delta_x, int delta_y)
{
    char pw_display_str[PW_SIZE] = "";
    char number[PW_SIZE] = "";

    if (delta_y < MNEG)
    {
        if(pw_index!=PW_SIZE)
        {
            if (pw_display[pw_index]!=0)
                pw_display[pw_index] = pw_display[pw_index] - 1;
            else
                pw_display[pw_index] = 9;
        }
        else
        {
            check_pw = true;
        }
    }
    else if (delta_y > MPOS)
    {
        if(pw_index!=PW_SIZE)
        {
            if (pw_display[pw_index]!=9)
                pw_display[pw_index] = pw_display[pw_index] + 1;
            else
                pw_display[pw_index] = 0;
        }
        else
        {
            check_pw = true;
        }
    }
    else if (delta_x < MNEG)
    {
        if (pw_index!=0)
            pw_index--;
        else
            pw_index = PW_SIZE;
    }
    else if (delta_x > MPOS)
    {
        if (pw_index!=PW_SIZE)
            pw_index++;
        else
            pw_index = 0;
    }

    for(int i = 0; i<PW_SIZE; i++)
    {
        itoa(pw_display[i], number, 10);
        if(i)
            strcat(pw_display_str, number);
        else
            strcpy(pw_display_str, number);
    }

    strcpy(pw_guess, pw_display_str);
    if (check_pw)
    {
        lock_status = password.is(pw_guess);
        if (lock_status)
        {
            miConfig.sibling = &miConfLock;
            for(int i=0;i<PW_SIZE;i++) { pw_display[i] = 0; }
            menu.start();
            return;
        }
        else
        {
            miConfig.sibling = &miConfUnlock;
        }
        check_pw = false;
    }

    if (lock_status)
        strcpy_P(screen.title, strConfLock);
    else
        strcpy_P(screen.title, strConfUnlock);

    strcpy_P(screen.content[0], strNull);
    strcat_P(pw_display_str, space);
    strcat_P(pw_display_str, ok);
    strcpy(screen.content[1], pw_display_str);
    strcpy_P(screen.content[2], (char*)pgm_read_word(&(indicators[pw_index])));
}
