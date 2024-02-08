/*
The Accelerometer module has 5 pins i.e.,

VCC - To be connected to NodeMCU +3.3v.

X - To be connected to Analog Pin A0 of the NodeMCU.

Y - NIL

Z - NIL

GND - To be connected to Ground Pin (GND) of the NodeMCU.

Note : Since NodeMCU has only one Analog Pin, you can connect either of X, Y, or Z pin to it. here we used x
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #include "ADXL335.h" // Include the ADXL335 library

// Replace these with your WiFi credentials
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// IFTTT webhook key
const char *iftttWebhookKey = "bfOh-57I2CvFbJitZqX1g";

// Define pins for ADXL335
const int xPin = D0; // Analog pin for X-axis
const int yPin = D1; // Analog pin for Y-axis
const int zPin = D2; // Analog pin for Z-axis
int relayPin = D3;
// Define threshold angle
const float thresholdAngle = 45.0; // Threshold angle in degrees
// Variable to track if SMS has been sent
bool smsSent = false;

// Create an instance of the ADXL335 class
// ADXL335 accelerometer;
void sendIFTTTMessage()
{
    // Create HTTPClient object
    HTTPClient http;
    WiFiClient Client;
    // Construct JSON data
    String jsonData = "{\"value1\":\"Angle exceeded 45 degrees!\"}";

    // Construct URL with webhook key
    String url = "http://maker.ifttt.com/trigger/sms/with/key/";
    url += iftttWebhookKey;

    // Send HTTP POST request
    http.begin(Client, url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0)
    {
        Serial.print("IFTTT message sent, response code: ");
        Serial.println(httpResponseCode);
    }
    else
    {
        Serial.print("Failed to send IFTTT message, error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}

void setup()
{
    Serial.begin(9600);
    pinMode(relayPin, OUTPUT);
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected");
    // accelerometer.begin();
}

void loop()
{
    //int16_t x, y, z;

    // accelerometer.getXYZ(&x, &y, &z);
    // Serial.println("value of X/Y/Z: ");
    // Serial.println(x);
    // Serial.println(y);
    // Serial.println(z);
    // Read acceleration values from ADXL335
    int16_t xAccel = analogRead(A0);
    float xAngle = xAccel;
    // int16_t yAccel = y;
    // int16_t zAccel = z;

    // Convert acceleration values to angles (example: using atan2 function)
    // float xAngle = atan2(yAccel, zAccel) * 180.0 / PI;
    // float yAngle = atan2(-xAccel, sqrt(yAccel * yAccel + zAccel * zAccel)) * 180.0 / PI;
    // float zAngle = atan2(sqrt(xAccel * xAccel + yAccel * yAccel), zAccel) * 180.0 / PI;

    // Print angles
    Serial.print("X Angle: ");
    Serial.println(xAccel);

    // Check if any angle exceeds threshold
    if (abs(xAngle) > thresholdAngle)
    {
        // If SMS has not been sent yet, send it
        if (!smsSent)
        {
            // Send message via IFTTT webhook
            sendIFTTTMessage();

            // Set flag to indicate SMS has been sent
            smsSent = true;

            // Activate relay
            digitalWrite(relayPin, HIGH);
        }
    }
    else
    {
        // Reset SMS sent flag and deactivate relay
        smsSent = false;
        digitalWrite(relayPin, LOW);
    }

    delay(1000);
}
