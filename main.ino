#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPmDNS.h>

// Set consts
// WiFi Connection set

#define SSID "SSID"
#define PASSWORD "PASSWORD"

// MQTT Connection set 

#define MQTT_ID "farias-ifrjesp32group-ifrjcnit"
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_MILLIS_TOPIC "farias_ifrjesp32group_ifrjcnit_millis"

// Define net client
WiFiClient espClient; 

// Define mqtt client
PubSubClient MQTT(espClient);

char millis_str[10] = "";


void setupWIFI(){
    // Connect with WiFi

    Serial.println();
    Serial.print("Connecting to");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // Loop para checar a conexão

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    // Display connection info

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP Address:");
    Serial.println(WiFi.localIP());
}

void setupMQTT(){
    // Config MQTT Broker connection

    MQTT.setServer(MQTT_BROKER, MQTT_PORT);

    // Conn exec

    while (!MQTT.connected()){
        Serial.print("- MQTT Setup: Tentando se conectar ao Broker MQTT: ");
        Serial.println(MQTT_BROKER);

        if(MQTT.connect(MQTT_ID)){
            Serial.println("- MQTT Setup: Conectado com sucesso");
        } else {
            Serial.println("- MQTT Setup: Falha ao se conectar, tentando novamente em 2s");
            delay(2000);
        }
    }
}

void setup(void){
    // Set baudrate of serial com
    Serial.begin(115200);

    // Call setup wifi
    setupWIFI();
    // Call setup mqtt
    setupMQTT();
}

void loop(void){
    sprintf(millis_str, "%d", millis()); // store printf of data (millis)

    MQTT.publish(MQTT_MILLIS_TOPIC, millis_str); // publish in topic

    setupWIFI();
    setupMQTT();
    delay(2000);
}