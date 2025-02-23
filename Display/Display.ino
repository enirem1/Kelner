#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
TFT_eSPI tft = TFT_eSPI();

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
int x, y, z;
struct Dish{
  int id;
  char* name;
  float price;
  int weight;
  char* ingredients[20];
  char* allergies[20];
};
Dish a = {1, "Pepperoni", 10.50, 600, {"Cheese", "Tomato", "Dough", "Pepperoni", "Oregano", "Mushrooms", "Olives", "Peppers", "Onions", "Basil"}, {"Gluten", "Dairy", "Soy", "Nuts", "Eggs"}};

// Function to display ingredients in a row with wrapping
void drawRowText(char* words[], int count, int startX, int startY, int textSize) {
    int cursorX = startX;
    int cursorY = startY;
    int spaceWidth = 6 * textSize; // Space width
    
    tft.setTextSize(textSize);
    for (int i = 0; i < count; i++) {
        int wordWidth = tft.textWidth(words[i]);
        
        // Check if word fits on the current line
        if (cursorX + wordWidth > TEXT_MAX_WIDTH) {
            cursorX = startX;
            cursorY += 15; // Move to the next row
        }
        
        tft.setCursor(cursorX, cursorY);
        tft.print(words[i]);
        tft.print(i < count - 1 ? ", " : ""); // Add comma if not the last word
        cursorX += wordWidth + spaceWidth; // Move cursor for next word
    }
}

// Display Dish Information
void DishBrowse(int touchX, int touchY, int touchZ) {
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
    
    // Dish Name
    tft.drawString("Family Pizza", 51, 37, 2);
    tft.drawString(a.name, 51, 57, 4);
    tft.drawFloat(a.price, 2, 227, 56, 2);
    
    tft.drawNumber(a.weight, 227, 37, 2);
    
    // Display Ingredients in a Row
    tft.setCursor(50, 85);
    tft.print("Ingredients:");
    drawRowText(a.ingredients, 10, TEXT_MARGIN, 100, 1);
    
    // Display Allergens in a Row
    tft.setCursor(50, 130);
    tft.print("Allergens:");
    drawRowText(a.allergies, 5, TEXT_MARGIN, 145, 1);
}

void setup() {
    Serial.begin(115200);
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(1);
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
}

void loop() {
    if (touchscreen.tirqTouched() && touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();
        x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
        y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
        z = p.z;
        DishBrowse(x, y, z);
        delay(100);
    }
}
