# ESP32-WOL-Telegram-Bot
This project uses an ESP32 to interface with a Telegram bot, enabling you to remotely wake your PC via Wake-on-LAN (WOL). By sending a specific command ("Wake PC") to the Telegram bot, the ESP32 sends a WOL magic packet to your computer's network interface, waking it up remotely.

# Features
- Wi-Fi Connection: Connects the ESP32 to your local Wi-Fi network.
- Telegram Integration: Communicates with a Telegram bot to receive commands.
- Wake-on-LAN: Sends a magic packet to wake up a PC using its MAC address.
- Secure Access: Ensures only a specific Telegram chat ID can send valid commands.

# Requirements
- ESP32 board
- Arduino IDE with the following libraries:
  - UniversalTelegramBot
  - WakeOnLan
  - ArduinoJson
- A PC configured for Wake-on-LAN

# Setup
1. Clone this repository to your local machine.
2. Open the ESP32_Telegram_WOL.ino file in the Arduino IDE.
3. Replace the placeholders with your Wi-Fi credentials, Telegram bot token, chat ID, and PC MAC address.
4. Upload the code to your ESP32.
5. Verify your PC's Wake-on-LAN functionality.

# Notes
- Ensure Wake-on-LAN is enabled on your PC (BIOS and network adapter settings).
- Test the magic packet functionality before deployment.
