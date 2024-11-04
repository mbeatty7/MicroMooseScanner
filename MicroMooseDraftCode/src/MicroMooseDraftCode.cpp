/* 
 * Project Micro Moose Draft Code
 * Author: Mamie-Jo Beatty
 * Date: 11/3/24
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */


#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_BME280.h"
#include "neopixel.h"
#include "Keypad_Particle.h"
#include "Grove_Air_Quality_Sensor.h"

float temp;

const byte ROWS = 4;
const byte COLS = 4;
const int OLED_RESET = -1;
// const int joystickx_pin = A0;
// const int joysticky_pin = A1;
const int hexAddress = 0x76;
AirQualitySensor sensor(A2);


const char degree = 248;

char customKey;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {D8, D9, D16, D15};
byte colPins[COLS] = {D17, D18, D19, D14};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int PIXELCOUNT = 1;
int n;
int status;

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme;

SYSTEM_MODE(AUTOMATIC);


SYSTEM_THREAD(ENABLED);


SerialLogHandler logHandler(LOG_LEVEL_INFO);


void setup() {
 Serial.begin(9600);
 waitFor(Serial.isConnected, 10000);

// neoPixel
 pixel.begin();
 pixel.setBrightness(255);
 pixel.show();
 

// OLED
display.begin();
display.display();
delay(1000);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(1,0);
display.setCursor(0,0);
display.printf("I am working :) ... \n");
display.display();

// BME280
display.begin(SSD1306_SWITCHCAPVCC,0x3C);
display.display();
status = bme.begin(hexAddress);
if(status == false){
  Serial.printf("BME280 at address 0x%02x failed to start \n", hexAddress);
}

}


void loop() {

  // neoPixel
  pixel.setPixelColor(n, 0x0000FF);
 pixel.show();
//  pixel.clear();
//  pixel.show();

 // Keypad

 customKey = customKeypad.getKey();

 if(customKey){
  Serial.printf("Key Pressed: %c\n", customKey);
  Serial.printf("Key Pressed (Hex Code) 0x%02X\n", customKey);
 }

 // BME
temp = bme.readTemperature();

Serial.printf("temp = %0.1f \n", temp);

display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1,0);
  display.setCursor(0,0);
  display.printf("temp = %0.1f \n", temp);
  display.display();


// air quality sensor
int quality = sensor.slope();

    Serial.print("Sensor value: ");
    Serial.println(sensor.getValue());

    if(quality == AirQualitySensor::FORCE_SIGNAL) {
        Serial.printf("EMISSIONS LARGE LEAK. SERVICE IMMEDIATELY. \n");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(1,0);
        display.setCursor(0,0);
        display.printf("EMISSIONS LARGE LEAK. SERVICE IMMEDIATELY. \n");
        display.display();
    } 
    if(quality == AirQualitySensor::HIGH_POLLUTION) {
        Serial.printf("CAUTION: EMISSIONS SMALL LEAK LARGE LEAK. \n");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(1,0);
        display.setCursor(0,0);
        display.printf("CAUTION: EMISSIONS SMALL LEAK LARGE LEAK. \n");
        display.display();
    } 
    if(quality == AirQualitySensor::LOW_POLLUTION) {
        Serial.printf("CAUTION: EMISSIONS SMALL LEAK. \n");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(1,0);
        display.setCursor(0,0);
        display.printf("CAUTION: EMISSIONS SMALL LEAK. \n");
        display.display();
    } 
    if(quality == AirQualitySensor :: FRESH_AIR) {
      Serial.printf("EMISSIONS WITHIN RANGE. \n"); 
      display.clearDisplay();
    }

    delay(1000);


}
