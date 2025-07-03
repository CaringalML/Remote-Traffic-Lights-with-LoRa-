// XIAO Momentary Button Control for T-Beam Red/Green LEDs
// + Signal Strength Indicator
// Connect button between D0 and GND (main control)
// Connect signal button between D1 and GND (signal check)
// Connect signal LED to D2 (signal strength indicator)
// Hold main button = Green LED ON (Red OFF)
// Release main button = Red LED ON (Green OFF)
// Press signal button = LED blinks according to signal strength

#include <RadioLib.h>

SX1262 radio = new Module(41, 39, 42, 40);  // Your working XIAO pins

// Main button control pins
const int BUTTON_PIN = 1;    // GPIO1 = D0 on XIAO ESP32-S3 (main control)

// Signal strength indicator pins
const int SIGNAL_BUTTON_PIN = 2;  // GPIO2 = D1 on XIAO ESP32-S3 (signal check)
const int SIGNAL_LED_PIN = 3;     // GPIO3 = D2 on XIAO ESP32-S3 (signal indicator)

// Main button variables
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 20;   // Fast debounce
const unsigned long SEND_INTERVAL = 100;   // Send refresh every 100ms while held
unsigned long lastSendTime = 0;
bool lastSentState = false;  // Track what we last sent
int commandCount = 0;        // Count total commands

// Signal button variables
bool lastSignalButtonState = HIGH;
bool currentSignalButtonState = HIGH;
unsigned long lastSignalDebounceTime = 0;
bool signalButtonPressed = false;

// Signal strength variables
float lastRSSI = -999;  // Store last received signal strength
bool signalLedActive = false;
unsigned long signalLedStartTime = 0;
int signalBlinksRemaining = 0;
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_DURATION = 200;  // LED on/off duration per blink
bool signalLedState = false;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // Setup button pins with pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SIGNAL_BUTTON_PIN, INPUT_PULLUP);
  
  // Setup signal LED pin
  pinMode(SIGNAL_LED_PIN, OUTPUT);
  digitalWrite(SIGNAL_LED_PIN, LOW);
  
  Serial.println("=== XIAO Momentary Button Remote with Signal Indicator ===");
  Serial.println("Main button on D0 (GPIO1) - T-Beam LED control");
  Serial.println("Signal button on D1 (GPIO2) - Signal strength check");
  Serial.println("Signal LED on D2 (GPIO3) - Signal strength indicator");
  Serial.println("🔴 Default: Red LED ON (standby)");
  Serial.println("🟢 Hold main button: Green LED ON");
  Serial.println("🔴 Release: Back to Red LED");
  Serial.println("📶 Press signal button: LED blinks = signal bars (0-10)");
  Serial.println("");
  
  // Initialize LoRa
  int state = radio.begin(915.0);  // 915MHz for NZ
  
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("✅ LoRa initialized successfully!");
    
    // Configure LoRa settings to match T-Beam
    radio.setSpreadingFactor(7);
    radio.setBandwidth(125.0);
    radio.setCodingRate(5);
    radio.setSyncWord(0x12);
    
    Serial.println("📡 LoRa ready - 915MHz, SF7, BW125");
    Serial.println("🎮 Momentary button control active!");
    Serial.println("📶 Signal strength indicator ready!");
    Serial.println("Hold main button for GREEN, release for RED");
    Serial.println("Press signal button to check signal strength");
    Serial.println("");
    
  } else {
    Serial.print("❌ LoRa initialization failed: ");
    Serial.println(state);
    while(true);
  }
}

void loop() {
  // Handle main button (LED control)
  handleMainButton();
  
  // Handle signal button (signal strength check)
  handleSignalButton();
  
  // Handle signal LED blinking
  handleSignalLed();
  
  delay(10);  // Fast loop for responsiveness
}

void handleMainButton() {
  // Read main button state
  int reading = digitalRead(BUTTON_PIN);
  
  // Debouncing
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    currentButtonState = reading;
  }
  
  // Determine if button is held (LOW = pressed due to pullup)
  bool buttonHeld = (currentButtonState == LOW);
  
  // Check if we need to send a command
  bool shouldSend = false;
  
  if (buttonHeld != lastSentState) {
    // State changed - send immediately
    shouldSend = true;
    lastSendTime = millis();
  } else if (buttonHeld && (millis() - lastSendTime > SEND_INTERVAL)) {
    // Button still held - send refresh command
    shouldSend = true;
    lastSendTime = millis();
  }
  
  if (shouldSend) {
    commandCount++;
    String command;
    
    if (buttonHeld) {
      command = "LED_ON";
      Serial.println("🟢 HOLD → GREEN (#" + String(commandCount) + ")");
    } else {
      command = "LED_OFF";  
      Serial.println("🔴 RELEASE → RED (#" + String(commandCount) + ")");
    }
    
    // Send LoRa command
    int state = radio.transmit(command);
    
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println("✅ Sent: " + command);
      printCurrentStatus();
    } else {
      Serial.println("❌ Send failed: " + String(state));
    }
    
    lastSentState = buttonHeld;
  }
  
  lastButtonState = reading;
}

void handleSignalButton() {
  // Don't check signal button if signal LED is currently active
  if (signalLedActive) {
    return;
  }
  
  // Read signal button state
  int reading = digitalRead(SIGNAL_BUTTON_PIN);
  
  // Debouncing
  if (reading != lastSignalButtonState) {
    lastSignalDebounceTime = millis();
  }
  if ((millis() - lastSignalDebounceTime) > DEBOUNCE_DELAY) {
    if (currentSignalButtonState == HIGH && reading == LOW) {
      // Button just pressed
      signalButtonPressed = true;
    }
    currentSignalButtonState = reading;
  }
  
  // Handle button press
  if (signalButtonPressed) {
    signalButtonPressed = false;
    checkSignalStrength();
  }
  
  lastSignalButtonState = reading;
}

void checkSignalStrength() {
  Serial.println("📶 Checking signal strength...");
  
  // Send a test packet to measure signal
  String testCommand = "SIGNAL_TEST";
  int state = radio.transmit(testCommand);
  
  if (state == RADIOLIB_ERR_NONE) {
    // Get the RSSI from the last transmission
    float rssi = radio.getRSSI();
    lastRSSI = rssi;
    
    Serial.println("📡 Test packet sent");
    Serial.println("📶 RSSI: " + String(rssi) + " dBm");
    
    // Convert RSSI to signal bars (0-10)
    int signalBars = rssiToSignalBars(rssi);
    Serial.println("📊 Signal strength: " + String(signalBars) + "/10 bars");
    
    // Start LED blinking sequence or show no signal
    if (signalBars == 0) {
      Serial.println("❌ No usable signal - LED will not activate");
      Serial.println("🔇 Signal too weak for reliable communication");
    } else {
      startSignalLedBlinks(signalBars);
      
      // Print signal quality description
      String quality = getSignalQuality(signalBars);
      Serial.println("📈 Quality: " + quality);
    }
    
  } else {
    Serial.println("❌ Signal test failed: " + String(state));
    Serial.println("🔌 Check LoRa connection or antenna");
    // No blinks for transmission failure
  }
}

int rssiToSignalBars(float rssi) {
  // Convert RSSI to signal bars (0-10 scale)
  // More detailed scale for better indication
  if (rssi >= -30) return 10;     // Exceptional (very close range)
  else if (rssi >= -40) return 9; // Excellent
  else if (rssi >= -50) return 8; // Very Strong
  else if (rssi >= -60) return 7; // Strong
  else if (rssi >= -70) return 6; // Good
  else if (rssi >= -80) return 5; // Fair
  else if (rssi >= -90) return 4; // Weak
  else if (rssi >= -100) return 3; // Very Weak
  else if (rssi >= -110) return 2; // Poor
  else if (rssi >= -120) return 1; // Very Poor
  else return 0;                   // No usable signal
}

void startSignalLedBlinks(int numBlinks) {
  signalLedActive = true;
  signalBlinksRemaining = numBlinks * 2; // *2 because each blink = on + off
  signalLedStartTime = millis();
  lastBlinkTime = millis();
  signalLedState = true; // Start with LED on
  digitalWrite(SIGNAL_LED_PIN, HIGH);
  
  Serial.println("💡 LED will blink " + String(numBlinks) + " times");
}

String getSignalQuality(int signalBars) {
  // Return descriptive quality based on signal bars
  switch(signalBars) {
    case 10: return "Exceptional (Point Blank)";
    case 9:  return "Excellent";
    case 8:  return "Very Strong";
    case 7:  return "Strong";
    case 6:  return "Good";
    case 5:  return "Fair";
    case 4:  return "Weak";
    case 3:  return "Very Weak";
    case 2:  return "Poor";
    case 1:  return "Very Poor";
    case 0:  return "No Signal";
    default: return "Unknown";
  }
}

void handleSignalLed() {
  if (!signalLedActive) {
    return;
  }
  
  // Check if it's time to change LED state
  if (millis() - lastBlinkTime >= BLINK_DURATION) {
    signalLedState = !signalLedState;
    digitalWrite(SIGNAL_LED_PIN, signalLedState ? HIGH : LOW);
    lastBlinkTime = millis();
    signalBlinksRemaining--;
    
    // Check if blinking sequence is complete
    if (signalBlinksRemaining <= 0) {
      signalLedActive = false;
      digitalWrite(SIGNAL_LED_PIN, LOW); // Ensure LED is off
      Serial.println("💡 Signal indicator complete");
      Serial.println("==================");
    }
  }
}

void printCurrentStatus() {
  Serial.println("📊 Current Status:");
  Serial.println("   Commands Sent: " + String(commandCount));
  Serial.println("   Button State: " + String(lastSentState ? "HELD (GREEN)" : "RELEASED (RED)"));
  if (lastRSSI != -999) {
    Serial.println("   Last RSSI: " + String(lastRSSI) + " dBm (" + String(rssiToSignalBars(lastRSSI)) + "/10 bars)");
  }
  Serial.println("   Uptime: " + String(millis()/1000) + "s");
  Serial.println("==================");
}
