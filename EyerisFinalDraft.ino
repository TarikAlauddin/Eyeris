// Eyeris Final Draft
// Tarik Alauddin, Jared Alano-Gray

#include "LCD.h"

int numNodes = 3;                                                    // number of nodes
int inputCount = 0;                                                  // tracks # of sensor inputs in a period of 5 seconds
                                                                    
int router0[] = {65, 194, 248, 167};                                 // router addresses
int router1[] = {65, 194, 247, 213};
int router2[] = {65, 191, 66, 223};

int lastLightMeasurements[] = {0, 0, 0};                             // continuously tracks measurements
int preferredLightLevel = 1023;                                      // users desired light level with no ambient light present

int routBatCheck[] = {1, 1, 1};                                      // track battery levels of sensor units

float voltageDividerRatio  = ((14.7+120)/14.7);                      // measured resistance values for voltage divider

// Arduino standard setup
void setup()
{
  Serial.begin(9600);                                                // Standard Baud Rate
  lcdSetup();                                                        // lcd setup sequence
}

// reads and filters Xbee packets into readable data
void retrieveOutputUpdateData()
{
  int lightVal, analogLightLSB, analogLightMSB, analogBatLSB, analogBatMSB, routerNum;
  float batteryVoltage;
  
  if (Serial.available() >= 20)                                           // only read when detecting Xbee packet
  {
    if (Serial.read() == 0x13)                                            // Xbee starting byte denotes size ->19(dec) or 13(hex)
    {
      int address2Check[4]; 
      for (int i = 0; i < 13; i++)                                        // gathers address, skipping to lighting data
      {
        if (i == 2)      {address2Check[0] = Serial.read();}
        else if (i == 3) {address2Check[1] = Serial.read();}
        else if (i == 4) {address2Check[2] = Serial.read();}
        else if (i == 5) {address2Check[3] = Serial.read();} 
        else             {byte discardByte = Serial.read();}
      }
    
      analogLightMSB = Serial.read();                                     // read bits for lighting data
      analogLightLSB = Serial.read();                                    

      analogBatMSB = Serial.read();                                       // read bits for battery voltage level
      analogBatLSB = Serial.read();


      routerNum = determineRouter(address2Check);                                 // determine router number
      lightVal = calculateLightLevel(analogLightMSB, analogLightLSB);             // calculate light level

      batteryVoltage = calculateBatteryLevel(analogBatMSB, analogBatLSB);

      updateMasterArray(routerNum, lightVal);                                     // update the master array
      
      if (batteryVoltage > 5.8)                                                   // update battery levels
      {
        routBatCheck[routerNum] = 1;
      } else
      {
        routBatCheck[routerNum] = 0;
      }
      inputCount++;
    }
  }
}

// Determines the what router has communicated with coordinator
int determineRouter(int address[])
{
    if (address[0]==router0[0] && address[1]==router0[1] && address[2]==router0[2] && address[3]==router0[3])
    {
      return 0;
    }
    if (address[0]==router1[0] && address[1]==router1[1] && address[2]==router1[2] && address[3]==router1[3])
    {
      return 1;
    }
    if (address[0]==router2[0] && address[1]==router2[1] && address[2]==router2[2] && address[3]==router2[3])
    {
      return 2;
    }

    return 10; // router address not found
}

// calculates analog levels based on MSB and LSB packets
int calculateLightLevel(int MSB, int LSB)
{
  return LSB + (MSB*256);
}

// calculate the battery voltage level
float calculateBatteryLevel(float MSB, float LSB)
{
  return ((((LSB + (MSB*256))/1023)*1.2)*voltageDividerRatio);
}

// update the lastLightMeasurements array with newly measured light values
void updateMasterArray(int routerNum, int lightVal)
{
  lastLightMeasurements[routerNum] = lightVal;
}

// finds average lumen measurement and updates lights accordingly
void updateLights()
{
  int average = 0;                                                     // average light sensor value
  int setLevel;                                                        // temporary variables for incrementing and storing desired light vlaue
  
  for (int i = 0; i < numNodes; i++)                                   // calculate total light values measured
  {
    average += lastLightMeasurements[i];
  }
  
  average /= numNodes;                                                  // find average of measured light values
  
  setLevel = preferredLightLevel - average;                             // calculate required light level by subtracting average light reading from desired level
  setLevel = constrain(setLevel, 0, 1023);
  setLevel = map(setLevel, 0, 1023, 0, 255);                            // map overal light reading to 0-255 light output

  Serial.print(setLevel);                                               // setting light level
}

// reset user settings
void clearData()
{
  for (int i = 0; i < numNodes; i++)
  {
    lastLightMeasurements[i] = 0;
  }

  preferredLightLevel = 0;
}

void updateLCD()
{
   clearLCD();
   lcdUserSetting(preferredLightLevel);
   lcdBatLevelUpdate(routBatCheck);
}

// intial system setup: defining user preferences -> for use in future models
void systemSetup()
{
  // ask user to:
  // reduce ambient light present
  // set desired light level (up and down?)
  // place sensors accordingly and power on
        // take light measurement (user preferredLightLevel variable set)
              // fill lastLightMeasurements array and ignore all input until system is ready
  // reintroduce ambient light
  // set system as ready
}

// main loop
void loop()
{
  while(inputCount < numNodes)
  {
    retrieveOutputUpdateData();
  }
  inputCount = 0;

  updateLCD();
 
  updateLights();                       // update lights
}
