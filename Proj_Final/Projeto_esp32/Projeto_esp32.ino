#include <WiFi.h>          // Biblioteca para gerenciar conexão WiFi
#include <PubSubClient.h>  // Biblioteca para implementar o protocolo MQTT
#include <ESP32Servo.h>    // Biblioteca para controlar servos com o ESP32

static const int servoPin = 13;  // Define o pino do servo motor
Servo servo1;                    // Cria um objeto Servo

// Credenciais da rede WiFi
const char* ssid = "CLARO_2GCA4C72";
const char* password = "B7CA4C72"; //senha wifi

// Endereço do broker MQTT
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;          // Objeto para gerenciar conexão WiFi
PubSubClient client(espClient); // Objeto para gerenciar conexão MQTT

long lastMsg = 0;              // Armazena o tempo da última mensagem
char msg[50];                  // Buffer para mensagens MQTT
int value = 0;                 // Variável de controle
char Contador = 0;             // Contador
String lastMessage = "";       // Armazena a última mensagem recebida

// Pino do LED
const int ledPin = 2;

void setup() {
  Serial.begin(115200);        // Inicializa comunicação serial a 115200 bps

  setup_wifi();                // Chama a função para configurar WiFi
  client.setServer(mqtt_server, 1883); // Configura o servidor MQTT
  client.setCallback(callback); // Define a função de callback para mensagens MQTT

  pinMode(ledPin, OUTPUT);      // Define o pino do LED como saída
  servo1.attach(servoPin);      // Anexa o servo ao pino definido
  pinMode(26, OUTPUT);          // Define o pino do buzzer como saída
  pinMode(25, INPUT_PULLUP);    // Define o pino do botão como entrada com pull-up
}

void setup_wifi() {
  delay(10);                    // Pequena pausa para garantir a inicialização
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);   // Inicia a conexão WiFi

  // Aguarda a conexão WiFi ser estabelecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Exibe o endereço IP obtido
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  // Converte a mensagem recebida em uma string
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Verifica o tópico e ajusta a saída conforme a mensagem recebida
  if (String(topic) == "poli/embarca") {
    lastMessage = messageTemp; // Armazena a última mensagem recebida
    Serial.print("Changing output to ");
    if (messageTemp == "on") {
      Serial.println("on");
      digitalWrite(ledPin, HIGH); // Acende o LED
    }
    else if (messageTemp == "off") {
      Serial.println("off");
      digitalWrite(ledPin, LOW); // Apaga o LED
    }
  }
}

void reconnect() {
  // Loop até que a conexão seja restabelecida
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("mqtt-explorer-33988a83")) { // Tenta se conectar ao broker MQTT
      Serial.println("connected");
      client.subscribe("poli/embarca"); // Inscreve-se no tópico
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Espera 5 segundos antes de tentar novamente
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect(); // Reconecta ao broker MQTT se necessário
  }
  client.loop(); // Mantém a conexão MQTT

  long now = millis();
  if (now - lastMsg > 5000) { // Verifica se 5 segundos se passaram
    lastMsg = now;

    if (digitalRead(25) == LOW || lastMessage == "matheus") { // Verifica se o botão foi pressionado ou se a última mensagem foi "matheus"
      servo1.write(90);          // Move o servo para 90 graus
      digitalWrite(26, HIGH);    // Aciona o buzzer
      delay(3000);               // Aguarda 3 segundos
      servo1.write(0);           // Retorna o servo para 0 graus
      digitalWrite(26, LOW);     // Desliga o buzzer
      delay(3000);               // Aguarda 3 segundos
    }
  }
}
