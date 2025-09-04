#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include <Adafruit_ADS1X15.h>

HTTPClient http;
Preferences preferences;

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

#define PIN_RELAY_AC1 19
#define PIN_RELAY_AC2 23

#define PIN_RELAY_N1 18
#define PIN_RELAY_N2 17
#define PIN_RELAY_N3 16

#define WIFI_SSID "WiFi-Ku"
#define WIFI_PASSWORD "1234567890"

String API_URL = "http://192.168.1.1:3000/api/iot";
String API_KEY = "apiKey";
String DEVICE_ID = "deviceId";

int ADS_ERROR = 0;
int WIFI_CLIENT_STATUS = 0;

// Kalibrasi Sensor Tegangan dan Arus

float PLN_VOLTAGE_MULTIPLER = 80.661;
float PLTS_VOLTAGE_MULTIPLER = 95.721;

float BATT_VOLTAGE_MULTIPLER = 4.96;

float CURRENT_SENSOR_FACTOR_1 = 0.0697;
float CURRENT_SENSOR_FACTOR_2 = 0.1258;
float CURRENT_SENSOR_FACTOR_3 = 0.0687;

float CURRENT_PLN_MULTIPLER = 0.152;
float CURRENT_PLTS_MULTIPLER = 0.343;

// Kontroller Config Default

int AUTOMATED = 1;
u_int32_t CONFIG_TIME = 0;

int SOURCE_PLN = 1;
int SOURCE_PLTS = 1;
float SOURCE_PLTS_MIN = 220;

String RELAY_N1_SOURCE = "auto";
String RELAY_N2_SOURCE = "auto";
String RELAY_N3_SOURCE = "auto";

int CURRENT_RELAY_N1_SOURCE = 1;
int CURRENT_RELAY_N2_SOURCE = 1;
int CURRENT_RELAY_N3_SOURCE = 1;

float RELAY_N1_MAX_LOAD = 0.136;
float RELAY_N2_MAX_LOAD = 0.227;
float RELAY_N3_MAX_LOAD = 0.318;

// Kalibrasi Sensor Arus Terhadap Sumber Tegangan

int CALIBRATED_USED_N1 = 1;
int CALIBRATED_USED_N2 = 1;
int CALIBRATED_USED_N3 = 1;

// Kontroller Local State

float PLN_VOLTAGE = 0;
float PLTS_VOLTAGE = 0;
float BATT_VOLTAGE = 0;

float ADS_CURRENT_AC1 = 0;
float ADS_CURRENT_AC2 = 0;
float ADS_CURRENT_AC3 = 0;

float RAW1_CURRENT_AC1 = 0;
float RAW1_CURRENT_AC2 = 0;
float RAW1_CURRENT_AC3 = 0;

float RAW2_CURRENT_AC1 = 0;
float RAW2_CURRENT_AC2 = 0;
float RAW2_CURRENT_AC3 = 0;

float CURRENT_AC1 = 0;
float CURRENT_AC2 = 0;
float CURRENT_AC3 = 0;

int RELAY_N1_STATE = 0;
int RELAY_N2_STATE = 0;
int RELAY_N3_STATE = 0;

float POWER_RELAY_N1 = 0;
float POWER_RELAY_N2 = 0;
float POWER_RELAY_N3 = 0;

void getSavedDeviceConfig()
{
  AUTOMATED = preferences.getInt("automated", AUTOMATED);
  CONFIG_TIME = preferences.getUInt("configTime", CONFIG_TIME);

  SOURCE_PLN = preferences.getInt("pln", SOURCE_PLN);
  SOURCE_PLTS = preferences.getInt("plts", SOURCE_PLTS);
  SOURCE_PLTS_MIN = preferences.getInt("plts_min", SOURCE_PLTS_MIN);

  RELAY_N1_SOURCE = preferences.getString("relay_sn1", RELAY_N1_SOURCE);
  RELAY_N2_SOURCE = preferences.getString("relay_sn2", RELAY_N2_SOURCE);
  RELAY_N3_SOURCE = preferences.getString("relay_sn3", RELAY_N3_SOURCE);

  RELAY_N1_MAX_LOAD = preferences.getFloat("relay_ln1", RELAY_N1_MAX_LOAD);
  RELAY_N2_MAX_LOAD = preferences.getFloat("relay_ln2", RELAY_N2_MAX_LOAD);
  RELAY_N3_MAX_LOAD = preferences.getFloat("relay_ln3", RELAY_N3_MAX_LOAD);
}

void updateDeviceConfig()
{
  if (WIFI_CLIENT_STATUS == 3)
  {
    http.begin(API_URL + "/config?deviceId=" + DEVICE_ID + "&key=" + API_KEY);
    int httpStatus = http.GET();

    if (httpStatus == HTTP_CODE_OK)
    {
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, http.getString());

      AUTOMATED = doc["data"]["automated"].as<boolean>() ? 1 : 0;
      CONFIG_TIME = doc["data"]["configTime"].as<u_int32_t>();

      SOURCE_PLN = doc["data"]["pln"]["active"].as<boolean>() ? 1 : 0;
      SOURCE_PLTS = doc["data"]["plts"]["active"].as<boolean>() ? 1 : 0;
      SOURCE_PLTS_MIN = doc["data"]["plts"]["min_voltage"].as<int>();

      RELAY_N1_SOURCE = doc["data"]["relay"]["n1"]["source"].as<String>();
      RELAY_N2_SOURCE = doc["data"]["relay"]["n2"]["source"].as<String>();
      RELAY_N3_SOURCE = doc["data"]["relay"]["n3"]["source"].as<String>();

      RELAY_N1_MAX_LOAD = doc["data"]["relay"]["n1"]["max_load"].as<float>();
      RELAY_N2_MAX_LOAD = doc["data"]["relay"]["n2"]["max_load"].as<float>();
      RELAY_N3_MAX_LOAD = doc["data"]["relay"]["n3"]["max_load"].as<float>();

      preferences.putInt("automated", AUTOMATED);
      preferences.putUInt("configTime", CONFIG_TIME);

      preferences.putInt("pln", SOURCE_PLN);
      preferences.putInt("plts", SOURCE_PLTS);
      preferences.putInt("plts_min", SOURCE_PLTS_MIN);

      preferences.putString("relay_sn1", RELAY_N1_SOURCE);
      preferences.putString("relay_sn2", RELAY_N2_SOURCE);
      preferences.putString("relay_sn3", RELAY_N3_SOURCE);

      preferences.putFloat("relay_ln1", RELAY_N1_MAX_LOAD);
      preferences.putFloat("relay_ln2", RELAY_N2_MAX_LOAD);
      preferences.putFloat("relay_ln3", RELAY_N3_MAX_LOAD);

      doc.clear();
    }
  }
  else
  {
    getSavedDeviceConfig();
  }

  http.end();
}

double computeACVolts(Adafruit_ADS1115 ads, int channel, int sampling = 25)
{
  float vmax = 0;
  float vmin = 0;
  float vnow = 0;

  for (int i = 0; i < sampling; i++)
  {
    int16_t adsChannel = ads.readADC_SingleEnded(channel);
    vnow = ads.computeVolts(adsChannel);

    if (vnow > vmax)
    {
      vmax = vnow;
    }
    else
    {
      if (vmin == 0)
      {
        vmin = vnow;
      }
      else
      {
        if (vnow < vmin)
        {
          vmin = vnow;
        }
      }
    }
    delayMicroseconds(100);
  }

  return vmax - vmin;
}

double computeDCVolts(Adafruit_ADS1115 ads, int channel, int sampling = 5)
{
  float voltage = 0;
  for (int i = 0; i < sampling; i++)
  {
    int16_t adsChannel = ads.readADC_SingleEnded(channel);
    voltage += ads.computeVolts(adsChannel);
  }

  return voltage / sampling;
}

void setup()
{
  delay(500);

  preferences.begin("app", false);

  delay(500);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(5000);

    WIFI_CLIENT_STATUS = WiFi.status();

    if (WIFI_CLIENT_STATUS == WL_NO_SSID_AVAIL || WIFI_CLIENT_STATUS == WL_DISCONNECTED)
    {
      getSavedDeviceConfig();
      break;
    }
    else
    {
      updateDeviceConfig();
    }
  }

  delay(500);

  ads1.setGain(GAIN_TWOTHIRDS);
  ads2.setGain(GAIN_TWOTHIRDS);

  delay(500);

  if (!ads1.begin(0x48))
  {
    ADS_ERROR += 1;
  }

  if (!ads2.begin(0x49))
  {
    ADS_ERROR += 1;
  }

  delay(500);

  pinMode(PIN_RELAY_N1, OUTPUT);
  pinMode(PIN_RELAY_N2, OUTPUT);
  pinMode(PIN_RELAY_N3, OUTPUT);

  delay(500);

  pinMode(PIN_RELAY_AC1, OUTPUT);
  pinMode(PIN_RELAY_AC2, OUTPUT);

  delay(500);
}

void loop()
{
  WIFI_CLIENT_STATUS = WiFi.status();
  if (WIFI_CLIENT_STATUS != 3)
  {
    WiFi.reconnect();
    WIFI_CLIENT_STATUS = WiFi.status();
  }

  digitalWrite(PIN_RELAY_AC1, SOURCE_PLN == 1 ? 0 : 1);
  digitalWrite(PIN_RELAY_AC2, SOURCE_PLTS == 1 ? 0 : 1);

  int SOURCE_PLTSN = 0;

  if (ADS_ERROR == 0)
  {
    float ADS_PLN_VOLTAGE = computeDCVolts(ads1, 0, 8);
    float ADS_PLTS_VOLTAGE = computeDCVolts(ads1, 1, 8);
    float ADS_BATT_VOLTAGE = computeDCVolts(ads1, 2, 5);

    char CHAR_ADS_PLN_VOLTAGE[10];
    char CHAR_ADS_PLTS_VOLTAGE[10];
    char CHAR_ADS_BATT_VOLTAGE[10];

    sprintf(CHAR_ADS_PLN_VOLTAGE, "%.2f", ADS_PLN_VOLTAGE);
    sprintf(CHAR_ADS_PLTS_VOLTAGE, "%.2f", ADS_PLTS_VOLTAGE);
    sprintf(CHAR_ADS_BATT_VOLTAGE, "%.2f", ADS_BATT_VOLTAGE);

    float RAW_PLN_VOLTAGE = atof(CHAR_ADS_PLN_VOLTAGE);
    float RAW_PLTS_VOLTAGE = atof(CHAR_ADS_PLTS_VOLTAGE);
    float RAW_BATT_VOLTAGE = atof(CHAR_ADS_BATT_VOLTAGE);

    char CHAR_RAW_PLN_VOLTAGE[10];
    char CHAR_RAW_PLTS_VOLTAGE[10];
    char CHAR_RAW_BATT_VOLTAGE[10];

    sprintf(CHAR_RAW_PLN_VOLTAGE, "%.0f", (SOURCE_PLN && RAW_PLN_VOLTAGE > 0.65 ? RAW_PLN_VOLTAGE : 0) * PLN_VOLTAGE_MULTIPLER);
    sprintf(CHAR_RAW_PLTS_VOLTAGE, "%.0f", (SOURCE_PLTS && RAW_PLTS_VOLTAGE > 1.95 ? RAW_PLTS_VOLTAGE : 0) * PLTS_VOLTAGE_MULTIPLER);
    sprintf(CHAR_RAW_BATT_VOLTAGE, "%.2f", (RAW_BATT_VOLTAGE > 0 ? RAW_BATT_VOLTAGE : 0) * BATT_VOLTAGE_MULTIPLER);

    PLN_VOLTAGE = atof(CHAR_RAW_PLN_VOLTAGE);
    PLTS_VOLTAGE = atof(CHAR_RAW_PLTS_VOLTAGE);
    BATT_VOLTAGE = atof(CHAR_RAW_BATT_VOLTAGE);

    if (PLN_VOLTAGE > 5)
    {
      SOURCE_PLTSN += 1;
    }

    if (PLTS_VOLTAGE > 5 && PLTS_VOLTAGE > SOURCE_PLTS_MIN)
    {
      SOURCE_PLTSN += 2;
    }

    if (SOURCE_PLTSN != 0)
    {
      RELAY_N1_STATE = RELAY_N1_SOURCE == "auto" ? (SOURCE_PLTSN == 1 ? 1 : (SOURCE_PLTSN == 2 ? 0 : (POWER_RELAY_N1 > RELAY_N1_MAX_LOAD ? 1 : 0))) : (RELAY_N1_SOURCE == "pln" ? 1 : 0);
      RELAY_N2_STATE = RELAY_N2_SOURCE == "auto" ? (SOURCE_PLTSN == 1 ? 1 : (SOURCE_PLTSN == 2 ? 0 : (POWER_RELAY_N2 > RELAY_N2_MAX_LOAD ? 1 : 0))) : (RELAY_N2_SOURCE == "pln" ? 1 : 0);
      RELAY_N3_STATE = RELAY_N3_SOURCE == "auto" ? (SOURCE_PLTSN == 1 ? 1 : (SOURCE_PLTSN == 2 ? 0 : (POWER_RELAY_N3 > RELAY_N3_MAX_LOAD ? 1 : 0))) : (RELAY_N3_SOURCE == "pln" ? 1 : 0);

      digitalWrite(PIN_RELAY_N1, RELAY_N1_STATE);
      digitalWrite(PIN_RELAY_N2, RELAY_N2_STATE);
      digitalWrite(PIN_RELAY_N3, RELAY_N3_STATE);

      CALIBRATED_USED_N1 = RELAY_N1_STATE == 1 ? 0 : 1;
      CALIBRATED_USED_N2 = RELAY_N2_STATE == 1 ? 0 : 1;
      CALIBRATED_USED_N3 = RELAY_N3_STATE == 1 ? 0 : 1;

      ADS_CURRENT_AC1 = computeACVolts(ads2, 0, 20);
      ADS_CURRENT_AC2 = computeACVolts(ads2, 1, 20);
      ADS_CURRENT_AC3 = computeACVolts(ads2, 2, 20);

      char CHAR_ADS_CURRENT_AC1[15];
      char CHAR_ADS_CURRENT_AC2[15];
      char CHAR_ADS_CURRENT_AC3[15];

      sprintf(CHAR_ADS_CURRENT_AC1, "%.4f", ADS_CURRENT_AC1 * 1000);
      sprintf(CHAR_ADS_CURRENT_AC2, "%.4f", ADS_CURRENT_AC2 * 1000);
      sprintf(CHAR_ADS_CURRENT_AC3, "%.4f", ADS_CURRENT_AC3 * 1000);

      RAW1_CURRENT_AC1 = atof(CHAR_ADS_CURRENT_AC1) * CURRENT_SENSOR_FACTOR_1;
      RAW1_CURRENT_AC2 = atof(CHAR_ADS_CURRENT_AC2) * CURRENT_SENSOR_FACTOR_2;
      RAW1_CURRENT_AC3 = atof(CHAR_ADS_CURRENT_AC3) * CURRENT_SENSOR_FACTOR_3;

      char CHAR_RAW1_CURRENT_AC1[15];
      char CHAR_RAW1_CURRENT_AC2[15];
      char CHAR_RAW1_CURRENT_AC3[15];

      sprintf(CHAR_RAW1_CURRENT_AC1, "%.2f", RAW1_CURRENT_AC1);
      sprintf(CHAR_RAW1_CURRENT_AC2, "%.2f", RAW1_CURRENT_AC2);
      sprintf(CHAR_RAW1_CURRENT_AC3, "%.2f", RAW1_CURRENT_AC3);

      RAW2_CURRENT_AC1 = atof(CHAR_RAW1_CURRENT_AC1);
      RAW2_CURRENT_AC2 = atof(CHAR_RAW1_CURRENT_AC2);
      RAW2_CURRENT_AC3 = atof(CHAR_RAW1_CURRENT_AC3);

      char CHAR_RAW2_CURRENT_AC1[15];
      char CHAR_RAW2_CURRENT_AC2[15];
      char CHAR_RAW2_CURRENT_AC3[15];

      sprintf(CHAR_RAW2_CURRENT_AC1, "%.3f", (RAW2_CURRENT_AC1 > 0.15 ? RAW2_CURRENT_AC1 : 0) * (CALIBRATED_USED_N1 == 0 ? CURRENT_PLN_MULTIPLER : CURRENT_PLTS_MULTIPLER));
      sprintf(CHAR_RAW2_CURRENT_AC2, "%.3f", (RAW2_CURRENT_AC2 > 0.15 ? RAW2_CURRENT_AC2 : 0) * (CALIBRATED_USED_N2 == 0 ? CURRENT_PLN_MULTIPLER : CURRENT_PLTS_MULTIPLER));
      sprintf(CHAR_RAW2_CURRENT_AC3, "%.3f", (RAW2_CURRENT_AC3 > 0.15 ? RAW2_CURRENT_AC3 : 0) * (CALIBRATED_USED_N3 == 0 ? CURRENT_PLN_MULTIPLER : CURRENT_PLTS_MULTIPLER));

      CURRENT_AC1 = atof(CHAR_RAW2_CURRENT_AC1);
      CURRENT_AC2 = atof(CHAR_RAW2_CURRENT_AC2);
      CURRENT_AC3 = atof(CHAR_RAW2_CURRENT_AC3);

      POWER_RELAY_N1 = (CURRENT_AC1 * (CALIBRATED_USED_N1 == 0 ? PLN_VOLTAGE : PLTS_VOLTAGE));
      POWER_RELAY_N2 = (CURRENT_AC2 * (CALIBRATED_USED_N2 == 0 ? PLN_VOLTAGE : PLTS_VOLTAGE));
      POWER_RELAY_N3 = (CURRENT_AC3 * (CALIBRATED_USED_N3 == 0 ? PLN_VOLTAGE : PLTS_VOLTAGE));

      RELAY_N1_STATE = RELAY_N1_SOURCE == "auto" ? (SOURCE_PLTSN == 1 ? 1 : (SOURCE_PLTSN == 2 ? 0 : (POWER_RELAY_N1 > RELAY_N1_MAX_LOAD ? 1 : 0))) : (RELAY_N1_SOURCE == "pln" ? 1 : 0);
      RELAY_N2_STATE = RELAY_N2_SOURCE == "auto" ? (SOURCE_PLTSN == 1 ? 1 : (SOURCE_PLTSN == 2 ? 0 : (POWER_RELAY_N2 > RELAY_N2_MAX_LOAD ? 1 : 0))) : (RELAY_N2_SOURCE == "pln" ? 1 : 0);
      RELAY_N3_STATE = RELAY_N3_SOURCE == "auto" ? (SOURCE_PLTSN == 1 ? 1 : (SOURCE_PLTSN == 2 ? 0 : (POWER_RELAY_N3 > RELAY_N3_MAX_LOAD ? 1 : 0))) : (RELAY_N3_SOURCE == "pln" ? 1 : 0);

      digitalWrite(PIN_RELAY_N1, RELAY_N1_STATE);
      digitalWrite(PIN_RELAY_N2, RELAY_N2_STATE);
      digitalWrite(PIN_RELAY_N3, RELAY_N3_STATE);

      CALIBRATED_USED_N1 = RELAY_N1_STATE == 1 ? 0 : 1;
      CALIBRATED_USED_N2 = RELAY_N2_STATE == 1 ? 0 : 1;
      CALIBRATED_USED_N3 = RELAY_N3_STATE == 1 ? 0 : 1;
    }
    else
    {
      RELAY_N1_STATE = 1;
      RELAY_N2_STATE = 1;
      RELAY_N3_STATE = 1;

      digitalWrite(PIN_RELAY_N1, RELAY_N1_STATE);
      digitalWrite(PIN_RELAY_N2, RELAY_N2_STATE);
      digitalWrite(PIN_RELAY_N3, RELAY_N3_STATE);

      CALIBRATED_USED_N1 = 0;
      CALIBRATED_USED_N2 = 0;
      CALIBRATED_USED_N3 = 0;

      POWER_RELAY_N1 = 0;
      POWER_RELAY_N2 = 0;
      POWER_RELAY_N3 = 0;

      CURRENT_AC1 = 0;
      CURRENT_AC2 = 0;
      CURRENT_AC3 = 0;
    }
  }
  else
  {
    RELAY_N1_STATE = 1;
    RELAY_N2_STATE = 1;
    RELAY_N3_STATE = 1;

    digitalWrite(PIN_RELAY_N1, RELAY_N1_STATE);
    digitalWrite(PIN_RELAY_N2, RELAY_N2_STATE);
    digitalWrite(PIN_RELAY_N3, RELAY_N3_STATE);
  }

  delay(1000);

  if (WIFI_CLIENT_STATUS == 3)
  {
    http.begin(API_URL + "/sensor?deviceId=" + DEVICE_ID + "&key=" + API_KEY);
    http.setTimeout(1000);

    DynamicJsonDocument doc(2048);

    doc["vpln"] = PLN_VOLTAGE;
    doc["vplts"] = PLTS_VOLTAGE;

    doc["vbatt"] = BATT_VOLTAGE;

    doc["lpln"] = (RELAY_N1_STATE == 1 ? POWER_RELAY_N1 : 0) + (RELAY_N2_STATE == 1 ? POWER_RELAY_N2 : 0) + (RELAY_N3_STATE == 1 ? POWER_RELAY_N3 : 0);
    doc["lplts"] = (RELAY_N1_STATE == 0 ? POWER_RELAY_N1 : 0) + (RELAY_N2_STATE == 0 ? POWER_RELAY_N2 : 0) + (RELAY_N3_STATE == 0 ? POWER_RELAY_N3 : 0);

    doc["relay"]["n1"]["power"] = POWER_RELAY_N1;
    doc["relay"]["n1"]["current"] = CURRENT_AC1;
    doc["relay"]["n1"]["source"] = RELAY_N1_STATE == 0 ? "plts" : "pln";

    doc["relay"]["n2"]["power"] = POWER_RELAY_N2;
    doc["relay"]["n2"]["current"] = CURRENT_AC2;
    doc["relay"]["n2"]["source"] = RELAY_N2_STATE == 0 ? "plts" : "pln";

    doc["relay"]["n3"]["power"] = POWER_RELAY_N3;
    doc["relay"]["n3"]["current"] = CURRENT_AC3;
    doc["relay"]["n3"]["source"] = RELAY_N3_STATE == 0 ? "plts" : "pln";

    String payload;
    serializeJson(doc, payload);

    int httpStatus = http.PUT(payload);

    if (httpStatus == HTTP_CODE_OK)
    {
      DynamicJsonDocument doc2(512);
      deserializeJson(doc2, http.getString());

      u_int32_t NEW_CONFIG_TIME = doc2["data"]["configTime"].as<u_int32_t>();

      if (NEW_CONFIG_TIME != CONFIG_TIME)
      {
        updateDeviceConfig();

        CONFIG_TIME = NEW_CONFIG_TIME;
        preferences.putUInt("configTime", NEW_CONFIG_TIME);
      }

      doc2.clear();
    }

    doc.clear();
    http.end();
  }
}