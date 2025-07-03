// T-Beam LED Controller - Receives LoRa commands from XIAO
// Controls dual LEDs - Simple version without battery monitoring

#include <RadioLib.h>

// LoRa configuration - T-Beam standard pins
SX1276 radio = new Module(18, 26, 14, 33);  // T-Beam LoRa pins: CS, DIO0, RST, DIO1

// LED configuration - Red/Green Status System
const int RED_LED_PIN = 4;    // T-Beam Red LED pin (GPIO 4) - Default ON 
const int GREEN_LED_PIN = 25;  // T-Beam Green LED pin (GPIO 25) - Turns ON when button pressed

// State variables
bool buttonPressed = false;   // Track if XIAO button is held
int commandCount = 0;
unsigned long lastCommandTime = 0;
unsigned long lastValidCommand = 0;  // Prevent command spam
unsigned long lastGreenCommand = 0;  // Track last time we got LED_ON
String lastCommand = "";
float lastRSSI = 0;
float lastSNR = 0;
const unsigned long COMMAND_COOLDOWN = 50;  // Min 50ms between valid commands
const unsigned long GREEN_TIMEOUT = 300;    // If no LED_ON for 300ms, auto-return to RED

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // Setup LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  // Initial state: RED ON, GREEN OFF
  digitalWrite(RED_LED_PIN, HIGH);   // Red LED ON (default/standby state)
  digitalWrite(GREEN_LED_PIN, LOW);  // Green LED OFF
  
  Serial.println("=== T-Beam Momentary Red/Green Controller ===");
  Serial.println("Red LED Pin (Standby): " + String(RED_LED_PIN));
  Serial.println("Green LED Pin (Button Held): " + String(GREEN_LED_PIN));
  Serial.println("🔴 Red = Standby | 🟢 Green = Button Held");
  Serial.println("Listening for XIAO momentary button...");
  Serial.println("Commands: LED_ON (hold), LED_OFF (release)");

  // Initialize LoRa radio
  int state = radio.begin(915.0);  // 915MHz (legal in NZ)

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("✅ T-Beam LED Controller ready!");
    
    // Simple LoRa settings (matching XIAO)
    radio.setSpreadingFactor(7);     
    radio.setBandwidth(125.0);       
    radio.setCodingRate(5);          
    radio.setSyncWord(0x12);         
    
    Serial.println("🎧 Listening for XIAO remote commands...");
    Serial.println("📡 LoRa: 915MHz, SF7, BW125");
    Serial.println("💡 LED will respond to button presses!");
    
  } else {
    Serial.print("❌ LoRa initialization failed: ");
    Serial.println(state);
    while(true);
  }
}

void loop() {
  String receivedMessage = "";
  int state = radio.receive(receivedMessage);
  
  if (state == RADIOLIB_ERR_NONE) {
    // Check for command spam protection
    if (millis() - lastValidCommand < COMMAND_COOLDOWN) {
      return;  // Ignore rapid commands
    }
    
    commandCount++;
    lastCommandTime = millis();
    lastValidCommand = millis();
    lastCommand = receivedMessage;
    lastRSSI = radio.getRSSI();
    lastSNR = radio.getSNR();
    
    // Process momentary button commands
    if (receivedMessage == "LED_ON") {
      lastGreenCommand = millis();  // Update green command timestamp
      if (!buttonPressed) {  // Only act if state actually changed
        buttonPressed = true;
        digitalWrite(RED_LED_PIN, LOW);    // Turn OFF red LED
        digitalWrite(GREEN_LED_PIN, HIGH); // Turn ON green LED
        Serial.println("🟢 GREEN ACTIVE!");
        printStatus();
      }
    } else if (receivedMessage == "LED_OFF") {
      if (buttonPressed) {  // Only act if state actually changed
        buttonPressed = false;
        digitalWrite(RED_LED_PIN, HIGH);   // Turn ON red LED
        digitalWrite(GREEN_LED_PIN, LOW);  // Turn OFF green LED
        Serial.println("🔴 RED STANDBY!");
        printStatus();
      }
    }
    
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // Normal - just waiting for data
  } else {
    Serial.print("📡 LoRa error: ");
    Serial.println(state);
  }
  
  // AUTO-TIMEOUT: If we're in GREEN mode but haven't received LED_ON recently, return to RED
  if (buttonPressed && (millis() - lastGreenCommand > GREEN_TIMEOUT)) {
    buttonPressed = false;
    digitalWrite(RED_LED_PIN, HIGH);   // Turn ON red LED
    digitalWrite(GREEN_LED_PIN, LOW);  // Turn OFF green LED
    Serial.println("⏰ AUTO-TIMEOUT → RED STANDBY!");
    Serial.println("   (No button refresh received)");
    printStatus();
  }
  
  delay(10);  // Faster loop for better responsiveness
}

void printStatus() {
  Serial.println("=== Status Update ===");
  Serial.print("LED State: ");
  if (buttonPressed) {
    Serial.println("GREEN (Button Held)");
  } else {
    Serial.println("RED (Standby)");
  }
  
  if (commandCount > 0) {
    Serial.print("Commands Received: ");
    Serial.println(commandCount);
    Serial.print("Last Command: ");
    Serial.println(lastCommand);
    Serial.print("Signal Quality - RSSI: ");
    Serial.print(lastRSSI, 1);
    Serial.print(" dBm, SNR: ");
    Serial.print(lastSNR, 1);
    Serial.println(" dB");
  }
  
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  Serial.println("=====================");
}
