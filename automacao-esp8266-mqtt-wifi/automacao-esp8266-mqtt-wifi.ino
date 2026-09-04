
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremote.h>

// Substitua pelos seus dados de Wi-Fi
const char* ssid = "F_Router";    
const char* password = "1292324100";
const int irPin = 4; // Defina o pino GPIO4 para o emissor IR

// Instancias  de classes
WiFiClient espClient;
PubSubClient client(espClient);
IRsend irsend(irPin); // Instância do emissor IR

// Dados do servidor MQTT
const char* mqtt_server = "broker.hivemq.com";  // Usando o servidor MQTT público (troque por seu servidor, se necessário)
const int mqtt_port = 1883;  // Porta padrão MQTT
const char* topic_publish = "fe6464c65c674037a8fea7aa34150954/app";
const char* topic_subscriber = "fe6464c65c674037a8fea7aa34150954/hard";

//CONSTANTES codigos Hexadecimais
const uint32_t liga_ar = 0x880095E;
const uint32_t desliga_ar = 0x88C0051;
const uint32_t temp_18 = 0x8808350;
const uint32_t temp_19 = 0x8808451;
const uint32_t temp_20 = 0x8808552;
const uint32_t temp_21 = 0x8808653;
const uint32_t temp_22 = 0x8808754;
const uint32_t temp_23 = 0x8808855;
const uint32_t temp_24 = 0x8808956;
const uint32_t temp_25 = 0x88C0051;
const uint32_t temp_26 = 0x88C0051;
const uint32_t temp_27 = 0x88C0051;
const uint32_t desliga_led = 0x88C0051;
const uint32_t velocidade_1 = 0x88C0051;
const uint32_t velocidade_2 = 0x88C0051;
const uint32_t velocidade_3 = 0x88C0051;
const uint32_t velocidade_auto = 0x88C0051;
const uint32_t modo_ventilador = 0x88C0051;
const uint32_t modo_ar = 0x88C0051;

// Funções para cada comando
void handleValidaPlacaOnline() {
  client.publish(topic_publish, "Placa Conectada !");
}
void dispatchHandleCommand(uint32_t ir_command, String mensagem ){
  irsend.sendLG(ir_command, 28);
  client.publish(topic_publish, mensagem.c_str());
}

void handleLigaAr() {
  // irsend.sendLG(liga_ar, 28);
  // client.publish(topic_publish, "Ar ligado !");
  dispatchHandleCommand(liga_ar, "Ar ligado !");
}

void handleDesligaAr() {
  // irsend.sendLG(desliga_ar, 28);
  // client.publish(topic_publish, );
  dispatchHandleCommand(desliga_ar, "Ar desligado !");
}

void handleTemp18() {
    dispatchHandleCommand(temp_18, "temperatura 18 graus !");
}
void handleTemp19() {
    dispatchHandleCommand(temp_19, "temperatura 19 graus !");
}
void handleTemp20() {
    dispatchHandleCommand(temp_20, "temperatura 20 graus !");
}
void handleTemp21() {
    dispatchHandleCommand(temp_21, "temperatura 21 graus !");
}
void handleTemp22() {
    dispatchHandleCommand(temp_22, "temperatura 22 graus !");
}
void handleTemp23() {
    dispatchHandleCommand(temp_23, "temperatura 23 graus !");
}
void handleTemp24() {
    dispatchHandleCommand(temp_24, "temperatura 24 graus !");
}
void handleTemp25() {
    dispatchHandleCommand(temp_25, "temperatura 25 graus !");
}
void handleTemp26() {
    dispatchHandleCommand(temp_26, "temperatura 26 graus !");
}
void handleTemp27() {
    dispatchHandleCommand(temp_27, "temperatura 27 graus !");
}
void handleDesligaLed() {
    dispatchHandleCommand(desliga_led, "temperatura 27 graus !");
}
void handleVelocidade1() {
    dispatchHandleCommand(velocidade_1, "Velocidade 1 !");
}
void handleVelocidade2() {
    dispatchHandleCommand(velocidade_2, "Velocidade 2 !");
}
void handleVelocidade3() {
    dispatchHandleCommand(velocidade_3, "Velocidade 3 !");
}
void handleVelocidadeAuto() {
    dispatchHandleCommand(velocidade_auto, "Velocidade Automatica !");
}
void handleModoVentilador() {
    dispatchHandleCommand(modo_ventilador, "Modo ar condicionado !");
}
void handleModoAr() {
    dispatchHandleCommand(modo_ar, "Modo Ventilador!");
}
// Tabela de comandos
struct Command {
  const char* name;       // Nome do comando
  void (*action)();       // Ponteiro para a função que executa o comando
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



// Função de callback para quando uma mensagem MQTT for recebida
// void callback(char* topic, byte* payload, unsigned int length) {

//   String mensagem = "";
//   for (unsigned int i = 0; i < length; i++) {
//     mensagem += (char)payload[i];
//   }

//   if (mensagem.indexOf("ValidaPlacaOnline") >= 0) {
//     client.publish(topic_publish, "Placa Conectada !" );
//   }

//   if (mensagem == "ar-on") {
   
//     irsend.sendLG(liga_ar, 28);
//     client.publish(topic_publish, "Ar ligado !" );
    
//   } else if (mensagem == "ar-off") {
   
//     irsend.sendLG(desliga_ar, 28);
//     client.publish(topic_publish, "Ar desligado !" );
//   }

// }
// void callback(char* topic, byte* payload, unsigned int length) {
//   // Criar um buffer de char para armazenar a mensagem
//   char mensagem[length + 1]; // +1 para o caractere nulo
//   for (unsigned int i = 0; i < length; i++) {
//     mensagem[i] = (char)payload[i];
//   }
//   mensagem[length] = '\0'; // Adiciona o caractere nulo ao final

//   // Verifica se a mensagem contém "ValidaPlacaOnline"
//   if (strstr(mensagem, "ValidaPlacaOnline") == 0) {
//     client.publish(topic_publish, "Placa Conectada !");
//   } else if (strcmp(mensagem, "ar-on") == 0) {
//     irsend.sendLG(liga_ar, 28);
//     client.publish(topic_publish, "Ar ligado !");
//   } else if (strcmp(mensagem, "ar-off") == 0) {
//     irsend.sendLG(desliga_ar, 28);
//     client.publish(topic_publish, "Ar desligado !");
//   }
// }
void callback(char* topic, byte* payload, unsigned int length) {
  // Criar um buffer de char para armazenar a mensagem
  char mensagem[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    mensagem[i] = (char)payload[i];
  }
  mensagem[length] = '\0'; // Adiciona o caractere nulo ao final

  // Procurar comando correspondente
  for (int i = 0; commands[i].name != nullptr; i++) {
    if (strcmp(mensagem, commands[i].name) == 0) {
      commands[i].action(); // Executa a função associada
      return;
    }
  }

  // Comando não encontrado
  client.publish(topic_publish, "Comando não reconhecido.");
}

// Conectar ao Wi-Fi
void setup_wifi() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {

  while (!client.connected()) {

    Serial.println();
    Serial.println("================================");
    Serial.println("       CONEXAO MQTT");
    Serial.println("================================");

    Serial.print("Broker: ");
    Serial.print(mqtt_server);
    Serial.print(":");
    Serial.println(mqtt_port);

    String clientId = "ESP8266-";
    clientId += String(ESP.getChipId(), HEX);

    Serial.print("Client ID: ");
    Serial.println(clientId);

    if (client.connect(clientId.c_str())) {

      Serial.println("MQTT CONECTADO!");

      if (client.subscribe(topic_subscriber)) {
        Serial.println("Subscribe realizado!");
      } else {
        Serial.println("Erro no subscribe!");
      }

      if (client.publish(topic_publish, "PLACA CONECTADA")) {
        Serial.println("Mensagem publicada!");
      } else {
        Serial.println("Erro ao publicar!");
      }

    } else {

      Serial.print("Falha MQTT, rc=");
      Serial.println(client.state());

      delay(5000);
    }
  }
}


void setup() {
  // Configura o Wi-Fi
  setup_wifi();
  
  // Configura o servidor MQTT
  IrSender.begin(irPin); // Inicializa o emissor IR no pino 4
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // Se o cliente não estiver conectado ao servidor MQTT, tenta reconectar
  if (!client.connected()) {
    reconnect();
  }

  // Manter a conexão com o servidor MQTT
  client.loop();

  // // Aguardar um pouco antes de enviar novamente
  delay(3000);
}


