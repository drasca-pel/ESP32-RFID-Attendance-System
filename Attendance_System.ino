#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Wi-Fi Settings ---
const char* ssid     = "YOUR_WIFI_SSID";         
const char* password = "YOUR_WIFI_PASSWORD"; 

// --- Google Deployment Link ---
String GOOGLE_SCRIPT_URL = "YOUR_GOOGLE_APPS_SCRIPT_WEB_APP_URL";

// --- Hardware Pin Definitions ---
#define RST_PIN     3
#define SS_PIN      5
#define I2C_SDA     8
#define I2C_SCL     9

#define RED_LED    1
#define BUZZER     2
#define GREEN_LED  10

MFRC522 mfrc522(SS_PIN, RST_PIN); 
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// --- Cooldown Settings ---
String lastCardID = "";
unsigned long lastScanTime = 0;
const unsigned long COOLDOWN_MS = 4000;

void printCentered(String text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, y);
  display.println(text);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  SPI.begin(4, 6, 7, 5); 
  mfrc522.PCD_Init();    
  
  Wire.begin(I2C_SDA, I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("OLED allocation failed");
  }
  display.setRotation(2);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  printCentered("Connecting...", 28);
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(150);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(150);
  digitalWrite(BUZZER, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  display.clearDisplay();
  printCentered("Ready!", 20);
  printCentered("Scan Card", 35);
  display.display();
  Serial.println("--- System Ready! ---");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  
  String cardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     cardID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
     cardID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardID.toUpperCase();
  
  String displayID = "";
  for(int i = 0; i < cardID.length(); i += 2) {
    displayID += cardID.substring(i, i+2);
    if(i < cardID.length() - 2) displayID += " ";
  }
  
  Serial.println("Card Detected: " + displayID);

  if (displayID == lastCardID && (millis() - lastScanTime) < COOLDOWN_MS) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(50);
    return;
  }
  lastCardID = displayID;
  lastScanTime = millis();

  display.clearDisplay();
  printCentered("Sending...", 28);
  display.display();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String url = GOOGLE_SCRIPT_URL + "?cardId=" + displayID;
    url.replace(" ", "%20"); 
    
    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000);
    
    int httpCode = http.GET();
    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode);

    String studentName = "Unknown Student";

    if (httpCode > 0) {
      studentName = http.getString();
      studentName.trim();
      Serial.println("Response Body (Name): " + studentName);
    }
    
    display.clearDisplay();
    
    if (httpCode == 200 && studentName != "Unknown Student" && !studentName.startsWith("Error")) {
      printCentered("Logged!", 20);
      printCentered(studentName, 35);
      display.display();
      
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(800);
      digitalWrite(GREEN_LED, LOW);
      
      Serial.println("Google Sheets Update: Success!");
    } else {
      printCentered("Failed!", 28);
      display.display();
      
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(800);
      digitalWrite(BUZZER, LOW);
      digitalWrite(RED_LED, LOW);
      
      Serial.println("Google Sheets Update: Failed or Student Unknown.");
    }

    http.end();
  }
  
  delay(1500); 
  
  display.clearDisplay();
  printCentered("Ready!", 20);
  printCentered("Scan Card", 35);
  display.display();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
