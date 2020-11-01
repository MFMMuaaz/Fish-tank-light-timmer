#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const int bigTankLight = 3;
const int smallTankLight = 4;
const int indicatorWiFi = 2;
bool indicatorStatus = true;

const char *ssid     = "Dialog 4G";
const char *password = "Abcd@12345";
const long utcOffsetInSeconds = 5.5*60*60;
const long updateFrequency = 24*60*60*1000;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void connectToWiFi(){
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    indicatorStatus = !indicatorStatus;
    delay ( 500 );
    Serial.print ( "." );
    digitalWrite(indicatorWiFi, indicatorStatus);
  }
  digitalWrite(indicatorWiFi, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  }

unsigned long lastUpdate = 0;

void setup(){
  Serial.begin(115200);
  pinMode(bigTankLight, OUTPUT);
  pinMode(smallTankLight, OUTPUT);
  pinMode(indicatorWiFi, OUTPUT);
  digitalWrite(indicatorWiFi, HIGH);
  connectToWiFi();

  timeClient.begin();
  Serial.println("Trying to sync the time with ntp.....!");
  if (timeClient.update()){
    Serial.println("Time is updated successfully!!!\n");
    }
  else{
    while (!timeClient.update()){
      timeClient.forceUpdate();
      }
    Serial.println("Time is updated successfully!!!\n");
    }
  lastUpdate = millis();
}

int currentHour = 0;

void loop() {
  unsigned long currentMillis = millis();
  if ((currentMillis-lastUpdate) > updateFrequency){
    Serial.println("Trying to sync the time with ntp.....!");
    if (timeClient.update()){
      Serial.println("Time is updated successfully!!!\n");
      }
    else{
      Serial.println("Time update attempt failed!!!\n");      
      }
    lastUpdate = millis();
    }

  Serial.println(timeClient.getFormattedTime());
//  Serial.print(daysOfTheWeek[timeClient.getDay()]);
//  Serial.print(", ");
//  Serial.print(timeClient.getHours());
//  Serial.print(":");
//  Serial.print(timeClient.getMinutes());
//  Serial.print(":");
//  Serial.println(timeClient.getSeconds());

  currentHour = timeClient.getHours();
  if (currentHour >= 7 && currentHour <= 10){
    digitalWrite(bigTankLight, HIGH);
    digitalWrite(smallTankLight, HIGH);
    Serial.println("Light is switched on in morning!!!");
    }
  else if (currentHour >= 11 && currentHour  <= 16){
    digitalWrite(bigTankLight, HIGH);
    digitalWrite(smallTankLight, LOW);
    Serial.println("Light is switched off!!!");
    }
  else if (currentHour >= 17 && currentHour <= 20){
    digitalWrite(bigTankLight, HIGH);
    digitalWrite(smallTankLight, HIGH);
    Serial.println("Light is switched on in evening!!!");
    }
  else if ((currentHour >= 21 && currentHour <= 23) || (currentHour >= 0 && currentHour <= 6)){
    digitalWrite(bigTankLight, LOW);
    digitalWrite(smallTankLight, LOW);
    Serial.println("Light is switched off!!!");
    }
  delay(1000-millis()+currentMillis);
}
