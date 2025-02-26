#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "enirem";
const char* password = "12345678";

// API endpoint
const char* baseAPI = "http://192.168.32.250:5000";  // Use a base URL for efficiency
struct Category{
  int Id;
  String Name;
}
struct Dish {
  int Id;
  String Name;
  float Price;
  int Weight;
  String Ingredients[20];
  String Allergies[20];
};

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");

  // Fetch dishes by category
  fetchByCategory(4);
}

void loop() {
  // No repeated calls needed for now
}

void fetchOneDish(int dishId) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }

  HTTPClient http;
  String url = String(baseAPI) + "/article/" + String(dishId);
  http.begin(url);

  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    String payload = http.getString();
    Serial.println("API Response: " + payload);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print("Deserialization failed: ");
      Serial.println(error.f_str());
      return;
    }

    Dish meal;
    meal.Id = doc["id"].as<int>();
    meal.Name = doc["name"].as<String>();
    meal.Price = doc["price"].as<float>();
    meal.Weight = doc["weight"].as<int>();

    Serial.println("Extracted Data:");
    Serial.print("ID: ");
    Serial.println(meal.Id);
    Serial.print("Name: ");
    Serial.println(meal.Name);
    Serial.print("Price: ");
    Serial.println(meal.Price);
    Serial.print("Weight: ");
    Serial.println(meal.Weight);
  } else {
    Serial.print("HTTP Request failed, error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void fetchCategory(){

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
  Serial.println("API Response: " + payload);
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
  for (int j = 0; j < mealCount; j++) {
    Serial.println("Extracted Data:");
    Serial.print("ID: ");
    Serial.println(Dishes[j].Id);
    Serial.print("Name: ");
    Serial.println(Dishes[j].Name);
    Serial.print("Price: ");
    Serial.println(Dishes[j].Price);
    Serial.print("Weight: ");
    Serial.println(Dishes[j].Weight);
    Serial.print("Ingredients: ");
    Serial.print(Dishes[j].Ingredients[0]);
    for (int k = 1; k < 20 && Dishes[j].Ingredients[k] != ""; k++) {
      Serial.print(", ");
      Serial.print(Dishes[j].Ingredients[k]);
    }
    Serial.println("");
    Serial.print("Allergies: ");
    Serial.print(Dishes[j].Allergies[0]);
    for (int k = 1; k < 20 && Dishes[j].Allergies[k] != ""; k++) {
      
      Serial.print(", ");
      Serial.println(Dishes[j].Allergies[k]);
    }
    Serial.println("\n--------------------------");
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
