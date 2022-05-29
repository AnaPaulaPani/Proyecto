#include <Wire.h>
#include <Adafruit_MLX90614.h>//libreria sensor de temperatura
#include <LiquidCrystal_I2C.h>//libreria del LCD
#include<Servo.h>//libreria servomotor
int Trig = 6;//pin Trig sensor de distancia
int Echo = 7;//pin echo sensor de distancia
long valor;//valor que toma el sensor de distancia
int pin8 = 8;//pin del buzzer

LiquidCrystal_I2C lcd(0x27,16,2);//declaracion del LCD 
Servo motor1;//delaracion del servo
Adafruit_MLX90614 mlx = Adafruit_MLX90614();//declaracion del sensor de temperatura

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciamos test"); 
  valor=30;//declaracion de la variable valor 
  pinMode(pin8, OUTPUT);//declaracion del buzzer
  motor1.attach(9);//declaracion del servomotor
 } 
void loop() {

//se inicializan los dos pines del sensor de distancia
pinMode (Trig, OUTPUT);
pinMode (Echo, INPUT);

long duracion;//la variable duracion se lee de tipo long
long distancia; //la variable distancia se lee de tipo long

//limpiamos de ruido el pin que realizo el disparo del ultrasonido y lo lanzamos
digitalWrite (Trig, LOW);
delayMicroseconds (2);
digitalWrite (Trig, HIGH);
delayMicroseconds (10);
digitalWrite (Trig, LOW);
//recibimos el ultrasonido de vuelta
 duracion = pulseIn (Echo, HIGH);
 
 duracion = duracion/2;//el valor duracion se lo divide entre 2
 distancia = duracion/29;//el valor distacia equivale al valor duracion entre 29
 
Serial.println(distancia);//se imprime en el monitor la variable distancia
  
  if(distancia >1000 && valor > 30 ) {//el proyecto se encedera al cumplir estos requisitos
mlx.begin();//se inicializa el sensor de temperatura
 lcd.init();//se inicializa la pantalla lcd                     
 lcd.init();//se inicializa la pantalla lcd    

lcd.backlight();//se enciende la pantalla lcd
lcd.setCursor(3,0);//se citua el la linia 3, 0 el mensaje
lcd.print("Distancia: ");//mensaje que se va a imprimir en la pantalla
lcd.print(distancia);//valor que se va a tomar para imprimir en la pantalla

lcd.setCursor(2,1);//se citua el la linia 2, 1 el mensaje
lcd.print("Temperatura: ");//mensaje que se va a imprimir en la pantalla
valor=mlx.readObjectTempC()+5;//al valor de la temperatura se le sumará 5 grados al no ser tan exacto el sensor
lcd.print(valor);//se imprimira en el lcd el valor de la temperatura

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); //se imprime en el monitor el valor "ambiente" en grados celsius
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");//se imprime en el monitor el valor "objeto" en grados celsius
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); //se imprime en el monitor el valor "ambiente" en grados farenheit
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");//se imprime en el monitor el valor "objeto" en grados farenheit
  
if (valor > 37 ) {//si el valor es mayor a los 37 grados se hará la siguiente sentencia
digitalWrite(pin8, HIGH);//buzzer empieza a sonar
delay(1000);//tiempo que va a sonar el buzzer 
digitalWrite(pin8,LOW);//buzzer deja de sonar
delay(1000);
}
else{ //else anidado
  motor1.write(80); //los grados que va a girar el primer servo
 delay(1500); //cada cuantos segundos los va a hacer
  motor1.write(35);//los grados que va a girar el primer servo
 delay(600); //cada cuantos segundos los va a hacer
}
  Serial.println();
  delay (2000);
}

else{
mlx.begin();//inicializacion del sensor de temperatura  
lcd.init();//inicializacion del lcd                       
lcd.init();//inicializacion del lcd  
delay (2000);
valor=mlx.readObjectTempC();//el valor que se tomo con anterioridad equivale a mlx.readObjectTempC
}
}
