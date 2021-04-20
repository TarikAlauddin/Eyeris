#define LED 11
#define LED1 10
#define LED2 9
int value = 0;
int lightVal1, lightVal2, lightVal3;

void setup() {
 // initialize serial communication:
 Serial.begin(9600);
 pinMode(LED, OUTPUT);
 pinMode(LED1, OUTPUT);
 pinMode(LED2, OUTPUT);
}
 
void loop() {
 if (Serial.available()) {
 // Serial.println(Serial.read());
      for (int i = 0; i < 4; i++)                                        // gathers address, skipping to lighting data
      {
        if (i == 0)      {lightVal1 = Serial.read();}
        else if (i == 1) {lightVal2 = Serial.read();} 
        else if (i == 2) {lightVal3 = Serial.read();}
      }
       value = (lightVal1 * 100) + (lightVal2 * 10) + lightVal3; 
     Serial.println(value);
     //delay(1000);
     analogWrite(LED, value);
     analogWrite(LED1, value);
     analogWrite(LED2, value);
  }
}
   
   
