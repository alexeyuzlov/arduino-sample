#include <IRremote.h>

int RECV_PIN = 11;

const int LED_PIN = 9;
boolean ledValue = false;
const unsigned long VALUE = 3772784863;
unsigned long latest = -1;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value);

    if (results.value == VALUE) {
      toggleLED();
      delay(100);
    }

    irrecv.resume(); // Receive the next value
  }

  delay(100);
}

void toggleLED() {
  ledValue = !ledValue;
  if (ledValue) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  Serial.println("Success, ledValue is: " + String(digitalRead(LED_PIN)));
}