/*
Autor : Leonardo Moraes
Programa : BackEnd do servidor 
projeto : Hidroponia com IOT
Escopo : ler sersor umidade, temperatura, condutividade, adicionar data e hora das leituras

*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <Ticker.h>
#include <DHT.h>

#define ssid "X-one"
#define senha "ston3Breaker"
#define AUTH_FIREBASE "v4WdSvCfPnTZhgeo4xWawQo3H7r5CbyHzbaksxgd"
#define HOST_FIREBASE "iot1-26a98.firebaseio.com"
#define tempo_para_pub 5000*60

Ticker ticker;
WiFiUDP udp;
NTPClient npt(udp, "a.st1.ntp.br" , -2 * 3600,  60000);
DHT dht(D5,DHT11);

bool permitir_pub= true;

void publica();
void conecta_wifi();
void conecta_firebaseDB();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  npt.begin();
  dht.begin();
  

  conecta_firebaseDB();
  conecta_wifi();
  ticker.attach_ms(tempo_para_pub, publica);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = dht.readTemperature();
  float u = dht.readHumidity();
  
  Serial.println(t);
  Serial.println(u);
  Serial.println(npt.getFormattedTime());
  delay(1000);
  
  if(permitir_pub){
     if(!isnan(t) | !isnan(u)){
        Firebase.pushFloat("Temperatura", t);
        Firebase.pushFloat("Umidade", u);
        Firebase.pushFloat("Condutividade", 0.0);
        Firebase.pushString("hora", npt.getFormattedTime());

        permitir_pub = false;
     }
     else{
        Serial.println("Erro na leitura do sensor");
     }
  }
}

void conecta_wifi(){
  
      Serial.println("iniciando a conexao");
      WiFi.begin("X-one", "ston3breaker");

      while(WiFi.status() != WL_CONNECTED){
          Serial.print(".");
          delay(500);  
      }

      Serial.print("conexão ao WiFi efetuada");

}

void conecta_firebaseDB(){
      Firebase.begin(HOST_FIREBASE, AUTH_FIREBASE);
      
}

void publica(){
    permitir_pub = true;

}
