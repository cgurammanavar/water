#define BLYNK_TEMPLATE_ID "TMPLFccSPn2t"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "uEIG9i2TkQjX1s_4HFqwBCbo3_8Xt-oR"



#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SENSORMT  D2
#define SENSORT1  D3
#define VALUE     D7

char auth[] = "uEIG9i2TkQjX1s_4HFqwBCbo3_8Xt-oR";
char ssid[] = "chandu";
char pass[] = "12345678";




long currentMillis = 0;
long previousMillis = 0;
long currentMillis1 = 0;
long previousMillis1 = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
volatile byte pulseCount1;
byte pulse1Sec = 0;
byte pulse1Sec1 = 0;
float flowRate;
float flowRate1;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;
int tcom;

BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(D7, HIGH);
  } 
  else{
    digitalWrite(D7, LOW);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  
}

 void IRAM_ATTR pulseCounter()
 {
    pulseCount++; }
    void IRAM_ATTR pulseCounter1()
 {
    pulseCount1++; }
void setup()
 {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    pinMode(SENSORMT,   INPUT_PULLUP);
    pinMode(SENSORT1,   INPUT_PULLUP);
    pinMode(VALUE,      OUTPUT);
    //digitalWrite(D4,HIGH);
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.status());
    tcom=0;
    pulseCount = 0;
    flowRate = 0.0;
    pulseCount1 = 0;
    flowRate1 = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;
    flowMilliLitres1 = 0;
    totalMilliLitres1 = 0;
    previousMillis1 = 0;
    attachInterrupt(digitalPinToInterrupt(SENSORMT), pulseCounter, FALLING);
    attachInterrupt(digitalPinToInterrupt(SENSORT1), pulseCounter1, FALLING);
    
  }
  
 void loop()
 {
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) 
    {
      pulse1Sec = pulseCount;
      pulseCount = 0;
      flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
      previousMillis = millis();
      flowMilliLitres = (flowRate / 60) * 1000;
      totalMilliLitres += flowMilliLitres;
      currentMillis = millis();
      Blynk.virtualWrite(V2, flowRate);
      Blynk.virtualWrite(V6,0);
      //Blynk.virtualWrite(V4,0);

      //Blynk.virtualWrite(V3, totalMilliLitres / 1000);
    }
    currentMillis1 = millis();
    if (currentMillis1 - previousMillis1 > interval) 
    {
      pulse1Sec1 = pulseCount1;
      pulseCount1 = 0;
      flowRate1 = ((1000.0 / (millis() - previousMillis1)) * pulse1Sec1) / calibrationFactor;
      previousMillis1 = millis();
      flowMilliLitres1 = (flowRate1 / 60) * 1000;
      totalMilliLitres1 += flowMilliLitres1;
      Serial.print("Flow rate: ");
      Serial.print(int(flowRate1));
      Serial.print("L/min");
      Serial.print("\t");
      Serial.print("Output Liquid Quantity: ");
      Serial.print(totalMilliLitres1);
      Serial.print("mL / ");
      Serial.print(totalMilliLitres1 / 1000);
      Serial.println("L");
      Blynk.virtualWrite(V1, flowRate1);
      Blynk.virtualWrite(V5, totalMilliLitres1 / 1000);
      tcom = (totalMilliLitres / 1000) +(totalMilliLitres1 / 1000);
      Blynk.virtualWrite(V3, tcom);
      Blynk.run();
      
    } 
    if (flowRate1==0 & flowRate!=0)
    {
      
      Blynk.logEvent("leak");
      Blynk.virtualWrite(V4, totalMilliLitres / 1000);
      Blynk.virtualWrite(V6,1);
      //Blynk.virtualWrite(V3, tcom);
      delay(10000);
      digitalWrite(D7,HIGH);
    } 
  

    
   }
  
