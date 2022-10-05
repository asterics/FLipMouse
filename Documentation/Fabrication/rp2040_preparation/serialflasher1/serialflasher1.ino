/*
 *  RP2040 - ESP32 serial passthrough
 *  
 *  Resets the ESP32 into download mode when flashed (115k2 Baud)
 */


#define ORANGE_LED_PIN  6

#define ESP32_RESET_PIN 3
#define ESP32_GPIO0_PIN 2

#define LED_TOGGLE_TIME    20000


void setup() {
  //LED output
  pinMode(ORANGE_LED_PIN, OUTPUT); 
  //ESP32 reset / download pins
  pinMode(ESP32_RESET_PIN, OUTPUT);
  pinMode(ESP32_GPIO0_PIN, OUTPUT);

  //enable both serial interfaces
  Serial.begin(115200);
  Serial2.begin(115200);
  
  //enable ESP32 into download mode
  digitalWrite(ESP32_GPIO0_PIN,LOW);   // connect GPIO0 to GND
  digitalWrite(ESP32_RESET_PIN,LOW);   // reset ESP32
  delay(100); 
  digitalWrite(ESP32_RESET_PIN,HIGH);
  delay(100);
  digitalWrite(ESP32_GPIO0_PIN,HIGH);
}

long cnt=0;
long bypass_reset=0;

// the loop function runs over and over again forever
void loop() {
  // toggle LED
  
  if (!(cnt++ % LED_TOGGLE_TIME)) 
    digitalWrite(ORANGE_LED_PIN,!digitalRead(ORANGE_LED_PIN));

  // pass serial transfer to Serial2 (ESP)
  
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial2.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial2.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial2.read());   // read it and send it out Serial (USB)
  }
  
}
