#include <Arduino.h>


//http://blynk-cloud.com/auth_token/get/pin
//House Drum = OKmgiy6ZpARxBTKdglU1otbsp2JQNWgU

//http://blynk-cloud.com/OKmgiy6ZpARxBTKdglU1otbsp2JQNWgU/get/V2

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#include <CertStoreBearSSL.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

BearSSL::CertStore certStore;
#include <time.h>

 
const String FirmwareVer={"1.0"}; 
//#define URL_fw_Version "/programmer131/otaFiles/master/version.txt"
//https://github.com/7Hemanth/GithubOTA/raw/main/version.txt
//https://github.com/7Hemanth/GithubOTA/raw/main/version.txt
//https://raw.githubusercontent.com/7Hemanth/GithubOTA/main/version.txt
#define URL_fw_Version "/7Hemanth/WATER/main/version.txt?"
//#define URL_fw_Version "https://raw.githubusercontent.com/7Hemanth/GithubOTA/main/version.txt"

//#define URL_fw_Bin "https://raw.githubusercontent.com/programmer131/otaFiles/master/firmware.bin"

#define URL_fw_Bin "https://raw.githubusercontent.com/7Hemanth/WATER/master/firmware.bin"
//https://github.com/7Hemanth/GithubOTA/blob/main/firmware.bin
//https://github.com/7Hemanth/GithubOTA/blob/main/firmware.bin?raw=true
//https://github.com/7Hemanth/GithubOTA/raw/main/firmware.bin
const char* host = "raw.githubusercontent.com";

const int httpsPort = 443;

void oled_disp();

int i=0;
uint8_t tp = 0;

unsigned long maintime = 0;

int waterper;

// DigiCert High Assurance EV Root CA
const char trustRoot[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j
ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL
MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3
LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug
RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm
+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW
PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM
xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB
Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3
hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg
EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF
MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA
FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec
nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z
eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF
hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2
Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe
vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep
+OkuE6N36B9K
-----END CERTIFICATE-----
)EOF";
X509List cert(trustRoot);


extern const unsigned char caCert[] PROGMEM;
extern const unsigned int caCertLen;

/*
const char* ssid = "sadhak";
const char* password = "S79Pu420";
*/




const char* ssid = "Puni";
const char* password = "Ravindra@7";

//Your Domain name with URL path or IP address with path
String serverName = "http://blynk-cloud.com/OKmgiy6ZpARxBTKdglU1otbsp2JQNWgU/get/V2";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;


void setClock();

void connect_wifi();

unsigned long previousMillis_2 = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 50000;
const long mini_interval = 1000;


void setup() {
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT);
  connect_wifi();
  setClock();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //or 0x3C
  display.clearDisplay(); //for Clearing the display
  display.clearDisplay();
  display.drawRoundRect(80, 20, 40, 40, 4, WHITE);
  display.display();
  delay(2000);
  delay(2000);
  display.fillRoundRect(82, 22, 36, 36, 4, WHITE);
  display.display();
  //oled_disp();
}

void mainloop() {
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "?temperature=24.37";
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
      //  Serial.print("HTTP Response code: ");
       // Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        
        waterper = payload.substring(2,payload.length()-2).toInt();
        //String test = payload.indexOf(1,(payload.length()-2));
        //Serial.println(waterper);

      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}





void setClock() {
   // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  /*
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
  */
}
  
void FirmwareUpdate()
{  
  WiFiClientSecure client;
  client.setTrustAnchors(&cert);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
  client.print(String("GET ") + URL_fw_Version + random(1000,5000) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
    Serial.print("Line");
 Serial.println(line);
  }
  String payload = client.readStringUntil('\n');
 Serial.print("Firmware");
 Serial.println(FirmwareVer);
  Serial.print("Payload");
 Serial.println(payload);
  
  payload.trim();
  Serial.println(payload);
  if(payload.equals(FirmwareVer))
  {   
     Serial.println("Device already on latest firmware version"); 
  }
  else
  {
    Serial.println("New firmware detected");
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); 
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_fw_Bin);
        
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    } 
  }
 }  


void repeatedCall(){
    unsigned long currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) 
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      setClock();
      FirmwareUpdate();
    }

    if ((currentMillis - previousMillis_2) >= mini_interval) {
      static int idle_counter=0;
      previousMillis_2 = currentMillis;    
     /* Serial.print(" Active fw version:");
      Serial.println(FirmwareVer);
      Serial.print("Idle Loop....");
      Serial.println(idle_counter++);
     if(idle_counter%2==0)
        digitalWrite(LED_BUILTIN, HIGH);
     else 
        digitalWrite(LED_BUILTIN, LOW);*/
     if(WiFi.status() == !WL_CONNECTED) 
          connect_wifi();
   }
 }

  /*
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Start");
  WiFi.mode(WIFI_STA);
  connect_wifi();  
  setClock();
  pinMode(LED_BUILTIN, OUTPUT);

  
}
*/

void oled_disp(){
//display.drawRoundRect(82, 22, 36, 36, 4, WHITE);
 /* display.fillRoundRect(80, 40, 40, 40, 4, BLACK);
  display.display();*/
  //int val =  map( waterper, 0, 100, 0, 40);
 // Serial.println(val);
  display.fillRoundRect(82, 22, 36, 36, 4, WHITE);
  display.display();
  display.fillRoundRect(82, 22, 36, map( waterper, 0, 100, 36, 0), 4, BLACK);
  // display.fillRoundRect(82, 22, 36, random(0,36), 4, BLACK);
  display.display();
  delay(200);

  // Changing Font Size
  display.setTextColor(WHITE,BLACK);
  display.setCursor(10,30);
  display.setTextSize(2.5);
  display.println(String(waterper)+" %");
  display.display();
  delay(20);
}



void loop()
{
  repeatedCall();  
  digitalWrite(LED_BUILTIN, HIGH);
  if(millis()>maintime+5000){
    digitalWrite(LED_BUILTIN, LOW);
    mainloop();
    maintime = millis();
     oled_disp();
  }  
 
}

void connect_wifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("O");
  }                                   
  Serial.println("Connected to WiFi");
}

