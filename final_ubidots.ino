#include <Wire.h>
#include <Adafruit_MLX90614.h>//libreria sensor de temperatura
#include <LiquidCrystal_I2C.h>//libreria del LCD
#include<Servo.h>//libreria servomotor
#include <UbidotsEthernet.h>//libreria servidor Ubidots
#include <Ethernet.h>//libreria shield ethernet
#include <SPI.h>
int Trig = 6;//pin Trig sensor de distancia
int Echo = 7;//pin echo sensor de distancia
long valor;//valor que toma el sensor de distancia
int pin8 = 8;//pin del buzzer

LiquidCrystal_I2C lcd(0x27,16,2);//declaracion del LCD 
char const * VARIABLE_LABEL_1 = "temperatura_mlx"; // Etiqueta de variable única para enviar los datos

int temperatura_mlx = 0;

char const * TOKEN = "BBFF-6JlLWqjpOEFlBLyzOjSTZgtAUAImaU"; // Token asignado por Ubidots
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//direccion MAC del shield de ethernet

Adafruit_MLX90614 mlx = Adafruit_MLX90614();//declaracion del sensor de temperatura
Servo motor1;//delaracion del servo

Ubidots client(TOKEN);//se declara la variable TOKEN

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciamos test"); 
  valor=30;//declaracion de la variable valor 

  pinMode(pin8, OUTPUT);//declaracion del buzzer
  motor1.attach(9);//declaracion del servomotor
   Serial.begin(9600);//comunicacion con el monitor serial a 9600 baudios
  mlx.begin();//inicializacion del senseor de temperatura
  
  Serial.print(F("Inicializando conexión ethernet..."));
  //mensaje que se imprimira en el monitor a la hora de establecer una conexion con la placa shield ethernet
  if (!Ethernet.begin(mac)) {//if que indica la conexion exitosa con la placa shield
    Serial.println(F("ops hubo un problema"));
    //en caso que no se haya podido establecer una conexion se imprimira este mensaje
  } else {
    delay(2000);
  Serial.println(F("PLaca ethernet lista!!!"));
  //en caso que si se haya podido establecer una conexion se imprimira este mensaje
  Serial.println(Ethernet.localIP());//se mostrará la direccion IP que esta utilizando la placa shield
  }
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
  
  if(distancia >1000 && valor > 30 ) {
    //el proyecto se encedera al cumplir estos requisitos
mlx.begin();//se inicializa el sensor de temperatura
 lcd.init();//se inicializa la pantalla lcd                     
 lcd.init();//se inicializa la pantalla lcd    

lcd.backlight();//se enciende la pantalla lcd
lcd.setCursor(3,0);//se citua el la linia 3, 0 el mensaje
lcd.print("Distancia: ");//mensaje que se va a imprimir en la pantalla
lcd.print(distancia);//valor que se va a tomar para imprimir en la pantalla

lcd.setCursor(2,1);//se citua el la linia 2, 1 el mensaje
lcd.print("Temperatura: ");//mensaje que se va a imprimir en la pantalla
valor=mlx.readObjectTempC()+5;
//al valor de la temperatura se le sumará 5 grados al no ser tan exacto el sensor
lcd.print(valor);//se imprimira en el lcd el valor de la temperatura

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  //se imprime en el monitor el valor "ambiente" en grados celsius
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC());
  Serial.println("*C");//se imprime en el monitor el valor "objeto" en grados celsius
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  //se imprime en el monitor el valor "ambiente" en grados farenheit
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); 
  Serial.println("*F");
  //se imprime en el monitor el valor "objeto" en grados farenheit

if (valor > 37 ) {
  //si el valor es mayor a los 37 grados se hará la siguiente sentencia
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
valor=mlx.readObjectTempC();
//el valor que se tomo con anterioridad equivale a mlx.readObjectTempC

mlx.begin();//inicializacion del sensor de temperatura para el envio de datos  
Ethernet.maintain();//se recibe la direccion IP con la libreria del shield ethernet
temperatura_mlx=mlx.readObjectTempC();
//la variable temperatura_mlx equivale a mlx.readObjectTempC a la hora del envio de datos
Serial.print(temperatura_mlx);//se imprime en el monitor la variable temperatura_mlx
Serial.println("*C");// Leemos la temperatura en grados celsius

//se envia al servidor las variables para poder almacenarlas
  client.add(VARIABLE_LABEL_1, temperatura_mlx);
  client.sendAll();
  
  delay(5000);
}
}
