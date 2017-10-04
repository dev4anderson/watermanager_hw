#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;

float vazao;      //Variável para armazenar o valor em L/min
int i = 0;        //Variável para contagem
int contpulso;
double litros;
double cont;

String litrosStr;

void setup() {
  
  configPorta();

  conectaWifi();

}


void loop() {
  
  calcLitros();

  if(i==60){

    enviaDados();

    limpaLitros();
    
  }
  
}

void incpulso ()
{
  contpulso++;
}

void calcLitros ()
{
    sei();      //Habilita interrupção
    delay (1000); //Aguarda 1 segundo
    cli();      //Desabilita interrupção
    cont = contpulso;
    litros = (cont)/330;
    i++;
}

void enviaDados ()
{
  if(WiFiMulti.run() == WL_CONNECTED){   //Check WiFi connection status
 
     HTTPClient http;                     //Declare object of class HTTPClient

     StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
     JsonObject& JSONencoder = JSONbuffer.createObject(); 

     litrosStr = litros;
 
     JSONencoder["liter"] = litrosStr;
     JSONencoder["serial"] = WiFi.macAddress();

    
     http.begin("https://watermanager.herokuapp.com/v1/consumption", "08:3B:71:72:02:43:6E:CA:ED:42:86:93:BA:7E:DF:81:C4:BC:62:30");      //Specify request destination
     http.addHeader("Content-Type", "application/json");  //Specify content-type header

     char JSONmessageBuffer[300];
     JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
     Serial.println(JSONmessageBuffer);
 
     int httpCode = http.POST(JSONmessageBuffer);         //Send the request
   
     String payload = http.getString();                  //Get the response payload
 
     Serial.println(httpCode);   //Print HTTP return code
     Serial.println(payload);    //Print request response payload
 
     http.end();  //Close connection 
 
   }else{
 
      Serial.println("Error in WiFi connection");   
  }

}

void limpaLitros ()
{
    i = 0;
    contpulso = 0;  
}

void conectaWifi ()
{
  // Conexão com rede Wifi
    WiFiMulti.addAP("AndroidA5", "iotfiap123");

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
void configPorta ()
{
    Serial.begin(9600);
    delay(10);

    pinMode(13, INPUT);
    attachInterrupt(13, incpulso, RISING); //Configura o pino 2(Interrupção 0) para trabalhar como interrupção
}



