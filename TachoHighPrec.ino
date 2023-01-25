/*
  High-precision tachometer code for Arduino Nano RP2040. 
  It measures rotation speed from 30RPM to 30000RPM. 
  We count time-lapse for 'nPulses' pulses, then average the 
  period for each revolution to obtain a high precision RPM. 
  Ver 2023.0125.1141
  By Hu Yu. 
*/

struct RPMPayload
{
  uint16_t RPM;
  byte crc;
};

const byte INT_PIN=7;  //We are using pin D7 (GPIO19)
volatile float pulseDetected = 0.0; //Pulse counter
const float TicksPerSec=1000000.0;
const float nPulses=10.0;
float timeout = TicksPerSec * 2.0;//Time out is 2s, which means that rotation speed slower than 30RPM will  not be measured. 
float denominator = nPulses * TicksPerSec;
RPMPayload rpmPayload;
byte rpmPayloadLen=sizeof(RPMPayload);

#define _DEBUG_

void setup()
{
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), interruptFunction, RISING);
  Serial.begin(115200);
}

void loop()
{
  memset(&rpmPayload,rpmPayloadLen,0);
  rpmPayload.RPM=MeasureRPM();
  rpmPayload.crc=CRC8((byte *)&rpmPayload.RPM, sizeof(float));
  Serial.write((byte *)&rpmPayload, rpmPayloadLen);
  #ifdef _DEBUG_
    Serial.print("\n [RPM]=");
    Serial.print(rpmPayload.RPM);
    Serial.print("\t [crc]=");
    Serial.print(rpmPayload.crc);
    Serial.print("\t [rpmPayloadLen]=");
    Serial.println(rpmPayloadLen);
  #endif
  delay(100);
}

void interruptFunction()
{
  pulseDetected=pulseDetected+1.0;
}

//----------------------------------------------
uint16_t MeasureRPM()
{
  float frequency = 0.0;
  uint16_t RPM=0;
  float dT=0;
  unsigned long previousMillis=0;
  unsigned long currentMillis=0;
  pulseDetected=0;
  previousMillis = micros();
  while(1)
  {
    currentMillis = micros();
    dT=currentMillis - previousMillis;
    if(dT>=timeout) break;
    if(pulseDetected < nPulses) continue;//Meature the time eslapsed for nPulses, then compute averaged period for each revolution. 
    currentMillis = micros();
    dT=(currentMillis - previousMillis)/denominator;
    break;
  }//等待脉冲；
  if(dT>=timeout||dT<=0.002) return 0;//Filter signals that are too fast or too slow. (30000>RPM>30)
  frequency = 1.0 / dT ;
  RPM= frequency * 60.0;
  //Debug logs: 
  #ifdef _DEBUG_
    Serial.println("----------------------");
    Serial.print("[Mes] nPulses=");
    Serial.print(nPulses);
    Serial.print("\t pulseDetected=");
    Serial.print(pulseDetected);
    Serial.print("\t dT=");
    Serial.print(dT);
    Serial.print("\t frequency=");
    Serial.println(frequency);
  #endif
  return RPM;
}

char CRC8(byte * data,int length) 
{
   char crc = 0x00;
   char extract;
   char sum;
   for(int i=0;i<length;i++)
   {
      extract = *data;
      for (char tempI = 8; tempI; tempI--) 
      {
         sum = (crc ^ extract) & 0x01;
         crc >>= 1;
         if (sum)
            crc ^= 0x8C;
         extract >>= 1;
      }
      data++;
   }
   return crc;
}