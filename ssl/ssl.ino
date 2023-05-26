#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "redmi";
const char* password = "1234567890";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    const char* host = "script.google.com";
    const int httpsPort = 443;

    if (!client.connect(host, httpsPort)) {
      Serial.println("Connection failed");
      return;
    }

    String url = "/macros/s/AKfycbzFGjK-KuLVMToBxF-WwSYpP6OaDym3zb51X7bK7AoD6fySK2qjcpByUlB7M1rrguk9/exec"; 
    String fingerprint = "08732C18301452C3CA3E027965B4FE90AC3F3E33"; 

    if (client.verify(fingerprint, host)) {
      Serial.println("Certificate match.");
    } else {
      Serial.println("Certificate mis-match");
    }

    String request = String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "User-Agent: ESP8266\r\n" +
                     "Connection: close\r\n\r\n";

    client.print(request);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received");
        break;
      }
    }

    String payload = "";
    while (client.available()) {
      payload = client.readStringUntil('\n');
      Serial.println(payload);
    }

    Serial.println("Closing connection");
    client.stop();
  }

  delay(5000);
}



#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>

const char* ssid = "your_network_ssid";
const char* password = "your_network_password";
const char* host = "example.com";
const int httpsPort = 443;
const char* fingerprint = "your_certificate_fingerprint";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  WiFiClientSecureBearSSL client;
  
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("Certificate verification passed");
  } else {
    Serial.println("Certificate verification failed");
  }
  
  client.print("GET / HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  Serial.println("Response received");
}

void loop() {
  // Your code here
}
