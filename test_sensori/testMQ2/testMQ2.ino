#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino UNO")
#define         Pin                     (A5)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-2") //MQ2
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ2CleanAir        (9.83) //RS / R0 = 9.83 ppm 

/*****************************Globals***********************************************/
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
/*****************************Globals***********************************************/

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600);
  MQ2.setRegressionMethod(1);
  MQ2.init();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++){
    MQ2.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  Serial.println(calcR0);
  Serial.println(calcR0/10);
  MQ2.setR0(calcR0/10);
  Serial.println("  done!.");
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}  
}

void loop() {
  String SEP = String(",");
  MQ2.update(); // Update data, the arduino will read the voltage from the analog pin
  //LPG
  MQ2.setA(574.25); MQ2.setB(-2.222);
  float LPG = MQ2.readSensor();
  //H2
  MQ2.setA(987.99); MQ2.setB(-2.162);
  float H2 = MQ2.readSensor();
  //CO
  MQ2.setA(36974); MQ2.setB(-3.109);
  float CO = MQ2.readSensor();
  //Alcol
  MQ2.setA(3616.1); MQ2.setB(-2.675);
  float alcol = MQ2.readSensor();
  //Propano
  MQ2.setA(658.71); MQ2.setB(-2.168);
  float propano = MQ2.readSensor();
  Serial.println(LPG+SEP+H2+SEP+CO+SEP+alcol+SEP+propano);
  delay(2000); //Sampling frequency
}