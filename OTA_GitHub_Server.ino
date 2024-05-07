#include <Arduino_ESP32_OTA.h>
#include <WiFi.h>
#include "arduino_secrets.h"
#include "root_ca.h"

/* Network SSID and password from arduino_secrets.h */
static char const SSID[] = SECRET_SSID;
static char const PASS[] = SECRET_PASS;

/* OTA file location (corrected) */
static char const OTA_FILE_LOCATION[] = "https://raw.githubusercontent.com/Satya-Permana/ESP32/main/Blink.ino.ota";

void setup() {
  Serial.begin(9600);

  // Wait for serial connection
  while (!Serial) {}

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");

  // Initialize OTA
  Arduino_ESP32_OTA ota;
  Arduino_ESP32_OTA::Error ota_err = Arduino_ESP32_OTA::Error::None;

  // Set custom Root CA
  ota.setCACert(root_ca);

  // Start OTA update
  Serial.println("Initializing OTA storage...");
  ota_err = ota.begin();
  if (ota_err != Arduino_ESP32_OTA::Error::None) {
    Serial.print("OTA storage initialization failed: ");
    Serial.println((int)ota_err);
    return;
  }

  Serial.println("Downloading OTA update...");
  int ota_download = ota.download(OTA_FILE_LOCATION);
  if (ota_download <= 0) {
    Serial.print("OTA download failed with error code: ");
    Serial.println(ota_download);
    return;
  }

  Serial.print(ota_download);
  Serial.println(" bytes downloaded.");

  Serial.println("Applying OTA update...");
  ota_err = ota.update();
  if (ota_err != Arduino_ESP32_OTA::Error::None) {
    Serial.print("OTA update failed with error code: ");
    Serial.println((int)ota_err);
    return;
  }

  Serial.println("Resetting to apply the update...");
  delay(1000);
  ota.reset();
}

void loop() {
  // Nothing to do here as all actions are in setup
}
