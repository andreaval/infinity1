/**
    Powered by SmartTech Benin
*/
#include <MG811.h>

MG811 mySensor = MG811(A1); // Analog input A0

float v400 = 4.535;
float v40000 = 3.206;

void setup(){
    Serial.begin(9600);
    Serial.println("MG811 CO2 Sensor");
    
    mySensor.begin(v400, v40000);
}


void loop(){
    Serial.print("Raw voltage: ");
    Serial.print(mySensor.raw());
    Serial.print("V, C02 Concetration: ");
    Serial.print(mySensor.read());
    Serial.println(" ppm");
    
    delay(1000); // 1 second
}