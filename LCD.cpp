// Tarik Alauddin
// lcd driver with desired functionality for Eyeris Light Controller

#include "LCD.h"

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// define spaces on screen
void lcdSetup()
{
    lcd.begin(16,2);                  // define size of LCD
    welcomeToEyeris();                // welcome sequence
}

// clearing the LCD
void clearLCD()
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

// welcome sequence to eyeris
void welcomeToEyeris()
{
  lcd.setCursor(0,0);
  nibbleWrite("Welcome To", 200);
  lcd.setCursor(0,1);
  nibbleWrite("Eyeris", 200);
  delay(3000);
  clearLCD();
}

// setup sequence
void lcdSystemSetup()
{
  lcd.setCursor(0,0);
  nibbleWrite("Follow System", 200);
  lcd.setCursor(0,1);
  nibbleWrite("Setup Procedures", 200);
  clearLCD();
}

// display user-defined light setting
void lcdUserSetting(int userLevel)
{
  lcd.setCursor(0,0);
  lcd.print("User Set: ");
  lcd.setCursor(10, 0);
  lcd.print(userLevel);
}

// check battery levels and display to user
void lcdBatLevelUpdate(int routBatCheck[])
{
  lcd.setCursor(0,1);
  lcd.print("Replace: ");
  
  if (!routBatCheck[0])
  {
    lcd.setCursor(9,1);
    lcd.print("1");
  } else
  {
    lcd.setCursor(9,1);
    lcd.print(" ");
  }
  
  if (!routBatCheck[1])
  {
    lcd.setCursor(10,1);
    lcd.print("2");
  } else
  {
    lcd.setCursor(10,1);
    lcd.print(" ");
  }

  
  if (!routBatCheck[2])
  {
    lcd.setCursor(11,1);
    lcd.print("3");
  } else
  {
    lcd.setCursor(11,1);
    lcd.print(" ");
  }
}

// given a string and delay, write one character at a time
void nibbleWrite(String str, int delayMs)
{
  for (int i = 0; i < str.length(); i++)
  {
    lcd.print(str[i]);   delay(delayMs);
  }
}
