#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>

#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

volatile unsigned long last_micros;
long debouncing_time = 250;

const byte interruptPin = 2;
volatile byte select = LOW;
volatile byte measuring = HIGH;

# define Circle 0
byte customchar [] = {
 B01110,
 B01010,
 B01110,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
}; 

Adafruit_MLX90614 mix = Adafruit_MLX90614();

LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup() {
 pinMode(interruptPin, INPUT_PULLUP);
 attachInterrupt (digitalPinToInterrupt (interruptPin), SelectTemp, LOW);

 lcd.begin();
 mlx.begin();

lcd.createChar(Circle, customChar);

lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("MLX90614 sensor");
delay (800);
lcd.setCursor(0, 1);
lcd.print("Measuring...");
delay(800);
lcd.clear();
lcd,setCursor(0, 0);
lcd.print("---Celsius---");
}

void loop() {
  if (select == LOW){
    if(measuring == HIGH){
      lcd.setCursor(0, 0);
      lcd.print("Ambiente: "); lcd.print(mlx.readAmbientTempC());
      lcd.write(Circle): lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("object: ");lcd.print(mlx.readAmbientTempC());
      lcd.write(Circle); lcd.print("C");
      delay(250);

    }else if (measuring == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("---Celsius---");
      delay(800);
      lcd.clear();
      measuring = HIGH;
    }
  }else if (select == HIGH) {
    if (measuring == HIGH){
      lcd.setCursor(0, 0);
      lcd.print("Ambiente: "); lcd.print(mlx.readAmbientTempF());
      lcd.write(Circle): lcd.print("F");
      lcd.setCursor(0, 1);
      lcd.print("object: ");lcd.print(mlx.readAmbientTempF());
      lcd.write(Circle); lcd.print("F");
      delay(250);

    }else if (measuring == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("--Fahrenheit--");
      delay(800);
      lcd.clear();
      measuring = HIGH;
    }
  }

}

void SelectTemp(){
  if((long)(micros() - last micros) >= debouncing_time*1000){//
    last_micros = micros();
    select=!select;
    measuring = LOW;
  }
}
