//i2c library: https://www.arduino.cc/en/reference/wire
#include <Wire.h> 

//uv sensor libray: https://github.com/adafruit/Adafruit_VEML6075
#include "Adafruit_VEML6075.h"

//borometer library: https://github.com/pilotak/LPS35HW
#include "LPS35HW.h"

//air quality library: https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library
#include "SparkFunCCS811.h"

//temperature and humidity library: https://github.com/adafruit/Adafruit_HTU21DF_Library
#include "Adafruit_HTU21DF.h"

//use i2c scanner to find addresses: https://www.arduino.cc/reference/en/libraries/i2cscanner/ 
//address for air quality sensor
#define CCS811_ADDR 0x5A
//define CCS811 as QA
CCS811 QA(CCS811_ADDR);

//define VEML6075 uv sensor as uv
Adafruit_VEML6075 uv = Adafruit_VEML6075();

//define LPS35HW as baroMeter
LPS35HW baroMeter;

//define HTU21DF humidity and temperature sensor as htu
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

//define variables if needed for calculations
//int tempVal = 0;
//int humVal = 0;
//int uvVal = 0;
//int pressureVal = 0;

//variables for air quality sensor
int co2Val = 0;
int TVOCVal = 0;

void setup() {
  //start serial connection with baud rate 9600
  Serial.begin(9600);

  //wait until serial is available
  while (!Serial) { delay(10); }

  //stop program when no barometer is found
  if (!baroMeter.begin()) {
    Serial.println("Could not find a LPS35HW barometer, check wiring!");
    while (1);
  }

  //stop program when no uv sensor is found
  if (! uv.begin()) {
    Serial.println("Could not find a VEML6075 uv sensor, check wiring!");
    while (1);
  }

  //stop program when no temperature and humidity sensor is found
  if (!htu.begin()) {
    Serial.println("Could not find a HTU21DF humidity and temperature, check wiring!");
    while (1);
  }

  //stop program when no air quality sensor is found
  if (QA.begin() == false){
    Serial.print("Could not find a CCS811 air quality sensor, check wiring!");
    while (1);
  }

  //filter last 9 samples, default off
  baroMeter.setLowPassFilter(LPS35HW::LowPassFilter_ODR9);
  baroMeter.setOutputRate(LPS35HW::OutputRate_1Hz);

}

void loop() {
  //read air quality if sensor is active
  if (QA.dataAvailable()){
    //air quality values are calculated based on the Total Volatile Organic Compound (TVOC)
    //the Co2 equivalent carbon dioxide reading (eCO2) based on the TVOC value
    //this sensor has to be calibrated against a known source!! 
    //measurements are not trustworthy without calibration
    //read more: https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor 
    QA.readAlgorithmResults();
    co2Val = QA.getCO2();
    TVOCVal = QA.getTVOC();
  }

  //use variables if nececeary
//  humVal = htu.readHumidity();
//  tempVal = htu.readTemperature();
//  tempVal = baroMeter.readTemp(); //baroMeter also has built in temperature sensor
//  pressureVal = baroMeter.readPressure();
//  uvVal = uv.readUVI();

  //make json
  Serial.print("[{\"bn\": \"serialSensor\",\"n\": \"temperature\", \"u\": \"°C\", \"v\": ");
  Serial.print(htu.readTemperature());
  Serial.print("}, {\"n\": \"humidity\",\"u\": \"percent\",\"v\": ");
  Serial.print(htu.readHumidity());
  Serial.print("}, {\"n\": \"pressure\",\"u\": \"hPa\",\"v\": ");
  Serial.print(baroMeter.readPressure());
  Serial.print("}, {\"n\": \"CO2\",\"u\": \"ppm\",\"v\": ");
  Serial.print(co2Val);
  Serial.print("}, {\"n\": \"TVOC\",\"u\": \"ppb\", \"v\": ");
  Serial.print(TVOCVal);
  Serial.print("}, {\"n\": \"uvIndex\",\"u\": \"\", \"v\": ");
  Serial.print(uv.readUVI());
  Serial.print("}]");

  //escape character used
  Serial.println("/");

  //read every 5 seconds
  delay(5000);
}