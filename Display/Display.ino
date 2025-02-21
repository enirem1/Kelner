
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
int FONT_SIZE=2;

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;
struct Dish{
  int id;
  char* name;
  float price;
  int weight;
  char* ingrdients[20];
  char* allergies[20];
};
Dish a={1,"Margarita",10.50,600,{"Cheese", "Tomato", "Dough", "Pepperoni", "Oregano"},{"Gluten","Dairy"}};
// Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  Serial.print(" | Pressure = ");
  Serial.print(touchZ);
  Serial.println();
}
int DishId;
// Print Touchscreen info about X, Y and Pressure (Z) on the TFT Display
void DishBrowse(int touchX, int touchY, int touchZ) {
  // Clear TFT screen
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawRect(0,0,45,235,TFT_WHITE);//Back button
  tft.fillTriangle(30,98,30,143,8,120,TFT_WHITE);// back arrow
  tft.drawRect(275,0,45,235,TFT_WHITE);//next
  tft.fillTriangle(288,98,288,143,310,120,TFT_WHITE);//next arrow
  tft.drawRect(46, 0, 227, 35, TFT_WHITE);//up button
  tft.fillTriangle(138,30,182,30,160,8,TFT_WHITE);//up arrow
  tft.drawRect(46, 200, 227, 35, TFT_WHITE);
  tft.fillTriangle(138,205,182,205,160,227,TFT_WHITE);//down arrow
  tft.drawString("Family Pizza",51,37,2);
  tft.drawString(a.name, 51, 57,4);
  tft.setFreeFont();
  tft.drawFloat(a.price,2,227, 56,2);
    tft.drawNumber(a.weight,227, 37,2);
  tft.drawString("123456789012345678901234567",50, 85,2);// 27 simbols per row for ingredients/allergies
  int centerX = SCREEN_WIDTH / 2;
  int textY = 80;
  String tempText;
  DishId=1;
  
  textY += 20;
  if(touchX>46&&touchX<273&&touchY<35){//up button
    tempText="Go up";
    tft.drawCentreString(tempText,centerX,textY,FONT_SIZE);
  }
  if(touchX<45){//back button
    tempText="Go back";
    tft.drawCentreString(tempText,centerX,textY,FONT_SIZE);
  }
  if(touchX>275){//next button
    tempText="Go next";
    tft.drawCentreString(tempText,centerX,textY,FONT_SIZE);
  }
  if(touchX>46&&touchX<273&&touchY>200){//down button
    tempText="Go down";
    tft.drawCentreString(tempText,centerX,textY,FONT_SIZE);
  }
}


void setup() {
  Serial.begin(115200);

   
  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(1);

  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

  // Clear the screen before writing to it
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
  // Set X and Y coordinates for center of display
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
 
  tft.drawCentreString("Hello, world!", centerX, 30, FONT_SIZE);
  tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);
}

void loop() {
  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z) info on the TFT display and Serial Monitor
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    printTouchToSerial(x, y, z);
    DishBrowse(x, y, z);

    delay(100);
  }
}