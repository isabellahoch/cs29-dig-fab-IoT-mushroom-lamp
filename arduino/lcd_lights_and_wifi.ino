// Include necessary libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <WiFi.h>

// Define Wi-Fi Info
const char* ssid = "dalilab";
const char* password = "helvetica";

// Define LED info
#define NUM_LEDS 120 // Number of LED's in your strip. Adjust accordingly.
#define DATA_PIN D10 // The pin where your data line is connected.

// Define screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LOGO_WIDTH 48
#define LOGO_HEIGHT 60
#define MICRO_WIDTH 16
#define MICRO_HEIGHT 19

// Define the maximum number of messages to display
#define MAX_MESSAGES 5

// Define SSD1306 display instance
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create an array to hold the messages
char messages[MAX_MESSAGES][20];

bool didReceiveMessage = false;

// Define the array of leds
CRGB leds[NUM_LEDS];

static uint8_t hue = 0;

bool isLightOn = false; // Boolean variable to track the light's on/off status, initially set to FALSE.

// Array of pretty colors in CHSV format (Hue, Saturation, Value)
const CRGBPalette16 pastelColorsPalette = CRGBPalette16(
  CHSV(30, 255, 200), // Light pink
  CHSV(90, 255, 200), // Light yellow
  CHSV(150, 255, 200), // Light green
  CHSV(210, 255, 200)  // Light blue
);

void connectToWiFi() {
  String wifiMessage = "Connecting to Wi-Fi...";
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  displayTeleprompterText(wifiMessage.c_str(), 25);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  char wifiConfirmationMessage[100];
  sprintf(wifiConfirmationMessage, "Connected to:\n\n %s \n\nwith IP address:\n\n %s", ssid, WiFi.localIP().toString().c_str());

  Serial.println(wifiConfirmationMessage);

  displayTeleprompterText(wifiConfirmationMessage, 25);

}

// Define custom bitmap for a mushroom created using custom digital drawing and https://manytools.org/hacker-tools/image-to-byte-array/go/
static const unsigned char PROGMEM mushroom_bmp[] = {
  0x0, 0x1f, 0xff, 0xff, 0xfe, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0x0, 
  0x0, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x0, 0xff, 0xf0, 0x0, 0xff, 0xc0, 
  0x1, 0xff, 0x80, 0x0, 0x3f, 0xe0, 0x3, 0xfe, 0xe0, 0x7, 0x1f, 0xf0, 
  0x7, 0xfd, 0xe0, 0x7, 0x8f, 0xf8, 0xf, 0xf9, 0xe0, 0x7, 0x87, 0xfc, 
  0xf, 0xf0, 0xe0, 0x7, 0x81, 0xfc, 0x1f, 0xe0, 0x0, 0x0, 0x1, 0xfe, 
  0x1f, 0xc0, 0x0, 0x0, 0x0, 0xff, 0x3f, 0x80, 0x0, 0x0, 0x0, 0x7f, 
  0x3f, 0x80, 0xf, 0x0, 0x0, 0x3f, 0x7f, 0x0, 0x1f, 0x0, 0x0, 0x3f, 
  0x7f, 0x0, 0x1f, 0x0, 0xe0, 0x1f, 0xfe, 0x0, 0xe, 0x0, 0xf0, 0xf, 
  0xfe, 0x0, 0x0, 0x0, 0xf0, 0xf, 0xfc, 0x0, 0x0, 0x0, 0xf0, 0x7, 
  0xbc, 0x78, 0x0, 0x0, 0x0, 0x7, 0xbc, 0x7c, 0x0, 0x0, 0x0, 0x7, 
  0x7c, 0x7c, 0x0, 0x0, 0x0, 0xe3, 0x7c, 0x78, 0x3, 0xc0, 0x0, 0xf3, 
  0x7c, 0x0, 0x3, 0xc0, 0x0, 0xf3, 0x7c, 0x0, 0x7, 0xc0, 0x0, 0xe3, 
  0x7e, 0x0, 0x3, 0xc0, 0x0, 0x3, 0x7f, 0x0, 0x0, 0x80, 0x0, 0x7, 
  0x3f, 0xc0, 0x0, 0x0, 0x0, 0xf, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x3, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x1f, 0xff, 0xff, 0xe7, 0xfe, 
  0x0, 0x0, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0xff, 0xff, 0xc8, 0x0, 
  0x0, 0x4, 0xff, 0xff, 0xe8, 0x0, 0x0, 0x5, 0xff, 0xff, 0xe8, 0x0, 
  0x0, 0x5, 0xff, 0xff, 0xe8, 0x0, 0x0, 0x5, 0xff, 0xff, 0xe8, 0x0, 
  0x0, 0x5, 0xff, 0xff, 0xf4, 0x0, 0x0, 0x5, 0xff, 0xff, 0xf4, 0x0, 
  0x0, 0x5, 0xff, 0xff, 0xf4, 0x0, 0x0, 0xb, 0xff, 0xff, 0xfa, 0x0, 
  0x0, 0xb, 0xff, 0xff, 0xfa, 0x0, 0x0, 0xb, 0xff, 0xff, 0xfa, 0x0, 
  0x0, 0xb, 0xff, 0xff, 0xfd, 0x0, 0x0, 0xb, 0xff, 0xff, 0xfd, 0x0, 
  0x0, 0xb, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x3, 0xff, 0xff, 0xfd, 0x0, 
  0x0, 0x17, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x17, 0xff, 0xff, 0xfd, 0x0, 
  0x0, 0x17, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x17, 0xff, 0xff, 0xfd, 0x0, 
  0x0, 0x17, 0xff, 0xff, 0xfd, 0x0, 0x0, 0x17, 0xff, 0xff, 0xfd, 0x0, 
  0x0, 0xb, 0xff, 0xff, 0xfa, 0x0, 0x0, 0xb, 0xff, 0xff, 0xfa, 0x0, 
  0x0, 0x9, 0xff, 0xff, 0xf4, 0x0, 0x0, 0x5, 0xff, 0xff, 0xe8, 0x0, 
  0x0, 0x2, 0x7f, 0xff, 0x90, 0x0, 0x0, 0x1, 0x9f, 0xfc, 0x60, 0x0
};

static const unsigned char PROGMEM medium_mushroom_bmp[] = {
  0x7, 0xff, 0xf0, 0xf, 0xe7, 0xf8, 0x1f, 0xc3, 0xfc, 0x3f, 0xf7, 0xfe, 
  0x3c, 0xf9, 0xde, 0x78, 0xf8, 0xf, 0x70, 0xfd, 0xe7, 0xf7, 0xf9, 0xe3, 
  0xef, 0xfd, 0xe3, 0xef, 0xbf, 0xcd, 0xef, 0x3f, 0xf, 0xe7, 0xff, 0xfd, 
  0x77, 0xff, 0xfb, 0x7f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xff, 
  0x0, 0xff, 0xf8, 0x3, 0xff, 0xc0, 0x3, 0xff, 0xe0, 0x3, 0xff, 0xe0, 
  0x3, 0xff, 0xe0, 0x3, 0xff, 0xf0, 0x3, 0xff, 0xf0, 0x1, 0xff, 0xf0, 
  0x3, 0xff, 0xf0, 0x7, 0xff, 0xf0, 0x3, 0xff, 0xf0, 0x3, 0xff, 0xf0, 
  0x3, 0xff, 0xe0, 0x1, 0xff, 0xc0
};

static const unsigned char PROGMEM micro_mushroom_bmp[] = {
  0x0, 0x0, 0x3, 0xe0, 0xf, 0xf0, 0x1f, 0xf8, 0x1f, 0xfc, 0x1f, 0xfc, 
  0x3f, 0xfc, 0x3f, 0xfc, 0x7, 0xf0, 0x7, 0xf0, 0x7, 0xf0, 0x7, 0xf0, 
  0x7, 0xf0, 0x7, 0xf0, 0x7, 0xf0, 0x7, 0xe0, 0x3, 0xc0, 0x0, 0x0, 
  0x0, 0x0
};

void shuffleArrayLeft(char arr[MAX_MESSAGES][20], int size) {
    char temp[20];
    strcpy(temp, arr[0]); // Store the first element
    
    for (int i = 1; i < size; i++) {
        strcpy(arr[i - 1], arr[i]); // Move each element to the left
    }

    strcpy(arr[size - 1], temp); // Place the temporarily stored element at the end
}


// Function to display text like a teleprompter
void displayTeleprompterText(const char *text, int scrollSpeed) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int xPos = 8;
  int yPos = SCREEN_HEIGHT;
  
  while (yPos > -SCREEN_HEIGHT) {
    display.clearDisplay();
    display.setCursor(xPos, yPos);
    display.println(text);
    display.display();
    delay(scrollSpeed);
    yPos--;
  }
}

int messageIndex = 0;

String getNextMessage() {
  String testMessages[] = {
    "Message 1",
    "Message 2",
    "Message 3",
    "Message 4",
    "Message 5"
  };

  String nextMessage = testMessages[messageIndex];
  messageIndex = (messageIndex + 1) % (sizeof(testMessages) / sizeof(testMessages[0])); // Cycle through messages

  return nextMessage;
}


void displayMessages(int scrollSpeed) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int yPos = SCREEN_HEIGHT;

  for (int i = 0; i < MAX_MESSAGES || yPos > -SCREEN_HEIGHT; i++) {
    String message = messages[i];
    display.setCursor(0, yPos);
    display.println(message);
    yPos--;
    display.display();
    delay(scrollSpeed); // pause for scroll speed
  }
  display.display();
}

void displayMushroomImage(int delayTime) {
  display.clearDisplay();
  display.drawBitmap(SCREEN_WIDTH / 2 - LOGO_WIDTH / 2, SCREEN_HEIGHT / 2 - LOGO_HEIGHT / 2, mushroom_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(delayTime);
}

void displayMicroMushroomImage(int xIncrement) {
  display.drawBitmap(xIncrement + MICRO_WIDTH / 2, SCREEN_HEIGHT / 2 - MICRO_HEIGHT / 2, micro_mushroom_bmp, MICRO_WIDTH, MICRO_HEIGHT, SSD1306_WHITE);
}

void displayLoadingMushrooms() {
  for(int x = 4; x < SCREEN_WIDTH-MICRO_WIDTH; x+=24) {
    displayMicroMushroomImage(x);
    delay(500);
    display.display();
  }
}

void displayWelcomeAnimation() {

    const char* welcomeMessage = "IoT MUSHROOM LAMP";
    const char* copyrightMessage = "Copyright 2023\nIsabella Hochschild\nCOSC 29 DigFab";

    display.clearDisplay();

    // Display mushroom loading animation
    displayLoadingMushrooms();

    display.clearDisplay();

    // Display welcome message
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, SCREEN_HEIGHT - 12);
    display.println(welcomeMessage);
    display.display();
    delay(3000); // Display welcome message for 3 seconds

    

    // Display copyright message
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 12);
    display.println(copyrightMessage);
    display.display();
    delay(3000); // Display copyright message for 3 seconds
}

void clearMessagesArray() {
  for (int i = 0; i < MAX_MESSAGES; i++) {
    strcpy(messages[i], " ");
  }
}

void setup() {
  Serial.begin(9600);

  Serial.println("hellooooo serial monitor!");

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255); // Set initial brightness to 50/255. Adjust this as per your requirement.


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {
      // Loop forever if display initialization fails
    }
  }

  delay(1000); // Pause for 1 second

  // Display the mushroom bitmap

  // Display mushroom image for 5 seconds
  displayMushroomImage(5000);

  // Clear the display before showing teleprompter messages
  display.clearDisplay();
  display.display();

  displayWelcomeAnimation();

  // clear the display before next portion
  display.clearDisplay();
  display.display();

  // initialize messages array
  clearMessagesArray();

  // connect to wifi
  connectToWiFi();
}

void loop() {
  // Clear the display
  display.clearDisplay();

  // "Waterfall" effect: each LED takes on the next color in the cycle
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((hue+i)%180, 180, 180); // Add 'i' to the hue to make each LED a different color (changed to 180 from 255 for softer feel)
  }

  FastLED.show();

  shuffleArrayLeft(messages, MAX_MESSAGES);

  String newMessage = getNextMessage();
  didReceiveMessage = true;
  displayTeleprompterText(newMessage.c_str(), 25);

  if(!didReceiveMessage) {
    delay(500);
  }

  hue++;
  // Display a teleprompter-like text
  // displayTeleprompterText("Welcome to My Mushroom Lamp Animation!", 100);

  // // Clear the display
  // display.clearDisplay();
  

  // Clear the display
  // display.clearDisplay();

  // Display a nature scene animation
  // testanimate(mushroom_mbp, LOGO_WIDTH, LOGO_HEIGHT);

  // Clear the display
  // display.clearDisplay();
}

// Rest of the code remains the same
