#include <Adafruit_MLX90614.h>//libreria sensor de temperatura
#include <UbidotsEthernet.h>//libreria servidor Ubidots
#include <Ethernet.h>//libreria shield ethernet
#include <SPI.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
// Pin digital al caual vamos a conectar el sensor de temperatura
 

// Parámetros para el servidor de Ubidots.
char const * TOKEN = "BBFF-6JlLWqjpOEFlBLyzOjSTZgtAUAImaU"; // Token asignado por Ubidots
char const * VARIABLE_LABEL_1 = "temperatura_mlx";
// Asigna una etiqueta de variable única para enviar los datos

int temperatura_mlx = 0;//se inicia el valor del sensor en 0 antes de recibir los datos
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//direccion MAC del shield de ethernet
 
Ubidots client(TOKEN);//se declara la variable TOKEN

void setup() {
  Serial.begin(9600);
    mlx.begin();//Inicializa el sensor de temperatura
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

mlx.begin();//inicializacion del sensor de temperatura para el envio de datos  
Ethernet.maintain();//se recibe la direccion IP con la libreria del shield ethernet
temperatura_mlx=mlx.readObjectTempC();
//la variable temperatura_mlx equivale a mlx.readObjectTempC a la hora del envio de datos
Serial.print(temperatura_mlx);//se imprime en el monitor la variable temperatura_mlx
Serial.println("*C");// Leemos la temperatura en grados celsius

//se envia al servidor las variables para poder almacenarlas
  client.add(VARIABLE_LABEL_1, temperatura_mlx);
  client.sendAll();
 
  delay(5000);//Esperemos 5 segunsdo antes de volver a subir otro datos a Ubidots 
}
