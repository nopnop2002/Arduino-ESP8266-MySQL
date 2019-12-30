// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonDocument. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.
//

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* SSID = "SSID"; // SSID of Wifi router
const char* PASSWORD = "PASSWORD"; // Password of Wifi router
String MYSQL_SERVER_IP = "SERVER_IP"; // IP address of MySQL(php-crud-api) server
String MYSQL_SERVER_POTY = "SERVER_PORT"; // Port number of php-crud-api(Default is 8080)
String PHP_URL = "/api.php/records/posts/";
String BASE_URL;
//String BASE_URL = "http://192.168.10.43:8080/api.php/records/posts/";

WiFiClient client;
HTTPClient http;

void parseVariant(JsonVariant variant) {
  JsonVariant val_id = variant.getMember("id");
  int id = val_id.as<int>();
  Serial.print("id=");
  Serial.println(id);

  JsonVariant val_user_id = variant.getMember("user_id");
  int user_id = val_user_id.as<int>();
  Serial.print("user_id=");
  Serial.println(user_id);

  JsonVariant val_category_id = variant.getMember("category_id");
  int category_id = val_category_id.as<int>();
  Serial.print("category_id=");
  Serial.println(category_id);

  JsonVariant val_content = variant.getMember("content");
  const char * content = val_content.as<char*>();
  Serial.print("content=");
  Serial.println(content);
}



int parseString(String input) {
  DynamicJsonDocument doc(1024);
  
  //deserializeJson(doc, input);
  DeserializationError error = deserializeJson(doc, input);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return 0;
  }   

#if 0      
  Serial.print("measureJson=");
  Serial.println(measureJson(doc));
#endif
  
  JsonObject root = doc.as<JsonObject>();

#if 0
  const char * records = root["records"];
  Serial.print("records=");
  Serial.println(records);
#endif
  JsonArray array = root.getMember("records");
  //Serial.print("val_records.size()=");
  //Serial.println(val_records.size());

  //JsonArray array  = doc.as<JsonArray>();
  Serial.print("array.size()=");
  Serial.println(array.size());
  if (array.size() == 0) {
    JsonVariant variant = doc.as<JsonVariant>();
    parseVariant(variant);
  } else {
    for(int i=0; i<array.size(); i++) {
      JsonVariant variant = array[i];
      parseVariant(variant);
      Serial.println();
    }
  }
  return measureJson(doc);
}

void initWiFi() {
  Serial.println();
  Serial.println("Connect to " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}

int httpRequestGet(String path)
{
  http.begin(BASE_URL + path);
  int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return 0;

  }

  if (httpCode != HTTP_CODE_OK) {
    return 0;
  }

  String response =  http.getString();
  http.end();
  Serial.println("##[RESULT]## ==> " + response);
  parseString(response);
  return response.length();
}

int httpRequestPost()
{
  http.begin(BASE_URL);
  http.addHeader("Content-Type", "Content-Type: application/json");
  int httpCode = http.POST("user_id=1&category_id=3&content=Hello World");
  //int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return 0;

  }

  if (httpCode != HTTP_CODE_OK) {
    return 0;
  }

  String response =  http.getString();
  http.end();
  Serial.println("##[RESULT]## ==> " + response);
  //parseString(response);
  return response.toInt();
}

void setup() {
  Serial.begin(115200);
  initWiFi();

  Serial.println("[Create to MySQL] - sending request...");
  BASE_URL = "http://" + MYSQL_SERVER_IP + ":" + MYSQL_SERVER_POTY + PHP_URL;
  Serial.println("##[BASE_URL]## ==> " + BASE_URL);
  Serial.println();
  
  int id = httpRequestPost();
  if (id != 0) {
    Serial.println("Create Success. Addes id = ");
    Serial.println(id);
  } else {
    Serial.println("Create Fail");
  }
  httpRequestGet("");
}

void loop() {
}
