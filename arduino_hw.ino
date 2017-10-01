#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;

float vazao; //Variável para armazenar o valor em L/min
float media=0; //Variável para tirar a média a cada 1 minuto
int contaPulso; //Variável para a quantidade de pulsos
int i=0; //Variável para contagem
int contpulso;
double litros;
double cont;

String litrosStr;

void setup() {
  
    Serial.begin(9600);
    delay(10);

    pinMode(13, INPUT);
    attachInterrupt(13, incpulso, RISING); //Configura o pino 2(Interrupção 0) para trabalhar como interrupção

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("******", "********");

    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}


void loop() {

  contaPulso = 0;   //Zera a variável para contar os giros por segundos
  sei();      //Habilita interrupção
  delay (1000); //Aguarda 1 segundo
  cli();      //Desabilita interrupção
  cont = contpulso;
  litros = (cont)/330;
  vazao = contaPulso / 5.5; //Converte para L/min
  media = media+vazao; //Soma a vazão para o calculo da media
  i++;

  if(i==2){

if(WiFiMulti.run() == WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;                     //Declare object of class HTTPClient

   StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
   JsonObject& JSONencoder = JSONbuffer.createObject(); 

    litrosStr = litros;
 
   JSONencoder["liter"] = litrosStr;
   JSONencoder["month"] = "9";
   JSONencoder["year"] = "2017";
   JSONencoder["serial"] = WiFi.macAddress();

    
   http.begin("https://watermanager.herokuapp.com/v1/consumption", "08:3B:71:72:02:43:6E:CA:ED:42:86:93:BA:7E:DF:81:C4:BC:62:30");      //Specify request destination
   http.addHeader("Content-Type", "application/json");  //Specify content-type header

   char JSONmessageBuffer[300];
   JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
   Serial.println(JSONmessageBuffer);
 
   //int httpCode = http.POST(JSONmessageBuffer);         //Send the request
   
   String payload = http.getString();                  //Get the response payload
 
   //Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection 
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }

  i = 0;
  contaPulso = 0;
  contpulso = 0;
  }
}

void incpulso ()
{
  contaPulso++; //Incrementa a variável de contagem dos pulsos
  contpulso++;
}
