//Include the library
#include <MQUnifiedsensor.h>

//Definitions
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A5 //Analog input 0 of your arduino
#define type "MQ-131" //MQ131
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ131CleanAir 15 //RS / R0 = 15 ppm
//#define calibration_button 13 //Pin to calibrate your sensor

MQUnifiedsensor MQ131(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  Serial.begin(9600);
  MQ131.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ131.init();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ131.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ131.calibrate(RatioMQ131CleanAir);
    Serial.print(".");
  }
  MQ131.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  MQ131.serialDebug(true);
  Serial.println("Ignore Ratio = RS/R0, for this example we will use readSensorR0Rs, the ratio calculated will be R0/Rs. Thanks :)");
}

void loop() {
  MQ131.update(); // Update data, the arduino will read the voltage from the analog pin
  String SEP = String(",");
  //NOx
  MQ131.setA(-462.43); MQ131.setB(-2.204);
  float nox = MQ131.readSensor();
  float nox2 = MQ131.readSensorR0Rs();
  //CL2
  MQ131.setA(47.209); MQ131.setB(-1.186);
  float cl2 = MQ131.readSensor();
  float cl22 = MQ131.readSensorR0Rs();
  //O3
  MQ131.setA(23.943); MQ131.setB(-1.11);
  float o3 = MQ131.readSensor();
  float o32 = MQ131.readSensorR0Rs();
  Serial.println(nox+SEP+nox2+SEP+cl2+SEP+cl22+SEP+o3+SEP+o32);
  delay(2000);
}