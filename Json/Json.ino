#include <WiFi.h>         // For ESP32 (use <ESP8266WiFi.h> for ESP8266)
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "enirem";        // Replace with your Wi-Fi SSID
const char* password = "12345678"; // Replace with your Wi-Fi Password

const char* serverUrlMenu = "http://192.168.238.250:5000/a/";  // Replace with your Menu API URL
const char* serverUrlIngredients = "http://192.168.238.250:5000/article/ingredients/";  // Replace with your Ingredients API URL

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");

    fetchJsonData(serverUrlMenu, "menu");
    fetchJsonData(serverUrlIngredients, "ingredients");
}

void fetchJsonData(const char* url, const char* dataType) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(url);

        int httpCode = http.GET();  // Send GET request
        if (httpCode > 0) { 
            String payload = http.getString();
            Serial.println("\nReceived JSON from: " + String(url));
            Serial.println(payload);

            // Parse JSON
            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                JsonArray dataArray = doc[dataType]; // Get menu or ingredients

                Serial.println("\n" + String(dataType) + ":");
                for (JsonObject item : dataArray) {
                    if (strcmp(dataType, "menu") == 0) {
                        Serial.print("ID: ");
                        Serial.println(item["id"].as<int>());
                        Serial.print("Name: ");
                        Serial.println(item["name"].as<String>());
                        Serial.print("Price: ");
                        Serial.println(item["price"].as<String>());
                        Serial.print("Weight: ");
                        Serial.println(item["weight"].as<int>());
                    } else if (strcmp(dataType, "ingredients") == 0) {
                        Serial.print("Ingredient: ");
                        Serial.println(item["name"].as<String>());
                    }
                    Serial.println();
                }
            } else {
                Serial.println("JSON parsing failed!");
            }
        } else {
            Serial.print("HTTP request failed, error: ");
            Serial.println(http.errorToString(httpCode));
        }
        http.end();
    } else {
        Serial.println("Wi-Fi Disconnected!");
    }
}

void loop() {
    // Optionally, call fetchJsonData() periodically
}
