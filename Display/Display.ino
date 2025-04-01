#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

TFT_eSPI tft = TFT_eSPI();
#define table_id 1
// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TEXT_MARGIN 50
#define TEXT_MAX_WIDTH (SCREEN_WIDTH - 2 * TEXT_MARGIN)
int FONT_SIZE = 1;

// Touchscreen coordinates: (x, y) and pressure (z)
const char* ssid = "enirem";
const char* password = "12345678";
float total;
byte showCat;
byte showDish;
byte stage;
const char* baseAPI = "http://192.168.165.250:5000";  // Use a base URL for efficiency
struct Category {
  int Id;
  String Name;
};
struct Dish {
  int Id;
  String Name;
  float Price;
  int Weight;
  String Ingredients[20];
  String Allergies[20];
};
int selected;
Category selectedCategory;
Dish selectedDish;

// Function to display ingredients in a row with wrapping
void drawRowText(String words[], int count, int startX, int startY, int textSize) {
  int cursorX = startX;
  int cursorY = startY;
  int spaceWidth = 6 * textSize;  // Space width

  tft.setTextSize(textSize);
  for (int i = 0; i < count; i++) {
    int wordWidth = tft.textWidth(words[i].c_str());  // Convert String to C-string for width calculation

    // Check if word fits on the current line
    if (cursorX + wordWidth > TEXT_MAX_WIDTH) {
      cursorX = startX;
      cursorY += 15;  // Move to the next row
    }

    tft.setCursor(cursorX, cursorY);
    tft.print(words[i]);
    if (words[i] != "") {
      tft.print(", ");  // Add comma if not the last word
      cursorX += tft.textWidth(", ");
    }
    cursorX += wordWidth + spaceWidth;  // Move cursor for next word
  }
}



void initUI() {

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Navigation Buttons
  tft.drawRect(0, 0, 45, 235, TFT_WHITE);
  tft.fillTriangle(30, 98, 30, 143, 8, 120, TFT_WHITE);
  tft.drawRect(275, 0, 45, 235, TFT_WHITE);
  tft.fillTriangle(288, 98, 288, 143, 310, 120, TFT_WHITE);
  tft.drawRect(46, 0, 227, 35, TFT_WHITE);
  tft.fillTriangle(138, 30, 182, 30, 160, 8, TFT_WHITE);
  tft.drawRect(46, 200, 227, 35, TFT_WHITE);
  tft.fillTriangle(138, 205, 182, 205, 160, 227, TFT_WHITE);
}
void setup() {
  selected = 0;
  showCat = 0;
  showDish = 0;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(1);
  tft.init();
  tft.setRotation(1);
  initUI();
  tft.drawString("Categories", 95, 40, 4);
}

void fetchCategory() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }

  HTTPClient http;
  String url = String(baseAPI) + "/category";
  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode != 200) {
    Serial.print("HTTP Request failed, error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return;
  }

  String payload = http.getString();
  Serial.println("API Response: " + payload);
  http.end();

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Deserialization failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonArray categories = doc.as<JsonArray>();
  int catCount = categories.size();
  Category cats[catCount];

  for (int i = 0; i < catCount; i++) {
    JsonObject category = categories[i];
    cats[i].Id = category["id"].as<int>();
    cats[i].Name = category["name"].as<String>();
  }
  int textX;

  if (showCat == 0) {
    initUI();
    textX = 160 - tft.textWidth(cats[0].Name);
    tft.drawString("Categories", 95, 40, 4);
    tft.drawString(cats[0].Name, textX, 90, 4);
    String temp = "Total:" + String(total, 2);
    textX = 160 - tft.textWidth(temp);
    tft.drawString(temp, textX, 130, 4);
    showCat = 1;
  }  // Print extracted values
  if (touchscreen.tirqTouched() && touchscreen.touched()) {

    TS_Point p = touchscreen.getPoint();
    int touchX = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    int touchY = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);

    initUI();



    if (touchX > 46 && touchX < 273 && touchY < 35 && selected > 0) {  //up button


      selected = selected - 1;
    }
    if (touchX > 46 && touchX < 273 && touchY > 200 && selected < catCount - 1) {  //down button


      selected = selected + 1;
    }
    if (touchX > 275) {  //next button
      selectedCategory.Id = cats[selected].Id;
      selectedCategory.Name = cats[selected].Name;
      selected = 0;
      stage = 1;
    }
    textX = 160 - tft.textWidth(cats[selected].Name);
    tft.drawString("Categories", 95, 40, 4);
    tft.drawString(cats[selected].Name, textX, 90, 4);
    textX = 160 - tft.textWidth(cats[selected].Name);
    String temp = "Total:" + String(total, 2);
    textX = 160 - tft.textWidth(temp);
    tft.drawString(temp, textX, 130, 4);
      
  }
}
void fetchByCategory(int category) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }


  HTTPClient http;
  String url = String(baseAPI) + "/category/articles/" + String(category);
  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode != 200) {
    Serial.print("HTTP Request failed, error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return;
  }

  String payload = http.getString();

  http.end();

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Deserialization failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonArray meals = doc.as<JsonArray>();
  int mealCount = meals.size();
  Dish Dishes[mealCount];

  for (int i = 0; i < mealCount; i++) {
    JsonObject meal = meals[i];
    Dishes[i].Id = meal["id"].as<int>();
    Dishes[i].Name = meal["name"].as<String>();
    Dishes[i].Price = meal["price"].as<float>();
    Dishes[i].Weight = meal["weight"].as<int>();
  }

  // Fetch ingredients for each dish
  for (int j = 0; j < mealCount; j++) {
    fetchIngredients(Dishes[j]);
    fetchAllergies(Dishes[j]);
  }

  // Print extracted values


  if (mealCount > 0) {
    if (showDish == 0) {

      initUI();
      tft.drawString(selectedCategory.Name, 51, 37, 2);
      tft.setCursor(150, 40);
      tft.setTextSize(1);
      tft.print(Dishes[0].Price, 2);
      int cursorX = 150 + tft.textWidth(String(Dishes[0].Price, 2));
      tft.setCursor(cursorX, 40);
      tft.print("lv.");
      cursorX += tft.textWidth("lv.   ");
      tft.setCursor(cursorX, 40);
      tft.print(String(Dishes[0].Weight));
      cursorX += tft.textWidth(String(Dishes[0].Weight));
      tft.setCursor(cursorX, 40);
      tft.print("g.");
      tft.drawString(Dishes[0].Name, 51, 57, 4);

      // Display Ingredients in a Row
      tft.setCursor(50, 85);
      tft.print("Ingredients:");
      drawRowText(Dishes[0].Ingredients, 10, TEXT_MARGIN, 100, 1);

      // // Display Allergens in a Row
      tft.setCursor(50, 130);
      tft.print("Allergens:");
      drawRowText(Dishes[0].Allergies, 5, TEXT_MARGIN, 145, 1);

      showDish = 1;
    }
    if (touchscreen.tirqTouched() && touchscreen.touched()) {
      TS_Point p = touchscreen.getPoint();
      int touchX = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
      int touchY = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);

      if (touchX > 46 && touchX < 273 && touchY < 35) {  //up button


        selected = selected - 1;
      }
      if (touchX > 46 && touchX < 273 && touchY > 200) {  //down button
        selected = selected + 1;
      }


      if (touchX < 45) {
        stage = 0;
        showCat = 0;
        showDish = 0;  //back button
      }
      if (selected < 0) {
        selected = 0;
      }
      if (selected > mealCount - 1) {
        selected = mealCount - 1;
      }
      if (touchX > 275) {
        showCat = 0;
        selectedDish = Dishes[selected];
        showDish = 0;
        stage = 2;  //next button
      }
      initUI();
      tft.drawString(selectedCategory.Name, 51, 37, 2);
      tft.setCursor(150, 40);
      tft.setTextSize(1);
      tft.print(Dishes[selected].Price, 2);
      int cursorX = 150 + tft.textWidth(String(Dishes[selected].Price, 2));
      tft.setCursor(cursorX, 40);
      tft.print("lv.");
      cursorX += tft.textWidth("lv.   ");
      tft.setCursor(cursorX, 40);
      tft.print(String(Dishes[selected].Weight));
      cursorX += tft.textWidth(String(Dishes[selected].Weight));
      tft.setCursor(cursorX, 40);
      tft.print("g.");
      tft.drawString(Dishes[selected].Name, 51, 57, 4);

      // Display Ingredients in a Row
      tft.setCursor(50, 85);
      tft.print("Ingredients:");
      drawRowText(Dishes[selected].Ingredients, 10, TEXT_MARGIN, 100, 1);

      // // Display Allergens in a Row
      tft.setCursor(50, 130);
      tft.print("Allergens:");
      drawRowText(Dishes[selected].Allergies, 5, TEXT_MARGIN, 145, 1);
      
    }
  } else {
    stage = 0;
  }
}

void fetchIngredients(Dish& dish) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }

  HTTPClient http;
  String url = String(baseAPI) + "/article/ingredients/" + String(dish.Id);
  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode != 200) {
    Serial.print("HTTP Request failed, error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return;
  }

  String payload = http.getString();
  Serial.println("Ingredients API Response: " + payload);
  http.end();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Deserialization failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonArray ingredients = doc.as<JsonArray>();
  for (int i = 0; i < ingredients.size() && i < 20; i++) {
    dish.Ingredients[i] = ingredients[i]["name"].as<String>();
  }
}

void fetchAllergies(Dish& dish) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }

  HTTPClient http;
  String url = String(baseAPI) + "/article/allergies/" + String(dish.Id);
  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode != 200) {
    Serial.print("HTTP Request failed, error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return;
  }

  String payload = http.getString();
  Serial.println("Ingredients API Response: " + payload);
  http.end();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Deserialization failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonArray allergies = doc.as<JsonArray>();
  for (int i = 0; i < allergies.size() && i < 20; i++) {
    dish.Allergies[i] = allergies[i]["name"].as<String>();
  }
}
void SendOrder() {

  if (selected > -1) {
    initUI();
    tft.setTextSize(2);
    tft.drawRect(0, 0, 45, 235, TFT_WHITE);
    tft.fillTriangle(30, 98, 30, 143, 8, 120, TFT_WHITE);
    tft.drawRect(275, 0, 45, 235, TFT_WHITE);
    tft.fillTriangle(288, 98, 288, 143, 310, 120, TFT_WHITE);
    String temp = "Do you want ";
    int cursorX = 160 - tft.textWidth(temp) / 2;
    tft.setCursor(cursorX, 60);
    tft.print(temp);
    temp = "to order: ";
    cursorX = 160 - tft.textWidth(temp) / 2;
    tft.setCursor(cursorX, 80);
    tft.print(temp);
    cursorX = 160 - tft.textWidth(selectedDish.Name) / 2;
    tft.setCursor(cursorX, 100);
    tft.print(selectedDish.Name);
    temp = "For:" + String(selectedDish.Price, 2) + "lv.";
    cursorX = 160 - tft.textWidth(selectedDish.Name) / 2;
    tft.setCursor(cursorX, 120);
    tft.print(temp);
    tft.setTextSize(1);

    selected = -1;
  }
  delay(250);
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    int touchX = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    int touchY = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    if (touchX < 45) {
      stage = 0;
      selected = 0;
      showCat = 0;
      showDish = 0;  //back button
    }
    if (touchX > 275) {
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        total += selectedDish.Price;
        String url = String(baseAPI) + "/Orders";
        // Start HTTP POST request
        http.begin(url);
        http.addHeader("Content-Type", "application/json");

        // Prepare JSON data
        DynamicJsonDocument doc(1024);
        doc["id_table"] = table_id;
        doc["id_article"] = selectedDish.Id;
        doc["status"] = 0;


        String jsonData;
        serializeJson(doc, jsonData);  // Convert JSON object to string

        // Send POST request
        int httpResponseCode = http.POST(jsonData);

        // Handle response
        if (httpResponseCode > 0) {
          Serial.print("HTTP Response Code: ");
          Serial.println(httpResponseCode);
          String response = http.getString();
          Serial.println("Response: " + response);
        } else {
          Serial.print("Error on sending POST request: ");
          Serial.println(httpResponseCode);
        }

        // Close connection
        http.end();
        stage = 0;
        showCat = 0;
        showDish = 0;
        selected = 0;
      }
    }
  }
}
void loop() {
  stage = 0;
  while (stage == 0) {

    fetchCategory();
    delay(100);
  }
  while (stage == 1) {

    fetchByCategory(selectedCategory.Id);
    delay(100);
  }
  while (stage == 2) {
    SendOrder();
    delay(100);
  }
}
