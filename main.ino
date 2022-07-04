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
#define MQTT_LED_TOPIC "fariasys-led-power-manage"

// Set ports

#define LED_R 2
#define LED_Y 18
#define LED_G 5

// Define net client
WiFiClient espClient; 

// Define mqtt client
PubSubClient MQTT(espClient);

char millis_str[10] = "";


void setupWIFI(){
  if(WiFi.status() == WL_CONNECTED){
    return;
  } else {
    // Connect with WiFi

    Serial.println();
    Serial.print("Connecting to");
    Serial.println(SSID);

    WiFi.begin(SSID, PASSWORD);

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
}

void setupMQTT(){
    // Config MQTT Broker connection

    MQTT.setServer(MQTT_BROKER, MQTT_PORT);
    MQTT.setCallback(mqtt_ifrj_callback); 

    // Conn exec

    while (!MQTT.connected()){
        Serial.print("- MQTT Setup: Tentando se conectar ao Broker MQTT: ");
        Serial.println(MQTT_BROKER);

        if(MQTT.connect(MQTT_ID)){
            Serial.println("- MQTT Setup: Conectado com sucesso");
             MQTT.subscribe(MQTT_LED_TOPIC);
        } else {
            Serial.println("- MQTT Setup: Falha ao se conectar, tentando novamente em 2s");
            delay(2000);
        }
    }
}

void setup(void){
    // Set baudrate of serial com
    Serial.begin(115200);

    // Pinmode
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_G, OUTPUT);

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
    MQTT.loop();
    delay(2000);
}

// Callback function
// Called when data is received in one of topics
void mqtt_ifrj_callback(char* topic, byte* payload, unsigned int length)
{
  String msg;
  Serial.print("- MQTT Callback Topic: ");
  Serial.println(topic);

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }

  //Controlando LED RED
  if (msg.equals("R1"))
    {
      digitalWrite(LED_R, HIGH);
      Serial.println("- MQTT Sub Conn: R1 Received");
    } else if (msg.equals("R0"))
    {
      digitalWrite(LED_R, LOW);
      Serial.println("- MQTT Sub Conn: R0 Received");
    }
  if (msg.equals("G1"))
    {
      digitalWrite(LED_G, HIGH);
      Serial.println("- MQTT Sub Conn: G1 Received");
    } else if (msg.equals("G0"))
    {
      digitalWrite(LED_G, LOW);
      Serial.println("- MQTT Sub Conn: G0 Received");
    }
  if (msg.equals("Y1"))
    {
      digitalWrite(LED_Y, HIGH);
      Serial.println("- MQTT Sub Conn: Y1 Received");
    } else if (msg.equals("Y0"))
    {
      digitalWrite(LED_Y, LOW);
      Serial.println("- MQTT Sub Conn: Y0 Received");
    }
   
  

}
