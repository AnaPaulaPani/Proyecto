//sensor de distancia
int ecoPin= 8;  // cabe amarillo
int trigPin= 9; // cable blanco
//led
int ledPin= 5; 

long duracion, distancia; //distancia

void setup(){
  Serial.begin(9600); 
  pinMode(trigPin, OUTPUT);   //envia la señal de ultrasonido
  pinMode(ecoPin, INPUT);     //reciba la señal de ultrasonido
  pinMode(ledPin, OUTPUT);    //prende el LED
}

 void loop(){
 //funcionamento del sensor envia y recibe la señal
 digitalWrite(trigPin, LOW);  delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); delayMicroseconds(5); 
 digitalWrite(trigPin, LOW);
 //fin
 duracion = pulseIn(ecoPin, HIGH); 
  //HIGH Si alto, comienza a contar
  //LOW se detiene el tiempo
  //http://arduino.cc/en/pmwiki.php?n=Reference/PulseIn 
  //retorna el pulso en microsegundos
 
 distancia = microsecondsToCentimeters(duracion);
 //transformo los microsegundos en centimetros
 
 Serial.print ("Tiempo en microsegundos: "); Serial.print(duracion);
 Serial.print("  Distancia estimada: "); Serial.print(distancia);
 Serial.print(" cm");
 Serial.println(" ");  
 delay(1000);
 if(distancia>70){
   digitalWrite(ledPin, HIGH);
   delay(20);}
 if(distancia<70){
   digitalWrite(ledPin, LOW);
   delay(20);}
 }
long microsecondsToCentimeters(long microseconds){
  // La velocidad del sonido a 20º de temperatura es 340 m/s o
  // 29 microsegundos por centrimetro.
  // La señal tiene que ir y volver por lo que la distancia a 
  // la que se encuentra el objeto es la mitad de la recorrida.
  return microseconds / 29 / 2;
}
