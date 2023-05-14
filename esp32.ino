#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define Fire_digital 34
#define ECHO_PIN 35
#define TRIG_PIN 33
#define RELAY_PIN 26

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "LOL";
const char* password = "12345678";

#define BOTtoken "6255940257:AAFETMkiqln5Wms4Xpyo3fwUpCGiwR60CbY"
#define CHAT_ID "1278616572"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 500;
unsigned long lastTimeBotRan;

void setup() {
  Serial.begin(115200);

  pinMode(Fire_digital, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(TRIG_PIN, LOW);
  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  
  int firesensorDigital = digitalRead(Fire_digital);
  if (firesensorDigital == LOW){
    Serial.println("Ada Api!");

    digitalWrite(RELAY_PIN, HIGH);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ada Api!");
    lcd.setCursor(2, 1);
    lcd.print((String) "Jarak Api : " + distance + " cm");

    String welcome = "Use the following commands to interact with the ESP32-CAM \n";
      welcome += "/photo : Ambil gambar kondisi lingkungan\n";
      welcome += "/flash : toggles flash LED \n";
    bot.sendMessage(CHAT_ID, "ADA API!", "");
    bot.sendMessage(CHAT_ID, welcome, "");
  }
  else{  
    digitalWrite(RELAY_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tidak Ada Api");
    lcd.setCursor(2, 1);
    lcd.print("");
  }
  delay(100);
}