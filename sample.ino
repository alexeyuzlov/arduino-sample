// https://github.com/Makuna/Rtc/blob/master/examples/DS3231_Simple/DS3231_Simple.ino

#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

const int BRIGHTNESS_PIN = A0;

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = 2; // define CLK pin (any digital pin)
const byte PIN_DIO = 3; // define DIO pin (any digital pin)
SevenSegmentExtended display(PIN_CLK, PIN_DIO);

unsigned long latestForDate = 0;
unsigned long latestForBrightness = 0;
const int DEBOUNCE_DATE = 1000;
const int DEBOUNCE_BRIGHTNESS = 50;
int brightnessInPercents = 50;
int brightnessRawValue = 0;

// run setup code
void setup()
{
  pinMode(BRIGHTNESS_PIN, INPUT);

  display.begin();           // initializes the display

  Rtc.Begin();
  // setupRTC();
}

void loop()
{
  unsigned long currentTime = millis();

  if (currentTime > latestForBrightness + DEBOUNCE_BRIGHTNESS)
  {
    latestForBrightness = currentTime;
    brightnessRawValue = analogRead(BRIGHTNESS_PIN);
    brightnessInPercents = map(brightnessRawValue, 0, 1023, 0, 100);
    brightnessInPercents = constrain(brightnessInPercents, 0, 100);
    display.setBacklight(brightnessInPercents);
  }

  if (currentTime > latestForDate + DEBOUNCE_DATE)
  {
    latestForDate = currentTime;

    RtcDateTime now = Rtc.GetDateTime();
    display.printTime(now.Hour(), now.Minute(), true);
  }
};

void setupRTC() {
  
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid())
  {
    if (Rtc.LastError() != 0)
    {
    }
    else
    {
      Rtc.SetDateTime(compiled);
    }
  }

  if (!Rtc.GetIsRunning())
  {
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Rtc.SetDateTime(compiled);
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}