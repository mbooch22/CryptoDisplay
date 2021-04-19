#include <ArduinoJson.h>

/*******************************************************************
    Update to project to display crypto currency prices using an ESP8266

    Main Hardware:
    - NodeMCU 1.0 (ESP-12E Module)
    - OLED SPI Display (SH1106)

    Written by Matt Bellucci
    
 *******************************************************************/

// ----------------------------
// Standard Libraries - Already Installed if you have ESP8266 set up
// ----------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------


#include "SH1106.h"
#include "SH1106Ui.h"
// https://github.com/squix78/esp8266-oled-ssd1306

#include <ArduinoJson.h>
// !! NOTE !!: When installing this select an older version than V6 from the drop down
// Required by the CoinMarketCapApi Library for parsing the response
// Available on the library manager (Search for "arduino json")
// https://github.com/squix78/esp8266-oled-ssd1306

/* Hardware Node MCU mini SPI pins
 D5 Node MCU - CLK  SPI
 D6 - not connected
 D7 Node MCU - MOSI (DOUT) SPI
 D1 Node MCU - RES SPI
 D2 Node MCU - SDC SPI
 D8 Node MCU - CCS SPI
 3V3 Node MCU - VCC SPI
 GND Node MCU - GND SPI
*/

// Pin definitions for SPI
// ESP8266
//#define OLED_RESET  5   // RESET
//#define OLED_DC     4   // Data/Command
//#define OLED_CS     15  // Chip select
// Node MCU
#define OLED_RESET  D1   // RESET
#define OLED_DC     D2   // Data/Command
#define OLED_CS     D8   // Chip select
// Pins based on your wiring 
#define SCL_PIN D5
#define SDA_PIN D3

// Uncomment one of the following based on OLED type
SH1106 display(true, OLED_RESET, OLED_DC, OLED_CS); // FOR SPI
//SH1106   display(OLED_ADDR, OLED_SDA, OLED_SDC);    // For I2C
//SH1106Ui ui     ( &display );

// ----------------------------
// Configurations - Update these
// ----------------------------

char ssid[] = "XX";       // your network SSID (name)
char password[] = "XX";  // your network key


WiFiClientSecure client;

String ipAddressString;
StaticJsonDocument<750> json;

void setup() {

  Serial.begin(9600);

  // ----------------------------
  // Everything below can be tinkered with if you want but should work as is!
  // ----------------------------

  // Initializing the display
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, F("Bellucci Bitcoin"));
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 18, F("By MB"));
  display.display();
  

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  delay(5000);
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, F("Connecting to Wifi..."));
  display.display();
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int b = 1;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 0, F("Attempt:"));
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 18, (String(b)));
    display.display();
    b++;
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 0, F("Wifi Connected"));
  display.display();
  delay(5000);
  ipAddressString = ip.toString();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, F("Getting Data..."));
  display.display();
  Serial.println("Getting Data...");
}

void loop() {
  yield();
  //Currently working API
  const char* host = "https://api.coincap.io/v2/assets?ids=";            
  String cryptoCoins[] = {"bitcoin", "ethereum", "xrp", "litecoin", "chainlink"}; //add other coins to array if needed 
  int numOfCoins = 5; // update number of coins if needed
  
  for(int i = 0; i< numOfCoins; i++){
    Serial.println(cryptoCoins[i]);
    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure(); //the magic line, use with caution
    Serial.println(host+cryptoCoins[i]);
    client.connect(host+cryptoCoins[i], 443);
    yield();
    http.begin(client, host+cryptoCoins[i]);
    yield();
    String payload;
    Serial.println("before data request");
    yield();
    if (http.GET() >0){
       yield();
       Serial.println("data request");
       payload = http.getString();
       if(payload.length() > 0) {
            Serial.print("Payload:");
            Serial.println(payload);
            DeserializationError error = deserializeJson(json, payload);
            // Test if parsing succeeds.
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
              return;
            }
            Serial.println(json["data"].as<String>());
            Serial.println(json["data"][0]["name"].as<String>());
            Serial.println(json["data"][0]["priceUsd"].as<String>());
            Serial.println(json["data"][0]["changePercent24Hr"].as<String>());
            displayHolding(json["data"][0]["symbol"].as<String>(), json["data"][0]["priceUsd"].as<String>(), json["data"][0]["changePercent24Hr"].as<String>());
            }
          }else {
            Serial.println("Payload Empty");
          }
      yield();
      http.end();
      //delay(5000);
    }
  }


void displayHolding(String symbol, String priceUsd, String changePercent24Hr) {  
    yield();
    display.clear();
  
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 0, symbol);
    display.setFont(ArialMT_Plain_24);
    display.drawString(64, 20, formatCurrency(priceUsd));
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 48, "24h: " + formatPercent(changePercent24Hr) + "%");
 
    display.display();
    delay(2000);
}

String formatCurrency(String priceUsd) {
  float price = priceUsd.toFloat();
  String formattedCurrency = "$";
  int pointsAfterDecimal = 2;
  formattedCurrency.concat(String(price, pointsAfterDecimal));
  Serial.print("Formatted Currency");
  Serial.println(formattedCurrency);
  return formattedCurrency;
}

String formatPercent(String percent) {
  return String(percent.toFloat(), 2);
}


/******************************************************************* 
 Example Payload:
 {
   "data":[
      {
         "id":"bitcoin",
         "rank":"1",
         "symbol":"BTC",
         "name":"Bitcoin",
         "supply":"18672456.0000000000000000",
         "maxSupply":"21000000.0000000000000000",
         "marketCapUsd":"1152624930304.1240977121428560",
         "volumeUsd24Hr":"33833128575.7846446188964901",
         "priceUsd":"61728.6194330367734010",
         "changePercent24Hr":"-2.5350684458562683",
         "vwap24Hr":"61923.3802279466865213",
         "explorer":"https://blockchain.info/"
      }
   ],
   "timestamp":1618602367803
}
 *******************************************************************/
