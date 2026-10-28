#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_LG.h> // Inclui os controles específicos para LG

// Substitua pelos seus dados de Wi-Fi
const char* ssid = "";    
const char* password = "";    
const int irPin = 4; // Defina o pino GPIO4 para o emissor IR

// Instancias de classes
WiFiClient espClient;
PubSubClient client(espClient);

// Instância do ar-condicionado LG
IRLgAc ac(irPin); 

// Dados do servidor MQTT
const char* mqtt_server = "192.168.0.250"; 
const int mqtt_port = 1883;
const char* topic_publish = "fe6464c65c674037a8fea7aa34150954/app";
const char* topic_subscriber = "fe6464c65c674037a8fea7aa341509541/hard_1";

const int LED_PIN = LED_BUILTIN; 

// Pisca o LED por 100ms 
void piscarLed() {
   digitalWrite(LED_PIN, LOW); // Acende 
   delay(100); 
   digitalWrite(LED_PIN, HIGH); // Apaga 
}

// ==========================================
// FUNÇÕES DE COMANDO COM IRremoteESP8266
// ==========================================

void handleValidaPlacaOnline() {
  client.publish(topic_publish, "Placa Conectada !");
}

// Função centralizada para enviar o estado do Ar e publicar no MQTT
void enviaEstadoAc(String mensagem) {
  ac.send(); // Constrói o código com as definições atuais e envia
  client.publish(topic_publish, mensagem.c_str());
}

void handleLigaAr() {
  ac.on();
  enviaEstadoAc("Ar ligado!");
}
void handleDesligaAr() {
  ac.off();
  enviaEstadoAc("Ar desligado!");
}
void handleTemp18() { ac.setTemp(18); enviaEstadoAc("Temperatura 18 graus!"); }
void handleTemp19() { ac.setTemp(19); enviaEstadoAc("Temperatura 19 graus!"); }
void handleTemp20() { ac.setTemp(20); enviaEstadoAc("Temperatura 20 graus!"); }
void handleTemp21() { ac.setTemp(21); enviaEstadoAc("Temperatura 21 graus!"); }
void handleTemp22() { ac.setTemp(22); enviaEstadoAc("Temperatura 22 graus!"); }
void handleTemp23() { ac.setTemp(23); enviaEstadoAc("Temperatura 23 graus!"); }
void handleTemp24() { ac.setTemp(24); enviaEstadoAc("Temperatura 24 graus!"); }
void handleTemp25() { ac.setTemp(25); enviaEstadoAc("Temperatura 25 graus!"); }
void handleTemp26() { ac.setTemp(26); enviaEstadoAc("Temperatura 26 graus!"); }
void handleTemp27() { ac.setTemp(27); enviaEstadoAc("Temperatura 27 graus!"); }

void handleDesligaLed() {
  ac.setLight(false); // Apaga o display do ar condicionado
  enviaEstadoAc("LED do ar desligado!");
}
void handleVelocidade1() {
  ac.setFan(kLgAcFanLow);
  enviaEstadoAc("Velocidade Baixa!");
}
void handleVelocidade2() {
  ac.setFan(kLgAcFanMedium); // CORRIGIDO: kLgAcFanMedium (escrito por extenso)
  enviaEstadoAc("Velocidade Media!");
}
void handleVelocidade3() {
  ac.setFan(kLgAcFanHigh);
  enviaEstadoAc("Velocidade Alta!");
}
void handleVelocidadeAuto() {
  ac.setFan(kLgAcFanAuto);
  enviaEstadoAc("Velocidade Automatica!");
}
void handleModoVentilador() {
  ac.setMode(kLgAcFan);
  enviaEstadoAc("Modo apenas Ventilador!");
}
void handleModoAr() {
  ac.setMode(kLgAcCool);
  enviaEstadoAc("Modo Ar Condicionado (Frio)!");
}

// ==========================================

// Tabela de comandos
struct Command {
  const char* name;
  void (*action)();
};

Command commands[] = {
  {"ValidaPlacaOnline", handleValidaPlacaOnline},
  {"ar-on", handleLigaAr},
  {"ar-off", handleDesligaAr},
  {"temp-18", handleTemp18},
  {"temp-19", handleTemp19},
  {"temp-20", handleTemp20},
  {"temp-21", handleTemp21},
  {"temp-22", handleTemp22},
  {"temp-23", handleTemp23},
  {"temp-24", handleTemp24},
  {"temp-25", handleTemp25},
  {"temp-26", handleTemp26},
  {"temp-27", handleTemp27},
  {"desliga_led", handleDesligaLed},
  {"velocidade_1", handleVelocidade1},
  {"velocidade_2", handleVelocidade2},
  {"velocidade_3", handleVelocidade3},
  {"velocidade_auto", handleVelocidadeAuto},
  {"modo-ventilador", handleModoVentilador},
  {"modo-ar", handleModoAr},
  {nullptr, nullptr} // Finalizador
};

void callback(char* topic, byte* payload, unsigned int length) {
  piscarLed();

  char mensagem[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    mensagem[i] = (char)payload[i];
  }
  mensagem[length] = '\0';

  Serial.println(" Mensagem Recebida: ");
  Serial.println(mensagem);

  for (int i = 0; commands[i].name != nullptr; i++) {
    if (strcmp(mensagem, commands[i].name) == 0) {
      commands[i].action(); 
      return;
    }
  }

  client.publish(topic_publish, "Comando não reconhecido.");
}

void setup_wifi() {
  Serial.begin(115200);
  delay(10);
  Serial.println("\nConectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi. IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESp82661_ArLG")) {
      Serial.println("Conectado!");
      client.subscribe(topic_subscriber);
      client.publish(topic_publish, "PLACA CONECTADA" );
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Apagado por padrão

  setup_wifi();
  
  // Inicia e configura valores iniciais pro "Ar Virtual" do ESP8266
  ac.begin();
  ac.off();                   // Começa desligado
  ac.setFan(kLgAcFanAuto);    // Ventilação em Auto
  ac.setMode(kLgAcCool);      // Modo Frio
  ac.setTemp(22);             // Temperatura padrão 22
  ac.setLight(true);          // Display LED ligado por padrão

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(50); 
}