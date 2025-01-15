#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_Password";

// Telegram Bot Token
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"  // Replace with your Telegram Bot Token

// Authorized Telegram users
const String AUTHORIZED_USERS[] = {"0123456789"}; // Replace with your telegram User ID
const int NUM_AUTHORIZED_USERS = sizeof(AUTHORIZED_USERS) / sizeof(AUTHORIZED_USERS[0]);

// PC's MAC address(format: "AA:BB:CC:DD:EE:FF")
const char* TARGET_MAC = "AA:BB:CC:DD:EE:FF";  // PC's MAC address

// Initialize Telegram Bot
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Initialize Wake-on-LAN
WiFiUDP UDP;
WakeOnLan WOL(UDP);

// Check for messages every 3 second
const unsigned long BOT_DELAY = 3000;
unsigned long bot_lasttime;

// Function to check if user is authorized
bool isAuthorizedUser(String user_id) {
  for (int i = 0; i < NUM_AUTHORIZED_USERS; i++) {
    if (AUTHORIZED_USERS[i] == user_id) {
      return true;
    }
  }
  return false;
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String user_id = bot.messages[i].from_id;
    String text = bot.messages[i].text;

    Serial.print(chat_id);
    Serial.print(" : ");
    Serial.println(text);

    // Check if user is authorized
    if (!isAuthorizedUser(user_id)) {
      bot.sendMessage(chat_id, "Unauthorized access denied.", "");
      continue;
    }

    if (text == "WakePC") {
      // Send Wake-on-LAN packet
      if (WOL.sendMagicPacket(TARGET_MAC)) {
        bot.sendMessage(chat_id, "Wake-on-LAN command sent to PC.", "");
      } else {
        bot.sendMessage(chat_id, "Failed to send Wake-on-LAN command.", "");
      }
    }
    else if (text == "/start") {
      String welcome = "Welcome!\n";
      welcome += "Send 'WakePC' to wake up the computer.\n";
      bot.sendMessage(chat_id, welcome, "");
    }
    else{
      bot.sendMessage(chat_id, "Unknown request.\nUse the command '/start' for a guide.", "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure secured client to skip certificate validation
  secured_client.setInsecure();

  // Configure Wake-on-LAN
  WOL.setRepeat(3, 100); // Optional: Send packet 3 times with 100ms between them
}

void loop() {
  if (millis() - bot_lasttime > BOT_DELAY) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    bot_lasttime = millis();
  }
}