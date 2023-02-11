/**
 * Codice per il funzionamento dei sensori della sonda Infinity1
 * @author Infinity1 Team - Liceo E. Medi Montegiorgio (FM)
 * @date 2022-11-01 Last review
 */
#include "Arduino_SensorKit.h"
#include "Seeed_BMP280.h"
#include <SPI.h>
#include <SD.h>
#include <DFRobot_DHT20.h>
#include <MQ135.h>
#include <MQ131.h>

//sensore umidità
DFRobot_DHT20 dht20;
//sensore MQ135
MQ135 mq135_sensor(A2);
//file di log
File logFile; 
//sensore barometro
BMP280 barometro;
//pressione al livello del mare
const float PRES_LIVELLO_MARE = 102350.00;
//temperatura rilevata
float t;
//pressione rilevata
float p;
//altitudine rilevata
float a;
//umidità
float u;
//nox
float nox;
//cloro
float cl2;
//ozono
float o3;
//anidride carbonica
float co2;
float co2_b;
float rzero;
//conteggio secondi
unsigned int sec=0;
//separatore valori nel file di log
String SEP = String(",");

//calcolo altitudine
float calcAltitude(float p, float t) {
  float C = (PRES_LIVELLO_MARE / p);
  C = pow(C, (1 / 5.25588)) - 1.0;
  C = (C * (t + 273.15)) / 0.0065;
  return C;
}

//reboot arduino
void(* resetFunc) (void) = 0;
void reboot(){
    Oled.println("reboot");
    delay(1000);
    resetFunc();
}

//creazione file di log
bool crea_file(){
  unsigned int i=1;
  String filename = String("log_")+i+String(".txt");
  logFile = SD.open(filename,FILE_WRITE); 
  if(logFile) return true;
  else return false;
}

//avvio
void setup(){
  Wire.setClock(100000);
  //avvio schermo Oled
  Oled.begin(); 
  Oled.setFlipMode(true);
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0,0);
  Oled.println("Avvio ...");
  //avvio lettore SD
  pinMode(9, OUTPUT);
  if(!SD.begin(9)){
    Oled.println("Scheda SD NO");
    reboot(); //se la scheda non è leggibile forzo il riavvio di Arduino
  }
  Oled.println("Scheda SD OK");
  if(crea_file()){
    Oled.println("File log OK");
    Oled.println(logFile.name());
  }
  else{
    Oled.println("File log NO");
    reboot(); //se la scheda non è leggibile forzo il riavvio di Arduino
  }
  //avvio barometro
  barometro = BMP280();
  barometro.init();
  Oled.println("Barometro OK");
  //avvio sensore umidità
  dht20.begin();
  Oled.println("Umidita' OK");
  //avvio sensore MQ131
  MQ131.begin(2,A1, LOW_CONCENTRATION,100000); 
  Oled.println("MQ131 OK");
  //MQ135 già avviato
  Oled.println("MQ135 OK");
  delay(5000);
  Oled.clearDisplay();
}

//misurazioni
void loop() {
  //lettura valori
  t=barometro.getTemperature();
  p=barometro.getPressure();
  a=calcAltitude(p,t);
  u=dht20.getHumidity()*100;
  MQ131.setEnv(t,u);
  o3 = MQ131.getO3(PPM);

  float rzero = mq135_sensor.getRZero();
  rzero = mq135_sensor.getCorrectedRZero(t,u);
  co2_raw = mq135_sensor.getPPM();
  co2 = mq135_sensor.getCorrectedPPM(t,u);

  //scrittura valori nel file di log
  logFile.println(sec);
  logFile.println(t+SEP+p);
  logFile.println(a+SEP+u);
  logFile.println(o3);
  logFile.println(co2+SEP+co2_raw);
  //superati i 500m non aggiorno il display
  if(a<500){
    Oled.refreshDisplay();
    Oled.setCursor(0,0);
    Oled.println(String("time: ")+sec);
    Oled.println(String("T:")+t+String("C"));
    Oled.println(String("P:")+p+String("p"));
    Oled.println(String("A:")+a+String("m"));
    Oled.println(String("U:")+u+String("%"));
    Oled.println(String("O3:")+o3+String("ppm"));
    Oled.println(String("CO2:")+co2+String("ppm"));
  }
  else{
    Oled.clearDisplay();
  } 
  delay(2000);
  sec+=2;
  if(sec%20==0) logFile.flush(); //ogni 20 secondi confermo la scrittura dei dati all'interno della scheda SD
}