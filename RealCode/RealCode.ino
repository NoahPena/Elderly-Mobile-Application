#include "Adafruit_FONA.h"

#define FONA_RST 3

char replybuffer[255];


HardwareSerial *fonaSerial = &Serial1;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char* buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

bool hasCalled = false;


void setup()
{
  while (!Serial);

  pinMode(13, OUTPUT);
  pinMode(12, INPUT);

  Serial.begin(115200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;
  }
  
  // Print module IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  disableGPRS();

  delay(2000);

  while(!enableGPRS())
  {
    delay(3000);
  }

  

  digitalWrite(13, HIGH);




}


void loop()
{
  if(digitalRead(12) && !hasCalled)
  {
    postData();
  }
  else if(digitalRead(12) && hasCalled)
  {
    hasCalled = false;
    pickUp();

  }

  if(fona.getCallStatus() == 3)
  {
    Serial.println("Calling...");
    hasCalled = true;
  }
}

bool enableGPRS()
{
  bool value = fona.enableGPRS(true);
  return value;
}

void disableGPRS()
{
  fona.enableGPRS(false);
}

void pickUp()
{
  fona.pickUp();
}


void postData()
{
  uint16_t statuscode;
    int16_t length;
    char* url = "elderly-mobile.herokuapp.com";
    char* data = "{\"number\": \"+13616882314\", \"latitude\": \"27.714518\", \"longititude\": \"-97.328522\"}";


    flushSerial();
    Serial.println(F("NOTE: in beta! Use simple websites to post!"));
    Serial.println(F("URL to post (e.g. httpbin.org/post):"));
    Serial.print(F("http://"));
    Serial.println(url);
    Serial.println(F("Data to post (e.g. \"foo\" or \"{\"simple\":\"json\"}\"):"));
    Serial.println(data);

    Serial.println(F("****"));
    if (!fona.HTTP_POST_start(url, F("application/json"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&length)) {
      Serial.println("Failed!");
      return;
    }
    while (length > 0) {
      while (fona.available()) {
        char c = fona.read();

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
        UDR0 = c;
#else
        Serial.write(c);
#endif

        length--;
        if (! length) break;
      }
    }
    Serial.println(F("\n****"));
    fona.HTTP_POST_end();
}



void flushSerial() {
  while (Serial.available())
    Serial.read();
}

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}
uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
  }
  Serial.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    Serial.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      //Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
