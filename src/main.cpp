#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const int umidadeAnalogico = A0;
const int lampada = D8;

int valorSolo;

//SSID of your network
char ssid[] = "elenao"; //SSID of your Wi-Fi router
char pass[] = "ramona17"; //Password of your Wi-Fi router
WiFiServer server(80);

void setup()
{
  pinMode(lampada, OUTPUT);
  pinMode(umidadeAnalogico, INPUT);
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  WiFiServer server(80);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  server.begin();
  Serial.println("Servidor inicializado");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop ()
{
  valorSolo = analogRead(umidadeAnalogico);
   // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.1.103:9060/data/dataAtual");
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
      DynamicJsonDocument doc(capacity);
      deserializeJson(doc, payload);
      int status = doc["status"]; // 200
      int hora = doc["hora"];
      int minutos = doc["minutos"];
      Serial.println("Hora");
      Serial.println(hora);
      Serial.println("Minuto");
      Serial.println(minutos);
      if(hora >= 7 && hora <= 19) {
        digitalWrite(lampada, HIGH);
      } else {
        digitalWrite(lampada, LOW);
      }
      Serial.println("Umidade");
      Serial.println(valorSolo);
    }
    http.end();
  }
  delay(3000);
}