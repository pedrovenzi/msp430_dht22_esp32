
#include <stdio.h>
#include <string.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID = "WIFIID";
const char* PASS = "WIFIPASSWORD";
const char* mqtt_server = "broker.hivemq.com";  // broker.hivemq.com
long lastTemp = 0;

// UART
const int bufferSize = 5;
char receivedString[bufferSize];

//WIFI
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);                       // ESP 32 -> ARDUINO
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // MSP430 <--> ESP32
                                            // pino 16 (RX) e 17(TX) do ESP e pino P3.3(TX) e P3.4(RX) do MSP430
  delay(100);
  // Configuracao dos pinos:
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectado a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...\n\r");

    if (client.connect("esp32Client")) {
      Serial.println("Conectado");
      client.publish("clima/temperatura", "Idle");
      client.publish("clima/umidade", "idle");
    } else {
      Serial.print("Erro:");
      Serial.print(client.state());
      Serial.println(" reconectando em 5 segundos");

      delay(5000);
    }
  }
}

// ------------------------- MQTT --------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Menssagem recebida[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '0') {
    Serial.println("LOW");
  }
  if ((char)payload[0] == '1') {
    Serial.println("HIGH");
  }
}

void loop() {
  int bytesRead;
  int umidade;
  int temperatura;
  float f_umidade;
  float f_temperatura;

    // MSP430 --> ESP32: Recebimento de dados pela comunicacao UART, pino 16 (RX) e 17(TX) do ESP e pino P3.3(TX) e P3.4(RX) do MSP430
  if (Serial2.available() > 0) {
      
      bytesRead = Serial2.readBytesUntil('\n', receivedString, bufferSize - 1);
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  long now = millis();

  if (now - lastTemp > 1000) {      // Publicar periodicamente os valores de temperatura e umidade
    lastTemp = now;

    if (bytesRead == 4) {
      Serial.print("Bytes recebidos: ");
      Serial.println(bytesRead);

      char s_umidade[8];
      char s_temperatura[8];

      umidade = ((int)receivedString[1] << 8) | (int)receivedString[0];
      f_umidade = ((float) umidade) / 10;
      Serial.print("UMIDADE CALC: ");
      Serial.println(f_umidade);

      temperatura = ((int)receivedString[3] << 8) | (int)receivedString[2];
      f_temperatura = ((float) temperatura) / 10;
      Serial.print("TEMPERATURA CALC: ");
      Serial.println(f_temperatura);

      dtostrf(f_umidade, 1, 2, s_umidade);
      dtostrf(f_temperatura, 1, 2, s_temperatura);
      client.publish("clima/umidade", s_umidade);
      client.publish("clima/temperatura", s_temperatura);
    }
  }
}


