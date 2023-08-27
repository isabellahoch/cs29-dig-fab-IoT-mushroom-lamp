#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>
#include <ArduinoWebsockets.h>

#define NUM_LEDS 30 // Number of LED's in your strip. Adjust accordingly.
#define DATA_PIN D10 // The pin where your data line is connected.

const char* ssid = "dalilab";
const char* password = "helvetica";

// Define the array of leds
CRGB leds[NUM_LEDS];

const char* webSocketServerHostname = "https://cs29-mushroom-lamp-backend.onrender.com";
const int webSocketServerPort = 80; // HTTP port

static uint8_t hue = 0;

using namespace websockets;

bool isLightOn = false; // Boolean variable to track the light's on/off status, initially set to FALSE.

// Array of pretty colors in CHSV format (Hue, Saturation, Value)
const CRGBPalette16 pastelColorsPalette = CRGBPalette16(
  CHSV(30, 255, 200), // Light pink
  CHSV(90, 255, 200), // Light yellow
  CHSV(150, 255, 200), // Light green
  CHSV(210, 255, 200)  // Light blue
);

WebsocketsClient client;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int getWebSocketServerPort() {
  WiFiClient client;
  if (client.connect(webSocketServerHostname, webSocketServerPort)) {
    client.println("GET / HTTP/1.1");
    client.print("Host: ");
    client.println(webSocketServerHostname);
    client.println("Connection: close");
    client.println();

    while (client.connected() && !client.available()) {
      delay(1);
    }

    while (client.available()) {
      String line = client.readStringUntil('\n');
      if (line.startsWith("PORT:")) {
        return line.substring(6).toInt();
      }
    }
  }
  
  return -1; // Error fetching port
}

void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Received message: ");
  Serial.println(message.data());
  if (message.data() == "ON") {
    isLightOn = true;
  } else if (message.data() == "OFF") {
    isLightOn = false;
  }
}

void onEventsCallback(WebsocketsEvent event, String data) {
  Serial.println("Event callback!");
  if(event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connection Opened");
  } else if(event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connection Closed");
  }
}

void setup() { 
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255); // Set initial brightness to 50/255. Adjust this as per your requirement.

  connectToWiFi();

  // Setup Callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  
  int websocketServerPort = getWebSocketServerPort();
  if (websocketServerPort > 0) {
    Serial.print("Connecting to WebSocket server on port ");
    Serial.println(websocketServerPort);
    // Construct WebSocket server URI
    String webSocketServerUri = "ws://" + String(webSocketServerHostname) + ":" + String(websocketServerPort);
    Serial.print("WebSocket Server URI: ");
    Serial.println(webSocketServerUri);
    // Connect to server
    client.connect(webSocketServerUri);
  } else {
    Serial.println("Error fetching WebSocket server port.");
  }
}

void loop() {
  client.poll();

  if (isLightOn) {
    // "Waterfall" effect: each LED takes on the next color in the cycle
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue + i * 10, 255, 255); // Add 'i' to the hue to make each LED a different color (changed to 180 from 255 for a softer feel)
    }

    FastLED.show();

    hue++; // Increment the base hue for the next cycle
  } else {
    FastLED.clear(); // Turn off all LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(0, 0, 0);
    }
    FastLED.show();  // Display the turned-off state
  }

  delay(500); // Delay for a smoother color transition. Adjust this as per your requirement.
}
