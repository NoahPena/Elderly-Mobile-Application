#include <TinyGPS.h>

#include <Adafruit_FONA.h>
#include <HardwareSerial.h>

#define FONA_RX 1
#define FONA_TX 0
#define FONA_RST 2

char replybuffer[255];

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

#define HWSERIAL Serial1
HardwareSerial fonaSS = Serial1;

TinyGPS gps;

uint8_t type;

void setup() 
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial1.begin(4800);
  Serial2.begin(4800);

  if(!fona.begin(fonaSS))
  {
      Serial.println("Couldn't find FONA");
      while(1);
  }

  

  

  
  Serial.println("Twerks");
}

void loop() 
{
  // put your main code here, to run repeatedly:

  //GPS 
//  bool newData = false;
//  unsigned long chars;
//  unsigned short sentences, failed;
//
//  // For one second we parse GPS data and report some key values
//  for (unsigned long start = millis(); millis() - start < 1000;)
//  {
//    while (Serial2.available())
//    {
//      char c = Serial2.read();
//      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
//      if (gps.encode(c)) // Did a new valid sentence come in?
//        newData = true;
//    }
//  }  
//
//  if (newData)
//  {
//    float flat, flon;
//    unsigned long age;
//    gps.f_get_position(&flat, &flon, &age);
//    Serial.print("LAT=");
//    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
//    Serial.print(" LON=");
//    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
//    Serial.print(" SAT=");
//    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
//    Serial.print(" PREC=");
//    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
//  }
//
//  // read the CCID
//  fona.getSIMCCID(replybuffer);  // make sure replybuffer is at least 21 bytes!
//  Serial.print(F("SIM CCID = ")); Serial.println(replybuffer);

  while(!fona.enableGPRS(true)) 
  {
    fona.enableGPRS(false);
      Serial.println(F("Failed to turn on GPRS. Trying again..."));
       
    if(fona.enableGPRS(true)) 
    {
      Serial.println(F("GPRS is on. Ready to make POST requests.")); 
      break;
    }
  }

  post();

}

void post() 
{
  // Post data to website
  uint16_t statuscode;
  int16_t length;
  char url[] = "http://localhost:3000/step";
  char data[80] = "{\"data\":\"1\"}";

  flushSerial();
  Serial.println(url);
  Serial.print(F("Data to post: "));
  Serial.println(data);

  Serial.println(F("****"));
  if (!fona.HTTP_POST_start(url, F("application/json"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&length)) {
    Serial.println("Failed!");
    goto exitFn;
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
  exitFn:
  ; // compiler complains if this isn't here
    // nothing to do after goto, which helps exit function if http fails
}

