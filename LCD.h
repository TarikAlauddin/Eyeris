#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>
#include <Arduino.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

void lcdSetup(); 
void clearLCD();
void welcomeToEyeris();
void lcdSystemSetup();
void lcdUserSetting(int userLevel);
void lcdBatLevelUpdate(int routBatCheck[]);
void nibbleWrite(String str, int delayMs);

#endif
